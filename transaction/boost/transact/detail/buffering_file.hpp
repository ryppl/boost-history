//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TRANSACT_DETAIL_BUFFERING_FILE_HPP
#define BOOST_TRANSACT_DETAIL_BUFFERING_FILE_HPP

#include <boost/mpl/size_t.hpp>
#include <boost/static_assert.hpp>
#include <boost/assert.hpp>
#include <cstring>

namespace boost{
namespace transact{
namespace detail{

template<class Base,std::size_t Capacity>
class buffering_seq_ofile{
public:
    typedef typename Base::size_type size_type;
    explicit buffering_seq_ofile(std::string const &name)
        : base(name)
        , size(0){}
    template<class Size>
    void write(void const *data,Size s){
        if(this->size + s <= Capacity){
            std::memcpy(this->buffer+this->size,data,s);
            this->size+=s;
        }else this->write_overflow(data,s);
    }
    size_type position() const{
        return this->base.position() + this->size;
    }
    void flush(){
        this->flush_buffer();
        this->base.flush();
    }
    void sync(){
        //don't flush buffer! caller is responsible to call flush() inside a mutex lock.
        this->base.sync();
    }
    ~buffering_seq_ofile(){
        try{
            this->flush_buffer();
        }catch(...){
#ifndef NDEBUG
            std::cerr << "ignored exception" << std::endl;
#endif
        }
    }
private:
    void write_overflow(void const *data,std::size_t s){
        BOOST_ASSERT(this->size + s > Capacity);
        if(this->size == 0){
            this->base.write(data,s);
        }else{
            std::size_t write=Capacity - this->size;
            std::memcpy(this->buffer+this->size,data,write);
            this->size=Capacity;
            this->flush_buffer();
            this->write(static_cast<char const *>(data)+write,s-write);
        }
    }
    void flush_buffer(){
        if(this->size > 0){
            this->base.write(this->buffer,this->size);
            this->size=0;
        }
    }

    Base base;
    char buffer[Capacity];
    std::size_t size;
};


}
}
}



#endif

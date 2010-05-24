//          Copyright Stefan Strasser 2009 - 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)


#ifndef BOOST_TRANSACT_DETAIL_SYNCING_FILE_HPP
#define BOOST_TRANSACT_DETAIL_SYNCING_FILE_HPP

#include <string>
#include <cstring>
#include <boost/filesystem.hpp>
#include <boost/mpl/size_t.hpp>
#include <boost/static_assert.hpp>
#include <boost/assert.hpp>
#include <boost/config.hpp>
#include <boost/transact/detail/file.hpp>

namespace boost{
namespace transact{
namespace detail{

class syncing_seq_ofile{
public:
    typedef unsigned int size_type;
    explicit syncing_seq_ofile(std::string const &name);
    void write(void const *data,std::size_t size);
    size_type position() const{ return this->pos; }
    void flush();
    void sync();
private:
    size_type pos;
	ofile  filedes;

private:
    void write_ahead(size_type const &s){
        BOOST_ASSERT(s % write_ahead_size == 0);
        if(this->pos != s){
			filedes.seek(s);
        }
        char data[write_page_size]; memset(data,0,write_page_size);
        BOOST_STATIC_ASSERT(write_ahead_size % write_page_size == 0);
        for(std::size_t c=0;c<write_ahead_size / write_page_size;++c){
			filedes.write(data,write_page_size);
        }
		filedes.sync();
		filedes.seek(this->pos);
    }
    void write_ahead(size_type const &start,size_type const &end){
        BOOST_ASSERT(start % write_ahead_size == 0);
        BOOST_ASSERT(end % write_ahead_size == 0);
        for(size_type off=start;off < end;off+=write_ahead_size) this->write_ahead(off);
    }

    static std::size_t const write_ahead_size=10*1024*1024;
    static std::size_t const write_page_size=4096;
};

inline syncing_seq_ofile::syncing_seq_ofile(std::string const &name)
    : pos(0)
    , filedes(name)
{
    this->write_ahead(0);
}

void syncing_seq_ofile::write(void const *data,std::size_t size){
    size_type const s=this->pos % write_ahead_size;
    if(s + size >= write_ahead_size){ //there must be at least one 0 at the and, so also write ahead if this is equal.
        size_type start=this->pos - s + write_ahead_size;
        size_type end=start+((s + size)/write_ahead_size) * write_ahead_size; //usually == start + write_ahead_size, but "size" can theoretically span a whole write_ahead_size
        BOOST_ASSERT(end > start);
        this->write_ahead(start,end);
    }

	std::size_t ret= filedes.write((char const *)data,size);
    if(ret > 0) this->pos+=ret;
    if(ret != std::size_t(size)) throw io_failure();
}


inline void syncing_seq_ofile::flush(){}
	
inline void syncing_seq_ofile::sync(){
	filedes.sync();
}


}
}
}



#endif

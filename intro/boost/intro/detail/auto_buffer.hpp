//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_AUTO_BUFFER_HPP
#define BOOST_INTRO_DETAIL_AUTO_BUFFER_HPP

#include <boost/type_traits/is_pod.hpp>
#include <boost/intro/array_extension.hpp>
#include <cstring>

namespace boost{
namespace intro{
namespace detail{

template<class Derived,class T>
class vector_facade{
public:
    typedef T value_type;
    typedef T *iterator;
    typedef T const *const_iterator;
    typedef T &reference;
    typedef T const &const_reference;
    iterator begin(){ return that().data(); }
    const_iterator begin() const{ return that().data(); }
    iterator end(){ return that().data() + that().size(); }
    const_iterator end() const{ return that().data() + that().size(); }

    reference operator[](std::size_t n){ return *(begin()+n); }
    const_reference operator[](std::size_t n) const{ return *(begin()+n); }
    bool empty() const{ return begin() == end(); }
    reference back(){ return *(end()-1); }
    const_reference back() const{ return *(end()-1); }

    void reserve(std::size_t n){
        if(that().capacity() < n){
            that().grow(n);
        }
        BOOST_ASSERT(that().capacity() >= n);
    }
    void push_back(const_reference x){
        reserve(that().size() + 1);
        new (end()) value_type(x);
        that().size(that().size() + 1);
    }
    void resize(std::size_t n,const_reference x=value_type()){
        reserve(n);
        if(n > that().size()) construct(end(),begin() + n,x);
        else destruct(begin() + n,end());
        that().size(n);
    }
    void clear(){
        destruct(begin(),end());
        that().size(0);
    }
    //extension:
    template<class InPlaceFactory>
    void push_back(InPlaceFactory const &fac){
        reserve(that().size() + 1);
        fac.template apply<T>(end());
        that().size(that().size() + 1);
    }
    template<class Iterator,class Size>
    void push_back(Iterator it,Size size){
        std::size_t const newsize=that().size()+size;
        reserve(newsize);
        construct_n(end(),it,size);
        that().size(newsize);
    }
protected:
    void destruct(){
        destruct(begin(),end());
    }
    static void destruct(const_iterator it,const_iterator end){
        for(;it != end;++it) it->~T();
    }
    static void construct(iterator begin,iterator end,const_reference x){
        iterator it=begin;
        try{
            for(;it != end;++it) new (it) T();
        }catch(...){
            destruct(begin,it);
            throw;
        }
    }
    static void construct(iterator outbegin,const_iterator begin,const_iterator end){
        iterator out=outbegin;
        try{
            for(const_iterator it=begin;it != end;++it,++out)
                new (out) T(*it);
        }catch(...){
            destruct(outbegin,out);
            throw;
        }
    }
    template<class Iterator,class Size>
    static void construct_n(T *dest,Iterator it,Size n){
        construct_n(dest,it,n,
            mpl::bool_<is_pod<T>::type::value
                && intro::continuous_values<Iterator>::type::value>());
    }
    template<class Iterator,class Size>
    static void construct_n(T *dest,Iterator it,Size n,mpl::true_ pod){
        std::memcpy(dest,&*it,sizeof(T)*std::size_t(n));
    }
    template<class Iterator,class Size>
    static void construct_n(T *dest,Iterator it,Size n,mpl::false_ pod){
        std::size_t c;
        try{
            for(c=0;c<n;++c){
                new (dest+c) T(*it++);
            }
        }catch(...){
            destruct(dest,dest+c);
            throw;
        }
    }
private:
    Derived &that(){ return static_cast<Derived &>(*this); }
    Derived const &that() const{ return static_cast<Derived const &>(*this); }
};

template<class T,std::size_t Embed>
class auto_buffer
    : public vector_facade<auto_buffer<T,Embed>,T>{
    typedef vector_facade<auto_buffer,T> base_type;
public:
    auto_buffer()
        : data_(emb_data())
        , size_(0)
        , capacity_(Embed){}
    ~auto_buffer(){
        base_type::destruct();
        if(data_ != emb_data()) ::operator delete(data_);
    }
    T *data() const{ return data_; }
    std::size_t size() const{ return size_; }
    void size(std::size_t n){ size_=n; }
    std::size_t capacity() const{ return capacity_; }
    
    void grow(std::size_t n){
        std::size_t newcap=capacity() * 2;
        if(n > newcap) newcap=n;
        T *newdata=
            reinterpret_cast<T *>(::operator new(newcap * sizeof(T)));
        try{
            base_type::construct(newdata,this->begin(),this->end());
        }catch(...){
            ::operator delete(newdata);
            throw;
        }
        base_type::destruct(this->begin(),this->end());
        this->capacity_=newcap;
        if(data_ != emb_data()) ::operator delete(data_);
        this->data_=newdata;
    }
private:
    T *emb_data(){
        return reinterpret_cast<T *>(this->emb_data_);
    }
    char emb_data_[sizeof(T) * Embed];
    T *data_;
    std::size_t size_;
    std::size_t capacity_;
};

template<class T,std::size_t Capacity>
class static_buffer
    : public vector_facade<static_buffer<T,Capacity>,T>{
    typedef vector_facade<static_buffer,T> base_type;
public:
    static_buffer() : size_(0){}
    T *data() const{ return emb_data(); }
    std::size_t size() const{ return size_; }
    void size(std::size_t n){ size_=n; }
    std::size_t capacity() const{ return Capacity; }
    
    void grow(std::size_t){
        BOOST_ASSERT(false);
    }
private:
    T *emb_data() const{
        return const_cast<T *>(reinterpret_cast<T const *>(this->emb_data_));
    }
    char emb_data_[sizeof(T) * Capacity];
    std::size_t size_;
};

template<class T>
class vector_view
    : public vector_facade<vector_view<T>,T>{
    typedef vector_facade<vector_view,T> base_type;
public:
    vector_view(T *data,std::size_t &size,std::size_t capacity)
        : data_(data)
        , size_(size)
        , capacity_(capacity){}
    T *data() const{ return data_; }
    std::size_t size() const{ return size_; }
    void size(std::size_t n){ size_=n; }
    std::size_t capacity() const{ return capacity_; }
    void grow(std::size_t){
        BOOST_ASSERT(false);
    }
private:
    T *data_; 
    std::size_t &size_; //reference!
    std::size_t capacity_;
};


}

template<class T,std::size_t Embed>
struct array_extension<detail::auto_buffer<T,Embed> >
    : mpl::true_{};


}
}



#endif

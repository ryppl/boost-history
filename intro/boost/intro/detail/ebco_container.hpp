//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_DETAIL_EBCO_CONTAINER_HPP
#define BOOST_INTRO_DETAIL_EBCO_CONTAINER_HPP

#include <boost/type_traits/is_empty.hpp>
#include <boost/swap.hpp>

namespace boost{
namespace intro{
namespace detail{

template<class T,class Tag,bool empty>
class ebco_container_base;

template<class T,class Tag>
class ebco_container_base<T,Tag,true>{
protected:
    ebco_container_base(){}
    explicit ebco_container_base(T const &){}
    T get() const{ return T(); }
    void set(T const &){}
    void swap(ebco_container_base &){}
};

template<class T,class Tag>
class ebco_container_base<T,Tag,false>{
protected:
    ebco_container_base(){}
    explicit ebco_container_base(T const &t) : t(t){}
    T const &get() const{ return this->t; }
    void set(T const &nt){ this->t=nt; }
    void swap(ebco_container_base &rhs){
        boost::swap(t,rhs.t);
    }
private:
    T t;
};

template<class T,class Tag=void>
class ebco_container
    : protected ebco_container_base<T,Tag,is_empty<T>::value>{
    typedef ebco_container_base<T,Tag,is_empty<T>::value> base_type;
protected:
    ebco_container(){}
    explicit ebco_container(T const &t) : base_type(t){}
};


}
}
}


#endif

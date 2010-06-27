//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_REFERENCE_HPP
#define BOOST_INTRO_REFERENCE_HPP

#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/not.hpp>
#include <boost/assert.hpp>
#include <boost/noncopyable.hpp>
#include <boost/type_traits/is_class.hpp>

namespace boost{
namespace intro{

template<class T>
class reference : noncopyable{
    void true_type(){}
    typedef void (reference::*unspecified_bool_type)();
public:
    reference(T &t) : t(&t){}
    operator T &() const{ return *t; }
    T &get() const{ return *t; }
    T *get_pointer() const{ return t; }
    T *operator->() const{ return t; }
    ~reference(){ t=0; }
private:
    T *t;
};

template<class T>
struct is_reference_wrapper : mpl::false_{};
template<class T>
struct is_reference_wrapper<reference<T> > : mpl::true_{};
template<class T>
struct is_reference_wrapper<reference<T> const> : mpl::true_{};
template<class T>
struct is_reference_wrapper<reference<T> volatile> : mpl::true_{};
template<class T>
struct is_reference_wrapper<reference<T> const volatile> : mpl::true_{};
template<class T>
struct is_reference_wrapper<reference<T> const &> : mpl::true_{};
template<class T>
struct is_reference_wrapper<reference<T> volatile &> : mpl::true_{};
template<class T>
struct is_reference_wrapper<reference<T> const volatile &> : mpl::true_{};

template<class T>
struct is_class :
    mpl::and_<
        boost::is_class<T>,
        mpl::not_<is_reference_wrapper<T> >
    >{};



}
}


#endif

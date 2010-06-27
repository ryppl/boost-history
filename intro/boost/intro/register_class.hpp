//          Copyright Stefan Strasser 2010.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE_1_0.txt or copy at
//          http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_INTRO_REGISTER_CLASS_HPP
#define BOOST_INTRO_REGISTER_CLASS_HPP

#include <boost/intro/dispatch_polymorphic.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/is_base_of.hpp>


#define BOOST_INTRO_REGISTER_CLASS(Derived) \
    template<class Algo,class Base> \
    typename boost::mpl::eval_if_c< \
        !boost::is_same<Base,Derived>::value && boost::is_base_of<Base,Derived>::value, \
        boost::intro::detail::register_class<Algo,Base,Derived>, \
        boost::mpl::identity<void> \
    >::type \
    register_classes(Algo const *,Base *,Derived *,boost::intro::detail::adl_tag){}


#endif

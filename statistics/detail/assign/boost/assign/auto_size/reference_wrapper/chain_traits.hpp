//////////////////////////////////////////////////////////////////////////////
// assign::detail::chain_traits.hpp                                         //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_CHAIN_TRAITS_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_CHAIN_TRAITS_ER_2010_HPP
#include <boost/mpl/bool.hpp>

namespace boost{
namespace assign{
namespace detail{

// fwd declare
template<typename T> struct assign_reference_copy;
template<typename T> struct assign_reference_rebind;
template<typename T> struct is_ref_wrapper;

template<typename T>
struct is_ref_wrapper<assign_reference_copy<T> >
    : boost::mpl::bool_<true>{};
    
template<typename T>
struct is_ref_wrapper<assign_reference_rebind<T> >
    : boost::mpl::bool_<true>{};

namespace ref_wrapper_traits{
    template<typename T> struct tag_of;
    namespace tag{ struct default_; }

    template<typename T>
    struct tag_of< 
        boost::assign::detail::assign_reference_copy<T> 
    > 
    {
        typedef ref_wrapper_traits::tag::default_ type;
    };
    
    template<typename T>
    struct tag_of< 
        boost::assign::detail::assign_reference_rebind<T> 
    > 
    {
        typedef ref_wrapper_traits::tag::default_ type;
    };

}// ref_wrapper_traits
}// detail
}// assign
}// boost

#endif

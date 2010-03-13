//////////////////////////////////////////////////////////////////////////////
// assign::detail::reference_traits.hpp                                     //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_REFERENCE_TRAITS_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_REFERENCE_TRAITS_ER_2010_HPP
#include <utility>
#include <boost/mpl/and.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/assign/auto_size/chain/is_reference_wrapper.hpp>
#include <boost/assign/auto_size/chain/reference_wrapper_traits.hpp>

// Map T, a reference or a reference_wrapper, to a convertible type.

namespace boost{
namespace assign{
namespace detail{
namespace reference_traits{

    template<typename T>
    struct remove_cref : boost::remove_const<
        typename boost::remove_reference<T>::type
    >{};

    template<typename T>
    struct is_const : boost::is_const<
        typename boost::remove_reference<T>::type
    >{};

    template<bool use_ref,bool use_const,typename T>
    struct filter_ref_const{
       typedef typename reference_traits::remove_cref<T>::type value_;
       typedef typename boost::mpl::eval_if_c<
           use_const,
           boost::add_const<value_>,
           boost::mpl::identity<value_>
        >::type const_;
       typedef typename boost::mpl::eval_if_c<
           use_ref,
           boost::add_reference<const_>,
           boost::mpl::identity<const_>
        >::type type;
    };

    template<typename T>
    struct convert_wrapper{
        typedef typename reference_traits::remove_cref<T>::type value_;
        typedef typename detail::is_ref_wrapper<value_>::type is_rw_;
        typedef typename boost::mpl::eval_if_c<     
            is_rw_::value,
            ref_wrapper_traits::convertible_to<value_>,
            boost::mpl::identity<T>    
        >::type type;
    };

}// reference_traits
}// detail
}// assign
}// boost

#endif

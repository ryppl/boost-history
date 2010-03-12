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
#include <boost/assign/chain/is_reference_wrapper.hpp>
#include <boost/assign/chain/reference_wrapper_traits.hpp>

// Maps U to a convertible type where U can be a reference_wrapper or a ref.

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
    struct filter_wrapper{
        typedef typename reference_traits::remove_cref<T>::type value_;
        typedef typename detail::is_ref_wrapper<value_>::type is_rw_;
        typedef typename boost::mpl::eval_if_c<     
            is_rw_::value,
            ref_wrapper_traits::convertible_to<value_>,
            boost::mpl::identity<T>    
        >::type type;
    };

namespace pair{

    template<typename T,typename U>
    struct use_const : boost::mpl::or_<
        typename reference_traits::is_const<T>::type,
        typename reference_traits::is_const<U>::type
    >{};
        
    template<typename T,typename U>
    struct use_reference : boost::mpl::and_<
        typename boost::is_reference<T>::type,
        typename boost::is_reference<U>::type
    >{};
    
    template<typename T,typename U>
    struct filter_ref_const{
        typedef typename pair::use_reference<T,U>::type use_ref_;
        typedef typename pair::use_const<T,U>::type use_const_;
        template<typename V>
        struct local 
           : reference_traits::filter_ref_const<
               use_ref_::value,use_const_::value,V>{}; 
        typedef typename local<T>::type first_;
        typedef typename local<U>::type second_;
        typedef std::pair<first_,second_> type;
    };
}// pair
namespace convertible_to{

    template<typename T,typename U>
    struct ignore_wrapper
    {
        typedef typename pair::filter_ref_const<T,U>::type pair_;
        typedef typename pair_::first first_;
        typedef typename pair_::second second_;
        typedef typename boost::is_convertible<
             first_, // from
             second_ // to
        >::type use_second_;
        typedef typename boost::mpl::if_c<
            use_second_::value,
            second_,
            first_
        >::type type;
        BOOST_MPL_ASSERT((boost::is_convertible<first_,type>));
        BOOST_MPL_ASSERT((boost::is_convertible<second_,type>));
    };

    template<typename T,typename U>
    struct filter_wrapper : convertible_to::ignore_wrapper<
        typename reference_traits::filter_wrapper<T>::type,
        typename reference_traits::filter_wrapper<U>::type
    >{};

    namespace meta{

        template<template<typename,typename> class M>
        struct helper{
            template<typename T,typename U>
            struct apply : boost::mpl::identity< M<T,U> >{};    
        };
        struct ignore_wrapper : meta::helper<convertible_to::ignore_wrapper>{};
        struct filter_wrapper : meta::helper<convertible_to::filter_wrapper>{};
    }

}// convertible_to
}// reference_traits
}// detail
}// assign
}// boost

#endif

//////////////////////////////////////////////////////////////////////////////
// assign::detail::pair_traits.hpp                                          //
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_DETAIL_PAIR_TRAITS_ER_2010_HPP
#define BOOST_ASSIGN_DETAIL_PAIR_TRAITS_ER_2010_HPP
#include <utility>
#include <boost/mpl/and.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/mpl/or.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/type_traits/remove_const.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/assign/auto_size/chain/conversion_traits.hpp>
#include <boost/assign/auto_size/chain/reference_traits.hpp>

// Maps (T,U) to (convert_to<T>::type, convert_to<U>::type) and the latter
// to a type that is convertible to by both.


namespace boost{
namespace assign{
namespace detail{
namespace pair_traits{

    template<typename D>
    struct add_check_symmetry{
        static void check_symmetry()
        {
            typedef typename D::type type;
            typedef typename D::symmetric::type sym_type;
            BOOST_MPL_ASSERT((boost::is_same<type,sym_type>));
        }
    };

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
    struct filter_ref_const : add_check_symmetry< filter_ref_const<T,U> >{
        typedef typename pair_traits::use_reference<T,U>::type use_ref_;
        typedef typename pair_traits::use_const<T,U>::type use_const_;
        template<typename F,typename S>
        struct pair{
            typedef F first_type;
            typedef S second_type;
        };
        template<typename V>
        struct local 
           : reference_traits::filter_ref_const<
               use_ref_::value,use_const_::value,V>{}; 
        typedef typename local<T>::type first_;
        typedef typename local<U>::type second_;
        typedef pair<first_,second_> type;
        
        struct symmetric : filter_ref_const<U,T>{}; 
    };

    template<typename T,typename U>
    struct ignore_wrapper : add_check_symmetry< ignore_wrapper<T,U> >
    {
        typedef typename pair_traits::filter_ref_const<T,U>::type traits_;
        typedef typename traits_::first_type first_;
        typedef typename traits_::second_type second_;
        typedef 
            typename boost::is_convertible<first_,second_>::type use_second_;
        typedef typename 
            boost::mpl::if_c<use_second_::value,second_,first_>::type type;
            
        struct symmetric : ignore_wrapper<U,T>{};
        static void internal_check(){
          typedef add_check_symmetry< ignore_wrapper<T,U> > super_;
          super_::check_symmetry();
          BOOST_MPL_ASSERT((boost::is_convertible<first_,type>));
          BOOST_MPL_ASSERT((boost::is_convertible<second_,type>));
        }
    };
    
    template<typename T,typename U>
    struct apply_conversion : pair_traits::ignore_wrapper<
        typename reference_traits::convert_to<T>::type,
        typename reference_traits::convert_to<U>::type
    >{};

    namespace meta{

        template<template<typename,typename> class M>
        struct helper{
            template<typename T,typename U>
            struct apply : M<T,U>{};    
        };
        struct ignore_wrapper : meta::helper<pair_traits::ignore_wrapper>{};
        struct apply_conversion : meta::helper<pair_traits::apply_conversion>{};
    }

}// pair_traits
}// detail
}// assign
}// boost

#endif

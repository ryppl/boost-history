// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_LESS_EQUAL_HPP
#define BOOST_METAMATH_DOUBLE_LESS_EQUAL_HPP

#include <boost/mpl/less_equal.hpp>
#include <boost/metamath/double/double.hpp>
#include <boost/metamath/double/greater.hpp>
#include <boost/detail/workaround.hpp>

namespace boost { namespace mpl {
   template<>
   struct less_equal_impl< 
         metamath::double_c_tag
        ,metamath::double_c_tag
#if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
#endif
   >
   {
       template< typename N1, typename N2 > struct apply
           : metamath::detail::greater_double_double<N2,N1>::type
       {};
   };

   template<>
   struct less_equal_impl< 
         integral_c_tag
        ,metamath::double_c_tag
#if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
#endif
   >
   {
       template< typename N1, typename N2 > struct apply
           : metamath::detail::greater_double_double<metamath::integral_to_double<N2>,N1>::type
       {};
   };

   template<>
   struct less_equal_impl< 
         metamath::double_c_tag
        ,integral_c_tag
#if BOOST_WORKAROUND(BOOST_MSVC,<=1300)
        ,0
        ,0
#endif
   >
   {
       template< typename N1, typename N2 > struct apply
           : metamath::detail::greater_double_double<N2,metamath::integral_to_double<N1> >::type
       {};
   };
}}

#endif //BOOST_METAMATH_DOUBLE_LESS_HPP

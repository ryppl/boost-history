/*-----------------------------------------------------------------------------+    
Copyright (c) 2010-2010: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_IS_DOMAIN_COMPARABLE_HPP_JOFA_100925
#define BOOST_ITL_TYPE_TRAITS_IS_DOMAIN_COMPARABLE_HPP_JOFA_100925

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/mpl/and.hpp> 
#include <boost/mpl/not.hpp> 
#include <boost/itl/type_traits/domain_type_of.hpp> 

namespace boost{ namespace itl
{
    namespace detail
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(domain_param)
        BOOST_MPL_HAS_XXX_TRAIT_DEF(domain_compare)
    }

    template <class Type>
    struct is_domain_comparable 
      : mpl::bool_
	  <
          detail::has_domain_type<Type>::value &&
          detail::has_domain_param<Type>::value &&
          detail::has_domain_compare<Type>::value 
	  >
    {};

}} // namespace boost itl

#endif



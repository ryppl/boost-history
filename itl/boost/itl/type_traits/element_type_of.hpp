/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_ELEMENT_TYPE_OF_HPP_JOFA_100902
#define BOOST_ITL_TYPE_TRAITS_ELEMENT_TYPE_OF_HPP_JOFA_100902

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/itl/type_traits/no_type.hpp>

namespace boost{ namespace itl
{
    namespace detail
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(element_type)
    }

    template <class Type>
    struct has_element_type 
      : mpl::bool_<detail::has_element_type<Type>::value>
    {};

    template <class Type, bool has_element_type> 
	struct get_element_type;

    template <class Type>
	struct get_element_type<Type, false>
    {
		typedef no_type type;
    };

    template <class Type>
	struct get_element_type<Type, true>
    {
		typedef typename Type::element_type type;
    };

    template <class Type>
	struct element_type_of
    {
		typedef typename 
			get_element_type<Type, has_element_type<Type>::value>::type type;
    };

}} // namespace boost itl

#endif



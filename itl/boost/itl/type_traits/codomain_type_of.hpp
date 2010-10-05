/*-----------------------------------------------------------------------------+    
Copyright (c) 2008-2009: Joachim Faulhaber
+------------------------------------------------------------------------------+
   Distributed under the Boost Software License, Version 1.0.
      (See accompanying file LICENCE.txt or copy at
           http://www.boost.org/LICENSE_1_0.txt)
+-----------------------------------------------------------------------------*/
#ifndef BOOST_ITL_TYPE_TRAITS_CODOMAIN_TYPE_OF_HPP_JOFA_100829
#define BOOST_ITL_TYPE_TRAITS_CODOMAIN_TYPE_OF_HPP_JOFA_100829

#include <set>
#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/bool.hpp>
#include <boost/itl/type_traits/no_type.hpp>

namespace boost{ namespace itl
{
    namespace detail
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(codomain_type)
    }

    template <class Type>
    struct has_codomain_type 
      : mpl::bool_<detail::has_codomain_type<Type>::value>
    {};

    template <class Type, bool has_codomain_type> 
    struct get_codomain_type;

    template <class Type>
    struct get_codomain_type<Type, false>
    {
        typedef no_type type;
    };

    template <class Type>
    struct get_codomain_type<Type, true>
    {
        typedef typename Type::codomain_type type;
    };

    template <class Type>
    struct codomain_type_of
    {
        typedef typename 
            get_codomain_type<Type, has_codomain_type<Type>::value>::type type;
    };

    template <class DomainT, class Compare, class Alloc>
    struct codomain_type_of<std::set<DomainT,Compare,Alloc> >
    {
        typedef typename std::set<DomainT,Compare,Alloc>::value_type type;
    };

}} // namespace boost itl

#endif



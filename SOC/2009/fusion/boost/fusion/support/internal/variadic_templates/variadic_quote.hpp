/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_SUPPORT_INTERNAL_VARIADIC_TEMPLATES_VARIADIC_QUOTE_HPP
#define BOOST_FUSION_SUPPORT_INTERNAL_VARIADIC_TEMPLATES_VARIADIC_QUOTE_HPP

//TODO cschmidt: Are there any compiler that support variadic templates but
//BOOST_MPL_CFG_NO_HAS_XXX ?!

#include <boost/mpl/has_xxx.hpp>

namespace boost { namespace fusion { namespace detail
{
    BOOST_MPL_HAS_XXX_TRAIT_DEF(type)

    template<template<typename...> class Type>
    struct variadic_quote
    {
    private:
        template<typename ConcreteType,bool/* HasType*/>
        struct apply_impl
        {
            typedef ConcreteType type;
        };

        template<typename ConcreteType>
        struct apply_impl<ConcreteType,true>
          : ConcreteType
        {};

    public:
        template<typename... Argument>
        struct apply
          : apply_impl<
                Type<Argument...>
              , has_type<Type<Argument...> >::type::value
            >
        {};
    };
}}}

#endif
// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_SUPPORT_VARIADIC_TEMPLATES_VARIADIC_QUOTE_HPP
#define BOOST_FUSION_SUPPORT_VARIADIC_TEMPLATES_VARIADIC_QUOTE_HPP

//TODO cschmidt: Are there any compiler that support variadic templates but
//BOOST_MPL_CFG_NO_HAS_XXX ?!

#include <boost/mpl/has_xxx.hpp>
#include <boost/mpl/bool.hpp>

namespace boost{namespace fusion
{
    namespace detail
    {
        BOOST_MPL_HAS_XXX_TRAIT_DEF(type)
    }

    template<template<typename...> class Type>
    struct variadic_quote
    {
    private:
        template<typename ConcreteType,typename/* HasType*/>
        struct apply_impl
        {
            typedef ConcreteType type;
        };

        template<typename ConcreteType>
        struct apply_impl<ConcreteType,mpl::true_>
          : ConcreteType
        {
        };

    public:
        template<typename... Argument>
        struct apply
          : apply_impl<Type<Argument...>,detail::has_type<Type<Argument...> > >
        {
        };
    };

}}
#endif

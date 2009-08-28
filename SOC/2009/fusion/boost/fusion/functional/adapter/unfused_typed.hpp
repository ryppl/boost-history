/*=============================================================================
    Copyright (c) 2009 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_TYPED_HPP
#define BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_TYPED_HPP

#include <boost/config.hpp>
#if defined(BOOST_NO_DECLTYPE) || defined(BOOST_NO_VARIADIC_TEMPLATES) || defined(BOOST_NO_RVALUE_REFERENCES) || 1
#   include <boost/fusion/functional/adapter/detail/no_0x/unfused_typed.hpp>
#else
#   define BOOST_FUSION_TYPED
#   include <boost/fusion/functional/adapter/detail/0x/unfused_impl.hpp>
#   undef BOOST_FUSION_TYPED
#endif

#endif

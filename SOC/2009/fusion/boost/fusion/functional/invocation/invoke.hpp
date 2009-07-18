// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_FUNCTIONAL_INVOCATION_INVOKE_HPP
#define BOOST_FUSION_FUNCTIONAL_INVOCATION_INVOKE_HPP

#include <boost/config.hpp>
#if defined(BOOST_NO_DECLTYPE) || defined(BOOST_NO_VARIADIC_TEMPLATES)
#   include <boost/fusion/functional/invocation/detail/no_decltype_or_variadic_templates/invoke.hpp>
#else
#   define BOOST_FUSION_INVOKE_NAME invoke
#   include <boost/fusion/functional/invocation/detail/decltype_and_variadic_templates/invoke_impl.hpp>
#   undef BOOST_FUSION_INVOKE_NAME
#endif

#endif

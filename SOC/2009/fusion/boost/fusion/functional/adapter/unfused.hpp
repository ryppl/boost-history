// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_HPP
#define BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_HPP

#include <boost/config.hpp>
#if defined(BOOST_NO_DECLTYPE) || defined(BOOST_NO_VARIADIC_TEMPLATES) || defined(BOOST_NO_RVALUE_REFERENCES)
#   include <boost/fusion/functional/adapter/detail/no_0x/unfused.hpp>
#elif !defined(BOOST_FUSION_CPP0X_NO_DEPRECEATED)
#   include <boost/fusion/functional/adapter/detail/0x/unfused_impl.hpp>
#endif

#endif

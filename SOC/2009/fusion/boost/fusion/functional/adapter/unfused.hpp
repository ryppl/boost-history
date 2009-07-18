// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_HPP
#define BOOST_FUSION_FUNCTIONAL_ADAPTER_UNFUSED_HPP

#include <boost/config.hpp>
//TODO cschmidt!!!
#if defined(BOOST_NO_DECLTYPE) || defined(BOOST_NO_VARIADIC_TEMPLATES)// || !defined(BOOST_FUSION_CPP0X_NO_DEPRECEATED)
#   include <boost/fusion/functional/adapter/detail/no_decltype_or_variadic_templates/unfused.hpp>
#else
#   include <boost/fusion/functional/adapter/detail/decltype_and_variadic_templates/unfused_impl.hpp>
#endif

#endif

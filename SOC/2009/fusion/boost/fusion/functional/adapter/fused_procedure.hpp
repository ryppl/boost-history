// Copyright Christopher Schmidt 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_FUSION_FUNCTIONAL_ADAPTER_FUSED_PROCEDURE_HPP
#define BOOST_FUSION_FUNCTIONAL_ADAPTER_FUSED_PROCEDURE_HPP

#include <boost/config.hpp>
#if defined(BOOST_NO_DECLTYPE) || defined(BOOST_NO_VARIADIC_TEMPLATES)
#   include <boost/fusion/functional/adapter/detail/no_decltype_or_variadic_templates/fused_procedure.hpp>
#elif !defined(BOOST_FUSION_CPP0X_NO_DEPRECEATED)
#   define BOOST_FUSION_ADAPTER_NAME fused_procedure
#   include <boost/fusion/functional/adapter/detail/decltype_and_variadic_templates/fused_impl.hpp>
#   undef BOOST_FUSION_ADAPTER_NAME
#endif

#endif

/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#ifndef BOOST_FUSION_FUNCTIONAL_GENERATION_MAKE_FUSED_PROCEDURE_HPP
#define BOOST_FUSION_FUNCTIONAL_GENERATION_MAKE_FUSED_PROCEDURE_HPP

#include <boost/config.hpp>
#if defined(BOOST_NO_DECLTYPE) || defined(BOOST_NO_VARIADIC_TEMPLATES) || !defined(BOOST_FUSION_CPP0X_NO_DEPRECEATED)
#   include <boost/fusion/functional/adapter/fused_procedure.hpp>

#   define BOOST_FUSION_CLASS_TPL_NAME fused_procedure
#   include <boost/fusion/functional/generation/detail/gen_make_adapter.hpp>
#   undef BOOST_FUSION_CLASS_TPL_NAME
#endif

#endif

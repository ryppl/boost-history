/*=============================================================================
    Copyright (c) 2006-2007 Tobias Schwinger
    Copyright (c) 2009-2010 Christopher Schmidt
  
    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#ifndef BOOST_FUSION_FUNCTIONAL_ADAPTER_LIMITS_HPP
#define BOOST_FUSION_FUNCTIONAL_ADAPTER_LIMITS_HPP

#include <boost/config.hpp>

#if defined(BOOST_NO_VARIADIC_TEMPLATES) || defined(BOOST_NO_RVALUE_REFERENCES) || !defined(BOOST_FUSION_CPP0X_NO_DEPRECEATED)
#   include <boost/fusion/container/vector/limits.hpp>

#   ifndef BOOST_FUSION_UNFUSED_MAX_ARITY
#       define BOOST_FUSION_UNFUSED_MAX_ARITY 6
#   elif BOOST_FUSION_UNFUSED_GENERIC_MAX_ARITY > FUSION_MAX_VECTOR_SIZE
#       error "BOOST_FUSION_UNFUSED_GENERIC_MAX_ARITY > FUSION_MAX_VECTOR_SIZE"
#   endif

#   ifndef BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY
#       define BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY 6
#   elif BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY > FUSION_MAX_VECTOR_SIZE
#       error "BOOST_FUSION_UNFUSED_TYPED_MAX_ARITY > FUSION_MAX_VECTOR_SIZE"
#   endif
#endif

#endif

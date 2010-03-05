/*=============================================================================
    Copyright (c) 2007 Tobias Schwinger

    Use modification and distribution are subject to the Boost Software 
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt).
==============================================================================*/

#ifndef BOOST_FUSION_FUNCTIONAL_GENERATION_MAKE_UNFUSED_HPP
#define BOOST_FUSION_FUNCTIONAL_GENERATION_MAKE_UNFUSED_HPP

#include <boost/fusion/functional/adapter/unfused.hpp>

#define BOOST_FUSION_FUSED_CLASS_NAME unfused
#include <boost/fusion/functional/generation/detail/gen_make_adapter.hpp>
#undef BOOST_FUSION_FUSED_CLASS_NAME

#endif

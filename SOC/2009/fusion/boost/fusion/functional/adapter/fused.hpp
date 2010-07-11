/*==============================================================================
    Copyright (c) 2006-2007 Tobias Schwinger
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_FUNCTIONAL_ADAPTER_FUSED_HPP
#define BOOST_FUSION_FUNCTIONAL_ADAPTER_FUSED_HPP

#include <boost/fusion/functional/invocation/invoke.hpp>

#define BOOST_FUSION_INVOCATION_METHOD invoke
#define BOOST_FUSION_ADAPTER_NAME fused
#include <boost/fusion/functional/adapter/detail/fused_impl.hpp>
#undef BOOST_FUSION_ADAPTER_NAME
#undef BOOST_FUSION_INVOCATION_METHOD

#endif

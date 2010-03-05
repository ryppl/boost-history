/*=============================================================================
    Copyright (c) 2005-2006 Joao Abecasis
    Copyright (c) 2006-2007 Tobias Schwinger
    Copyright (c) 2009-2010 Christopher Schmidt

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/

#ifndef BOOST_FUSION_FUNCTIONAL_INVOCATION_INVOKE_PROCEDURE_HPP
#define BOOST_FUSION_FUNCTIONAL_INVOCATION_INVOKE_PROCEDURE_HPP

#include <boost/fusion/functional/invocation/limits.hpp>

#define BOOST_FUSION_INVOKE_IMPL_MAX_ARITY BOOST_FUSION_INVOKE_PROCEDURE_MAX_ARITY
#define BOOST_FUSION_RETURN_VOID
#define BOOST_FUSION_INVOKE_NAME invoke_procedure
#include <boost/fusion/functional/invocation/detail/invoke.hpp>
#undef BOOST_FUSION_INVOKE_NAME
#undef BOOST_FUSION_RETURN_VOID
#undef BOOST_FUSION_INVOKE_IMPL_MAX_ARITY

#endif

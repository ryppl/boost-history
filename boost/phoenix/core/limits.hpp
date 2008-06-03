/*=============================================================================
    Copyright (c) 2001-2007 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#ifndef PHOENIX_CORE_LIMITS_HPP
#define PHOENIX_CORE_LIMITS_HPP

#include <boost/preprocessor/selection/min.hpp>
#include <boost/preprocessor/selection/max.hpp>

#if !defined(PHOENIX_LIMIT)
# define PHOENIX_LIMIT 10
#endif

#if !defined(PHOENIX_ARG_LIMIT)
# define PHOENIX_ARG_LIMIT PHOENIX_LIMIT
#endif

#if !defined(PHOENIX_PERFECT_FORWARD_LIMIT)
# define PHOENIX_PERFECT_FORWARD_LIMIT 0
#endif

#if !defined(PHOENIX_ACTOR_LIMIT)
# define PHOENIX_ACTOR_LIMIT PHOENIX_LIMIT
#elif (PHOENIX_ACTOR_LIMIT > PHOENIX_LIMIT)
# error PHOENIX_ACTOR_LIMIT is set too high
#endif

#if !defined(FUSION_MAX_TUPLE_SIZE)
# define FUSION_MAX_TUPLE_SIZE PHOENIX_LIMIT
#elif (FUSION_MAX_TUPLE_SIZE < PHOENIX_LIMIT)
# error FUSION_MAX_TUPLE_SIZE is set too low
#endif

#if !defined(BOOST_PROTO_MAX_FUNCTION_CALL_ARITY)
# define BOOST_PROTO_MAX_FUNCTION_CALL_ARITY BOOST_PP_MIN(PHOENIX_LIMIT, 6)
#endif

#if defined(BOOST_PROTO_MAX_ARITY)
# if BOOST_PROTO_MAX_ARITY < PHOENIX_LIMIT
#  error BOOST_PROTO_MAX_ARITY must be at least as large as PHOENIX_LIMIT
# endif
#else
# define BOOST_PROTO_MAX_ARITY BOOST_PP_MAX(PHOENIX_LIMIT, 5)
#endif

#if defined(BOOST_MPL_LIMIT_METAFUNCTION_ARITY)
# if BOOST_MPL_LIMIT_METAFUNCTION_ARITY < PHOENIX_LIMIT
#  error BOOST_MPL_LIMIT_METAFUNCTION_ARITY must be at least as large as PHOENIX_LIMIT
# endif
#else
# define BOOST_MPL_LIMIT_METAFUNCTION_ARITY BOOST_PP_MAX(PHOENIX_LIMIT, 5)
#endif

//// this include will bring in mpl::vectorN and
//// fusion::tupleN where N is PHOENIX_ACTOR_LIMIT
//#include <boost/fusion/include/vector.hpp>

// OK, limits set. Include Proto.
#include <boost/proto/proto.hpp>

#endif

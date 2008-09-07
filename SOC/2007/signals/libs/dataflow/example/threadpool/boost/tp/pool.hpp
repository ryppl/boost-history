//  Copyright (c) 2008 Oliver Kowalke. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_TP_POOL_H
#define BOOST_TP_POOL_H

#include <boost/preprocessor/repetition.hpp>

#include <boost/tp/fixed.hpp>
#include <boost/tp/unbounded_channel.hpp>

namespace boost { namespace tp
{
#ifndef BOOST_TP_MAX_ARITY
# define BOOST_TP_MAX_ARITY 8
#endif

// Expands as: A0& a0, A1& a1, A2& a2, ... AN& aN
#define BOOST_TP_CTOR_ARG(z, n, A) \
    BOOST_PP_CAT(A, n) const& BOOST_PP_CAT(a, n)
#define BOOST_ENUM_TP_CTOR_ARGS(n, A) BOOST_PP_ENUM(n, BOOST_TP_CTOR_ARG, A)

template<
	typename Strategy = fixed,
	typename Channel = unbounded_channel<>
>
struct pool
: public Strategy::template impl< Channel >
{
	typedef Strategy							strategy_type;
	typedef Channel								channel_type;
	typedef typename channel_type::queue_type	queue_type;

#define BOOST_TP_CTOR(z, n, A)											\
	template< BOOST_PP_ENUM_PARAMS(n, typename A) >			\
	pool(BOOST_ENUM_TP_CTOR_ARGS(n, A))									\
	: Strategy::template impl< Channel >(BOOST_PP_ENUM_PARAMS(n, a))	\
	{}
	BOOST_PP_REPEAT_FROM_TO( 1, BOOST_TP_MAX_ARITY, BOOST_TP_CTOR, A)
#undef BOOST_TP_CTOR
};

#undef BOOST_ENUM_TP_CTOR_ARGS
#undef BOOST_TP_CTOR_ARG
#undef BOOST_TP_MAX_ARITY
} }

#endif // BOOST_TP_POOL_H

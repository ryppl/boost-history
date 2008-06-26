/**
 * \file boost/mirror/algorithm/detail/accumulate.hpp
 * Implementation of the accumulate algorithm 
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_DETAIL_ACCUMULATE_HPP
#define BOOST_MIRROR_ALGORITHM_DETAIL_ACCUMULATE_HPP

#include <boost/mirror/algorithm/detail/iterative.hpp>
#include <boost/mirror/algorithm/next.hpp>

namespace boost {
namespace mirror {
namespace detail {

template <
	class Current,
	class End,
	class State,
	class ForwardOp
>
struct accumulate
{
	typedef typename accumulate<
		typename next<Current>::type,
		End,
		typename ForwardOp::template apply<
			State, 
			typename deref<Current>::type
		>::type,
		ForwardOp
	>::type type;
};

template <
	class End,
	class State,
	class ForwardOp
>
struct accumulate<
	End,
	End,
	State,
	ForwardOp
>
{
	typedef State type;
};



} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard


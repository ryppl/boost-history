/**
 * \file boost/mirror/algorithm/accumulate.hpp
 *
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_ACCUMULATE_HPP
#define BOOST_MIRROR_ALGORITHM_ACCUMULATE_HPP

#include <boost/mirror/algorithm/detail/accumulate.hpp>
#include <boost/mirror/algorithm/begin.hpp>
#include <boost/mirror/algorithm/end.hpp>

namespace boost {
namespace mirror {

template <
	class MetaObjectSequence,
	class InitialState,
	class ForwardOp
>
struct accumulate
: detail::accumulate<
	typename begin<MetaObjectSequence>::type,
	typename end<MetaObjectSequence>::type,
	InitialState,
	ForwardOp
>
{ };

template <
	class IteratorBegin,
	class IteratorEnd,
	class InitialState,
	class ForwardOp
>
struct accumulate_on_range
: detail::accumulate<
	IteratorBegin,
	IteratorEnd,
	InitialState,
	ForwardOp
>
{ };


} // namespace mirror
} // namespace boost

#endif //include guard


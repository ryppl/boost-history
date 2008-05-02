/**
 * \file boost/mirror/algorithm/detail/no_op.hpp
 * Dummy no-op functor 
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_DETAIL_NO_OP_HPP
#define BOOST_MIRROR_ALGORITHM_DETAIL_NO_OP_HPP


namespace boost {
namespace mirror {
namespace detail {

struct no_op
{
	template <typename ArgType>
	inline ArgType operator()(ArgType arg){return arg;}
};

} // namespace detail
} // namespace mirror
} // namespace boost

#endif //include guard


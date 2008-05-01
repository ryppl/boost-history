/**
 * \file boost/mirror/function/identity_op.hpp
 *
 * Functor that just returns its single argument.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_FUNCTION_IDENTITY_OP_HPP
#define BOOST_MIRROR_FUNCTION_IDENTITY_OP_HPP

namespace boost {
namespace mirror {

struct identity_op
{
	template <class argument_type>
	inline argument_type operator()(argument_type arg)
	{
		return arg;
	}
};

} // namespace mirror
} // namespace boost

#endif //include guard


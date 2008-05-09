/**
 * \file boost/mirror/algorithm/prior.hpp
 * Returns the iterator pointing to the prior element
 * in a meta-object sequence.
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_PRIOR_HPP
#define BOOST_MIRROR_ALGORITHM_PRIOR_HPP

#include <boost/mirror/algorithm/detail/iterator.hpp>

namespace boost {
namespace mirror {

/** Prior iterator
 */
template <class Iterator>
struct prior
{
	typedef typename mpl::apply<
		typename Iterator::get_prior_iterator,
		typename Iterator::position
	>::type type;
};

} // namespace mirror
} // namespace boost

#endif //include guard


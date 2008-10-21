/**
 * \file boost/mirror/iterator/is_valid.hpp
 *
 *  returns mpl::true_ if the passed iterator is valid
 *  returns mpl::false_ otherwise
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ITERATOR_IS_VALID_HPP
#define BOOST_MIRROR_ITERATOR_ID_VALID_HPP

#include <boost/mpl/times.hpp>
#include <boost/mpl/and.hpp>
#include <boost/mpl/less_equal.hpp>
#include <boost/mpl/less.hpp>

namespace boost {
namespace mirror {

template <
	class Iterator
>
struct is_valid 
{
	typedef typename mpl::and_<
		typename mpl::less_equal<
			typename mpl::times<
				typename Iterator::begin_position,
				typename Iterator::direction
			>::type,
			typename mpl::times<
				typename Iterator::position,
				typename Iterator::direction
			>::type
		>::type,
		typename mpl::less<
			typename mpl::times<
				typename Iterator::position,
				typename Iterator::direction
			>::type,
			typename mpl::times<
				typename Iterator::end_position,
				typename Iterator::direction
			>::type
		>::type
	>::type type;
};

} // namespace mirror
} // namespace boost

#endif //include guard


/**
 * \file boost/mirror/iterator/iterator_not_equal.hpp
 * 
 * Iterator inequality comparison
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ITERATOR_ITERATOR_NOT_EQUAL_HPP
#define BOOST_MIRROR_ITERATOR_ITERATOR_NOT_EQUAL_HPP

#include <boost/mirror/iterator/detail/iterator_comparison.hpp>
#include <boost/mpl/not_equal_to.hpp>

namespace boost {
namespace mirror {

template <class I1, class I2>
struct iterator_not_equal
: detail::compare_iterators<I1, I2, mpl::not_equal_to>
{ };


} // namespace mirror
} // namespace boost

#endif //include guard


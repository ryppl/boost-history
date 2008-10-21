/**
 * \file boost/mirror/iterator/iterator_greater.hpp
 * 
 * Iterator greater than comparison
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ITERATOR_ITERATOR_GREATER_HPP
#define BOOST_MIRROR_ITERATOR_ITERATOR_GREATER_HPP

#include <boost/mirror/iterator/detail/iterator_comparison.hpp>
#include <boost/mpl/greater.hpp>

namespace boost {
namespace mirror {

template <class I1, class I2>
struct iterator_greater
: detail::compare_iterators<I1, I2, mpl::greater>
{ };


} // namespace mirror
} // namespace boost

#endif //include guard


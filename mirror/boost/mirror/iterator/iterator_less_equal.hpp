/**
 * \file boost/mirror/iterator/iterator_less_equal.hpp
 * 
 * Iterator less than or equal comparison
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ITERATOR_ITERATOR_LESS_EQUAL_HPP
#define BOOST_MIRROR_ITERATOR_ITERATOR_LESS_EQUAL_HPP

#include <boost/mirror/iterator/detail/iterator_comparison.hpp>
#include <boost/mpl/less_equal.hpp>

namespace boost {
namespace mirror {

template <class I1, class I2>
struct iterator_less_equal
: detail::compare_iterators<I1, I2, mpl::less_equal>
{ };


} // namespace mirror
} // namespace boost

#endif //include guard


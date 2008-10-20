/**
 * \file boost/mirror/algorithm/iterator_less.hpp
 * 
 * Iterator less than comparison
 *
 *  Copyright 2008 Matus Chochlik. Distributed under the Boost
 *  Software License, Version 1.0. (See accompanying file
 *  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#ifndef BOOST_MIRROR_ALGORITHM_ITERATOR_LESS_HPP
#define BOOST_MIRROR_ALGORITHM_ITERATOR_LESS_HPP

#include <boost/mirror/algorithm/detail/iterator_comparison.hpp>
#include <boost/mpl/less.hpp>

namespace boost {
namespace mirror {

template <class I1, class I2>
struct iterator_less
: detail::compare_iterators<I1, I2, mpl::less>
{ };


} // namespace mirror
} // namespace boost

#endif //include guard


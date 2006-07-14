// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/detail/set_view_iterator.hpp
/// \brief Iterator adaptors from multi index to bimap.

#ifndef BOOST_BIMAP_VIEWS_DETAIL_SET_VIEW_ITERATOR_HPP
#define BOOST_BIMAP_VIEWS_DETAIL_SET_VIEW_ITERATOR_HPP

#include <boost/iterator/iterator_adaptor.hpp>

#include <boost/bimap/detail/core_iterator_type_of.hpp>
#include <boost/bimap/relation/support/value_type_of.hpp>

namespace boost {
namespace bimap {
namespace views {
namespace detail {

// set view iterator

// At least for now, the set iterator will be the multi_index iterator

// typedef -undefined bimap-::iterator set_view_iterator;
// typedef -undefined bimap-::const_iterator set_view_const_iterator;
// typedef -undefined bimap-::reverse_iterator set_view_reverse_iterator;
// typedef -undefined bimap-::const_reverse_iterator set_view_const_reverse_iterator;


} // namespace detail
} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_DETAIL_SET_VIEW_ITERATOR_HPP




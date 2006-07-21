// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/detail/map_view_iterator.hpp
/// \brief Iterator adaptors from multi index to bimap.

#ifndef BOOST_BIMAP_VIEWS_DETAIL_MAP_VIEW_ITERATOR_HPP
#define BOOST_BIMAP_VIEWS_DETAIL_MAP_VIEW_ITERATOR_HPP

#include <boost/iterator/transform_iterator.hpp>
#include <boost/bimap/relation/support/get_pair_functor.hpp>

namespace boost {
namespace bimap {
namespace views {
namespace detail {

/// \brief Iterator adaptor from multi index to bimap.

template< class Tag, class Relation, class CoreIterator, class ValueType >
struct map_view_iterator
{
    typedef transform_iterator
    <
        relation::support::GetPairFunctor<Tag,Relation>,
        CoreIterator,
        ValueType &

    > type;
};

} // namespace detail
} // namesapce views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_DETAIL_MAP_VIEW_ITERATOR_HPP



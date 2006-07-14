// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.

/// \file views/detail/to_pair_from_relation.hpp
/// \brief Relation to pair value converter.

#ifndef BOOST_BIMAP_VIEWS_DETAIL_TO_PAIR_FROM_RELATION_MAP_VIEW_HPP
#define BOOST_BIMAP_VIEWS_DETAIL_TO_PAIR_FROM_RELATION_MAP_VIEW_HPP

#include <boost/bimap/relation/support/pair_by.hpp>

namespace boost {
namespace bimap {
namespace views {
namespace detail {

/// \brief Relation to pair value converter.

template< class Tag, class Relation >
struct to_pair_view_from_relation
{
    const typename relation::support::pair_type_by<Tag,Relation>::type &
    operator()(const Relation & r) const
    {
        return relation::support::pair_by<Tag>(r);
    }

    typename relation::support::pair_type_by<Tag,Relation>::type &
    operator()(Relation & r) const
    {
        return relation::support::pair_by<Tag>(r);
    }
};

} // namespace detail
} // namespace views
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_VIEWS_DETAIL_TO_PAIR_FROM_RELATION_HPP


// MISC Multi Index Specialized Containers
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/misc for library home page.


#ifndef BOOST_BIMAP_DETAIL_CORE_ITERATOR_TYPE_OF_HPP
#define BOOST_BIMAP_DETAIL_CORE_ITERATOR_TYPE_OF_HPP

#include <boost/bimap/relation/detail/metadata_access_builder.hpp>

namespace boost {
namespace bimap {
namespace detail {

// Metafunction core iterator type of

/*

    template< class Tag, class Relation >
    struct core_iterator_type_of
    {
        typedef -UNDEFINED- type;
    };

*/

BOOST_BIMAP_SYMMETRIC_METADATA_ACCESS_BUILDER
(
    core_iterator_type_of,
    left_core_iterator_type,
    right_core_iterator_type
);

} // namespace detail
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DETAIL_CORE_ITERATOR_TYPE_BY_HPP


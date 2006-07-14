// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

#ifndef BOOST_BIMAP_COLLECTION_DETAIL_VIEW_BINDER_HPP
#define BOOST_BIMAP_COLLECTION_DETAIL_VIEW_BINDER_HPP

/// \file collection/detail/view_binder.hpp
/// \brief Define macros to help building metafunctions

#include <boost/utility/enable_if.hpp>
#include <boost/multi_index/tag.hpp>
#include <boost/bimap/support/key_type_of.hpp>

#include <boost/bimap/collection/support/compute_view_type.hpp>

/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_COLLECTION_TO_MAP_VIEW_TYPE(                              \
                                                                              \
    IS_KEY_TYPE_METAFUNCTION,                                                 \
    MAP_VIEW_TYPE                                                             \
                                                                              \
)                                                                             \
                                                                              \
namespace support {                                                           \
                                                                              \
template< class Tag, class BimapType >                                        \
struct compute_map_view_type                                                  \
<                                                                             \
    Tag, BimapType,                                                           \
    typename enable_if                                                        \
    <                                                                         \
        IS_KEY_TYPE_METAFUNCTION                                              \
        <                                                                     \
            typename bimap::support::key_type_of<Tag,BimapType>::type         \
        >                                                                     \
                                                                              \
    >::type                                                                   \
>                                                                             \
{                                                                             \
    typedef MAP_VIEW_TYPE                                                     \
    <                                                                         \
        Tag,                                                                  \
        BimapType                                                             \
                                                                              \
    > type;                                                                   \
};                                                                            \
                                                                              \
} // namespace support

/*///////////////////////////////////////////////////////////////////////////*/



/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_COLLECTION_TO_SET_VIEW_TYPE(                              \
                                                                              \
    IS_KEY_TYPE_OF_METAFUNCTION,                                              \
    SET_VIEW_TYPE                                                             \
                                                                              \
)                                                                             \
                                                                              \
namespace support {                                                           \
                                                                              \
template< class SetOfRelationType, class IndexType >                          \
struct compute_set_view_type                                                  \
<                                                                             \
    SetOfRelationType, IndexType,                                             \
    typename enable_if                                                        \
    <                                                                         \
        IS_KEY_TYPE_OF_METAFUNCTION<SetOfRelationType>                        \
                                                                              \
    >::type                                                                   \
>                                                                             \
{                                                                             \
    typedef SET_VIEW_TYPE<IndexType> type;                                    \
};                                                                            \
                                                                              \
} // namespace support

/*///////////////////////////////////////////////////////////////////////////*/


#endif // BOOST_BIMAP_COLLECTION_DETAIL_VIEW_BINDER_HPP

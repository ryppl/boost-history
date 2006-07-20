// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file collection/support/compute_view_type.hpp
/// \brief Compute map and set view metafunctions declaration

#ifndef BOOST_BIMAP_COLLECTION_SUPPORT_COMPUTE_MAP_VIEW_TYPE_HPP
#define BOOST_BIMAP_COLLECTION_SUPPORT_COMPUTE_MAP_VIEW_TYPE_HPP

#include <boost/bimap/detail/safe_enabler.hpp>

namespace boost {
namespace bimap {
namespace collection {
namespace support {

template< class Tag, class BimapCore, class Enable = void /*detail::safe_enabler*/ >
struct compute_map_view_type;

template< class SetKeyType, class IndexType, class Enable = void /*detail::safe_enabler*/ >
struct compute_set_view_type;


/** \struct compute_map_view_type

\brief Computes the map view type associated to a side of a Bimap.

\code
template< class Tag, class BimapCore >
struct compute_map_view_type
{
    typedef {MapViewType} type;
};
\endcode

                                                                                        **/


/** \struct compute_set_view_type

\brief Computes the set view type associated to a set_type_of specification and an index type.

\code
template< class Tag, class BimapCore >
struct compute_set_view_type
{
    typedef {SetViewType} type;
};
\endcode

                                                                                        **/


} // namespace support
} // namespace collection
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_COLLECTION_SUPPORT_COMPUTE_MAP_VIEW_HPP

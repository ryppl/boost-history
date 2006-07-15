// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file relation/support/pair_type_by.hpp
/// \brief pair_type_by<tag,relation> metafunction

#ifndef BOOST_BIMAP_RELATION_SUPPORT_PAIR_TYPE_BY_HPP
#define BOOST_BIMAP_RELATION_SUPPORT_PAIR_TYPE_BY_HPP

#include <boost/bimap/relation/detail/metadata_access_builder.hpp>

/** \struct boost::bimap::relation::support::pair_type_by

\brief Metafunction to obtain the view type indexed by one of the sides.

\code

template< class Tag, class Relation >
struct pair_type_by
{
        typedef {compatible with std::pair} type;
};

\endcode

See also member_at, pair_by().
\ingroup relation_group
                                                                    **/

namespace boost {
namespace bimap {
namespace relation {
namespace support {

// Implementation of pair type by metafunction

BOOST_BIMAP_SYMMETRIC_METADATA_ACCESS_BUILDER
(
    pair_type_by,
    left_pair,
    right_pair
);

// Implementation of pair reference type by metafunction

BOOST_BIMAP_SYMMETRIC_METADATA_ACCESS_BUILDER
(
    pair_reference_type_by,
    left_pair_reference,
    right_pair_reference
);

// Implementation of const pair reference type by metafunction

BOOST_BIMAP_SYMMETRIC_METADATA_ACCESS_BUILDER
(
    const_pair_reference_type_by,
    const_left_pair_reference,
    const_right_pair_reference
);


} // namespace support
} // namespace relation
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_RELATION_SUPPORT_PAIR_BY_TYPE_HPP


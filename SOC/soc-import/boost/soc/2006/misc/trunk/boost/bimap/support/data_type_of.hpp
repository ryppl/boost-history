// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file support/data_type_of.hpp
/// \brief Metafunction to access the data types of a bimap

#ifndef BOOST_BIMAP_SUPPORT_DATA_TYPE_OF_HPP
#define BOOST_BIMAP_SUPPORT_DATA_TYPE_OF_HPP

#include <boost/bimap/relation/detail/metadata_access_builder.hpp>

/** \struct boost::bimap::support::data_type_of

\brief Metafunction to obtain the data type of one of the sides in a bimap

The tag parameter can be either a user defined tag or \c member_at::{side}.
This is the actual data type stored in the bimap.
\code

template< class Tag, class Bimap >
struct data_type_of
{
    typedef typename Bimap::{side}_data_type type;
};

\endcode

The following holds:

\code

BOOST_STATIC_ASSERT
(
    is_same< data_type_of< member_at::left, bimap<A,B> >::type, A >::value
);

\endcode

See also member_at.
\ingroup bimap_group
                                                                    **/

namespace boost {
namespace bimap {
namespace support {

// Implementation of data type of metafunction

BOOST_BIMAP_SYMMETRIC_METADATA_ACCESS_BUILDER
(
    data_type_of,
    left_data_type,
    right_data_type
);

} // namespace support
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_SUPPORT_DATA_TYPE_OF_HPP


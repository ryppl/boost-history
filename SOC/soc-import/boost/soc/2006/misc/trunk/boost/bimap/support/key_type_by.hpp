// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file support/key_type_of.hpp
/// \brief Metafunction to access the set types of a bimap

#ifndef BOOST_BIMAP_SUPPORT_KEY_TYPE_OF_HPP
#define BOOST_BIMAP_SUPPORT_KEY_TYPE_OF_HPP

#include <boost/bimap/relation/detail/metadata_access_builder.hpp>

/** \struct boost::bimap::support::key_type_of

\brief Metafunction to obtain the key type of one of the sides in a bimap

The tag parameter can be either a user defined tag or \c member_at::{side}.
The returned type is one of the {SetType}_of definition classes.

\code

template< class Tag, class Bimap >
struct value_type_by
{
    typedef typename Bimap::{side}_key_type type;
};

\endcode

See also member_at.
\ingroup bimap_group
                                                                    **/


namespace boost {
namespace bimap {
namespace support {

// Implementation of key type type of metafunction

BOOST_BIMAP_SYMMETRIC_METADATA_ACCESS_BUILDER
(
    key_type_of,
    left_key_type,
    right_key_type
);


} // namespace support
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_SUPPORT_KEY_TYPE_OFF


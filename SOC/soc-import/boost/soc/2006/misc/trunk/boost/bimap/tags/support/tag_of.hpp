// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file tags/support/tag_of.hpp
/// \brief Safe way to acces the tag of a type

#ifndef BOOST_BIMAP_TAGS_SUPPORT_TAG_OF_HPP
#define BOOST_BIMAP_TAGS_SUPPORT_TAG_OF_HPP

#include <boost/bimap/tags/tagged.hpp>
#include <boost/bimap/detail/debug/static_error.hpp>

/** \struct boost::bimap::tags::support::tag_of
\brief Metafunction to obtain the tag of a type.

\code
template< class TaggedType >
struct tag_of
{
    typedef {Tag} type;
};
\endcode

If the type is not tagged you will get a compile timer error with the following message:

\verbatim
USING_TAG_OF_WITH_AN_UNTAGGED_TYPE, TaggedType
\endverbatim

See also tagged, value_type_of.
                                                                                **/

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

namespace boost {
namespace bimap {
namespace tags {
namespace support {


// tag_of metafunction

template< class Type >
struct tag_of
{
    BOOST_BIMAP_STATIC_ERROR( USING_TAG_OF_WITH_AN_UNTAGGED_TYPE, (Type) );
};

template< class Type, class Tag >
struct tag_of< tagged< Type, Tag > >
{
    typedef Tag type;
};


} // namespace support
} // namespace tags
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#endif // BOOST_BIMAP_TAGS_SUPPORT_TAG_OF_HPP


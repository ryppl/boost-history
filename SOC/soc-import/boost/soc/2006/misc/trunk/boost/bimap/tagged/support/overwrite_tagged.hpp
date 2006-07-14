// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.

/// \file tagged/support/overwrite_tagged.hpp
/// \brief Hard tagging

#ifndef BOOST_BIMAP_TAGGED_SUPPORT_OVERWRITE_TAGGED_HPP
#define BOOST_BIMAP_TAGGED_SUPPORT_OVERWRITE_TAGGED_HPP

#include <boost/bimap/tagged/tagged.hpp>

/** \struct boost::bimap::tagged::support::overwrite_tagged
\brief Hard tagging metafunction

\code
template< class Type, class Tag >
struct overwrite_tagged
{
    typedef {TaggedType} type;
};
\endcode

If the type is not tagged, this metafunction returns a tagged type with the
passed tag. If it is tagged it returns a new tagged type with the tag replaced
by the one passed as a parameter.

See also tagged, default_tagged.
                                                                                **/

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

namespace boost {
namespace bimap {
namespace tagged {
namespace support {


// Change the tag

template< class Type, class NewTag >
struct overwrite_tagged
{
    typedef tagged<Type,NewTag> type;
};

template< class Type, class OldTag, class NewTag >
struct overwrite_tagged< tagged< Type, OldTag >, NewTag >
{
    typedef tagged<Type,NewTag> type;
};


} // namespace support
} // namespace tagged
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#endif // BOOST_BIMAP_TAGGED_SUPPORT_OVERWRITE_TAGGED_HPP



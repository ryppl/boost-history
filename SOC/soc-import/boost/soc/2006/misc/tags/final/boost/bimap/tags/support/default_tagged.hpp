// Boost.Bimap
//
// Copyright (c) 2006 Matias Capeletto
//
// This code may be used under either of the following two licences:
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE. OF SUCH DAMAGE.
//
// Or:
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

/// \file tags/support/default_tagged.hpp
/// \brief Weak tagging

#ifndef BOOST_BIMAP_TAGS_SUPPORT_DEFAULT_TAGGED_HPP
#define BOOST_BIMAP_TAGS_SUPPORT_DEFAULT_TAGGED_HPP

#include <boost/bimap/tags/tagged.hpp>

/** \struct boost::bimap::tags::support::default_tagged
\brief Weak tagging metafunction

\code
template< class Type, class Tag >
struct default_tagged
{
    typedef {TaggedType} type;
};
\endcode

If the type is not tagged, this metafunction returns a tagged type with the
default tag. If it is tagged, the returns the type unchanged.

See also tagged, overwrite_tagged.
                                                                                **/

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

namespace boost {
namespace bimap {
namespace tags {
namespace support {


// Default Tagging
// A metafunction that create a tagged type with a default tag value.

template< class Type, class DefaultTag >
struct default_tagged
{
    typedef tagged<Type,DefaultTag> type;
};

template< class Type, class OldTag, class NewTag >
struct default_tagged< tagged< Type, OldTag >, NewTag >
{
    typedef tagged<Type,OldTag> type;
};

} // namespace support
} // namespace tags
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#endif // BOOST_BIMAP_TAGS_SUPPORT_DEFAULT_TAGGED_HPP




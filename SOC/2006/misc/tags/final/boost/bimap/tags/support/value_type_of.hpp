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

/// \file tags/support/value_type_of.hpp
/// \brief Consistent way to access the value type of a tagged or untagged type.

#ifndef BOOST_BIMAP_TAGS_SUPPORT_VALUE_TYPE_OF_HPP
#define BOOST_BIMAP_TAGS_SUPPORT_VALUE_TYPE_OF_HPP

#include <boost/bimap/tags/tagged.hpp>

/** \struct boost::bimap::tags::support::value_type_of
\brief Metafunction to work with tagged and untagged type uniformly

\code
template< class Type >
struct value_type_of
{
    typedef {UntaggedType} type;
};
\endcode

If the type is tagged this metafunction returns Type::value_type, and if it is not
tagged it return the same type. This allows to work consistenly with tagged and
untagged types.

See also tagged, tag_of.
                                                                                **/

#ifndef BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES


namespace boost {
namespace bimap {
namespace tags {
namespace support {


// value_type_of metafunction

template< class Type >
struct value_type_of
{
    typedef Type type;
};

template< class Type, class Tag >
struct value_type_of< tagged< Type, Tag > >
{
    typedef Type type;
};


} // namespace support
} // namespace tags
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_DOXYGEN_WILL_NOT_PROCESS_THE_FOLLOWING_LINES

#endif // BOOST_BIMAP_TAGS_SUPPORT_VALUE_TYPE_OF_HPP



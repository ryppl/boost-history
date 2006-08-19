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

/// \file relation/member_at.hpp
/// \brief Defines the tags for the member_at::side idiom

#ifndef BOOST_BIMAP_RELATION_MEMBER_AT_HPP
#define BOOST_BIMAP_RELATION_MEMBER_AT_HPP

namespace boost {
namespace bimap {
namespace relation {

/// \brief member_at::side idiom to access relation values and types using metaprogramming. 
/**

This tags are used to specify which member you want to acces when using a metafunction over
a symmetrical type. The idea is to be able to write code like:

\code
result_of::get<member_at::left,relation>::type data = get<member_at::left>(rel);
\endcode

The relation class supports this idiom even when the elements are tagged. This is useful
because a user can decide to start tagging in any moment of the development.

See also member_with_tag, is_tag_of_member_at_left, is_tag_of_member_at_right, get
value_type_of, pair_by, pair_type_by.

\ingroup relation_group
                                                                                        **/
namespace member_at {

    /// \brief Member at left tag
    /**
    See also member_at, rigth.
                                            **/

    struct left  {};

    /// \brief Member at right tag
    /**
    See also member_at, left.
                                            **/

    struct right {};
};

} // namespace relation
} // namespace bimap
} // namespace boost

#endif // BOOST_BIMAP_RELATION_MEMBER_AT_HPP



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

/// \file relation/detail/totally_ordered_pair.hpp
/// \brief Macro to help build totally ordered pairs.

#ifndef BOOST_BIMAP_RELATION_DETAIL_TOTALLY_ORDERED_PAIR
#define BOOST_BIMAP_RELATION_DETAIL_TOTALLY_ORDERED_PAIR

/****************************************************************************/

#define BOOST_BIMAP_TOTALLY_ORDERED_PAIR_IMPLEMENTATION(                     \
        A_FIRST, A_SECOND,                                                   \
        B_TYPE,                                                              \
        B_FIRST, B_SECOND                                                    \
    )                                                                        \
                                                                             \
    bool operator==(const B_TYPE & b) const                                  \
    {                                                                        \
        return ( ( A_FIRST  == b.B_FIRST  ) &&                               \
                 ( A_SECOND == b.B_SECOND ) );                               \
    }                                                                        \
                                                                             \
    bool operator!=(const B_TYPE & b) const                                  \
    {                                                                        \
        return ( ( A_FIRST  != b.B_FIRST  ) ||                               \
                 ( A_SECOND != b.B_SECOND ) );                               \
    }                                                                        \
                                                                             \
    bool operator< (const B_TYPE & b) const                                  \
    {                                                                        \
        return ( ( A_FIRST  <  b.B_FIRST  ) &&                               \
                 ( A_SECOND <  b.B_SECOND ) );                               \
    }                                                                        \
                                                                             \
    bool operator<=(const B_TYPE & b) const                                  \
    {                                                                        \
        return ( *this < b || *this == b );                                  \
    }                                                                        \
                                                                             \
    bool operator> (const B_TYPE & b) const                                  \
    {                                                                        \
        return ! (*this <= b);                                               \
    }                                                                        \
                                                                             \
    bool operator>=(const B_TYPE & b) const                                  \
    {                                                                        \
        return ( *this > b || *this == b );                                  \
    }

/****************************************************************************/

#endif // BOOST_BIMAP_RELATION_DETAIL_TOTALLY_ORDERED_PAIR


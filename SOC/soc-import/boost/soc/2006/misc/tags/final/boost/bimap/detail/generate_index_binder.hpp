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

/// \file detail/generate_index_binder.hpp
/// \brief Define macros to help building the set type of definitions


#ifndef BOOST_BIMAP_DETAIL_GENERATE_INDEX_BINDER_HPP
#define BOOST_BIMAP_DETAIL_GENERATE_INDEX_BINDER_HPP

#include <boost/multi_index/tag.hpp>


/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_GENERATE_INDEX_BINDER_0CP(                                \
                                                                              \
    MULTI_INDEX_TYPE                                                          \
                                                                              \
)                                                                             \
                                                                              \
template< class KeyExtractor, class Tag >                                     \
struct index_bind                                                             \
{                                                                             \
    typedef MULTI_INDEX_TYPE                                                  \
    <                                                                         \
        multi_index::tag< Tag >,                                              \
        KeyExtractor                                                          \
                                                                              \
    > type;                                                                   \
};                                                                            \
                                                                              \

/*///////////////////////////////////////////////////////////////////////////*/



/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_GENERATE_INDEX_BINDER_1CP(                                \
                                                                              \
    MULTI_INDEX_TYPE,                                                         \
    CONFIG_PARAMETER                                                          \
                                                                              \
)                                                                             \
                                                                              \
template< class KeyExtractor, class Tag >                                     \
struct index_bind                                                             \
{                                                                             \
    typedef MULTI_INDEX_TYPE                                                  \
    <                                                                         \
        multi_index::tag< Tag >,                                              \
        KeyExtractor,                                                         \
        CONFIG_PARAMETER                                                      \
                                                                              \
    > type;                                                                   \
};                                                                            \

/*///////////////////////////////////////////////////////////////////////////*/




/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_GENERATE_INDEX_BINDER_2CP(                                \
                                                                              \
    MULTI_INDEX_TYPE,                                                         \
    CONFIG_PARAMETER_1,                                                       \
    CONFIG_PARAMETER_2                                                        \
)                                                                             \
                                                                              \
template< class KeyExtractor, class Tag >                                     \
struct index_bind                                                             \
{                                                                             \
    typedef MULTI_INDEX_TYPE                                                  \
    <                                                                         \
        multi_index::tag< Tag >,                                              \
        KeyExtractor,                                                         \
        CONFIG_PARAMETER_1,                                                   \
        CONFIG_PARAMETER_2                                                    \
                                                                              \
    > type;                                                                   \
                                                                              \
};                                                                            \

/*///////////////////////////////////////////////////////////////////////////*/


// This is a special registration to allow sequenced and random access indices
// to play along smoothly with the other index types.

/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_GENERATE_INDEX_BINDER_0CP_NO_EXTRACTOR(                   \
                                                                              \
    MULTI_INDEX_TYPE                                                          \
                                                                              \
)                                                                             \
                                                                              \
template< class KeyExtractor, class Tag >                                     \
struct index_bind                                                             \
{                                                                             \
    typedef MULTI_INDEX_TYPE< multi_index::tag< Tag > > type;                 \
};                                                                            \

/*///////////////////////////////////////////////////////////////////////////*/

// This is yet another special registration to allow unconstrained sets
// to play along smoothly with the other index types.

/*///////////////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_GENERATE_INDEX_BINDER_FAKE                                \
                                                                              \
template< class KeyExtractor, class Tag >                                     \
struct index_bind                                                             \
{                                                                             \
    typedef void type;                                                        \
};                                                                            \

/*///////////////////////////////////////////////////////////////////////////*/

#endif // BOOST_BIMAP_DETAIL_GENERATE_INDEX_BINDER_HPP

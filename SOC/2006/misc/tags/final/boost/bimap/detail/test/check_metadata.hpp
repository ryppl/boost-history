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

#ifndef BOOST_BIMAP_DETAIL_CHECK_METADATA_HPP
#define BOOST_BIMAP_DETAIL_CHECK_METADATA_HPP

#include <boost/mpl/assert.hpp>
#include <boost/type_traits/is_same.hpp>
#include <boost/preprocessor/cat.hpp>


// Easier way to call BOOST_MPL_ASSERT_MSG in class scope
/*//////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_MPL_ASSERT_MSG_ACS(p1,p2,p3)                     \
                                                                     \
    struct p2 {};                                                    \
    BOOST_MPL_ASSERT_MSG(p1,p2,p3);                                  \

/*//////////////////////////////////////////////////////////////////*/


// Build a descriptive name.
/*//////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_WRONG_METADATA_MESSAGE(                          \
                                                                     \
        P_CLASS,                                                     \
        P_NAME,                                                      \
        P_CORRECT_TYPE                                               \
                                                                     \
    )                                                                \
                                                                     \
    BOOST_PP_CAT                                                     \
    (                                                                \
        WRONG_METADATA__,                                            \
        BOOST_PP_CAT                                                 \
        (                                                            \
            P_CLASS,                                                 \
            BOOST_PP_CAT                                             \
            (                                                        \
                __AT__,                                              \
                BOOST_PP_CAT                                         \
                (                                                    \
                    P_NAME,                                          \
                    BOOST_PP_CAT                                     \
                    (                                                \
                        __IS_DIFERENT_TO__,                          \
                        P_CORRECT_TYPE                               \
                    )                                                \
                )                                                    \
            )                                                        \
        )                                                            \
    )

/*//////////////////////////////////////////////////////////////////*/


// Check if the metadata have the correct type, and if not inform
// it with a useful compile time message.
/*//////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_CHECK_METADATA(                                  \
                                                                     \
        P_CLASS,                                                     \
        P_NAME,                                                      \
        P_CORRECT_TYPE                                               \
                                                                     \
    )                                                                \
                                                                     \
    BOOST_BIMAP_MPL_ASSERT_MSG_ACS                                   \
    (                                                                \
        (                                                            \
            ::boost::is_same                                         \
            <                                                        \
                P_CLASS::P_NAME,                                     \
                P_CORRECT_TYPE                                       \
                                                                     \
            >::value                                                 \
        ),                                                           \
        BOOST_BIMAP_WRONG_METADATA_MESSAGE                           \
        (                                                            \
            P_CLASS,                                                 \
            P_NAME,                                                  \
            P_CORRECT_TYPE                                           \
        ),                                                           \
        (P_CLASS::P_NAME,P_CORRECT_TYPE)                             \
    )

/*//////////////////////////////////////////////////////////////////*/


// Just for autodocumment the test code
/*//////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_TEST_STATIC_FUNCTION(NAME)                       \
    namespace NAME

/*//////////////////////////////////////////////////////////////////*/


// Just for autodocument the test code
/*//////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_CALL_TEST_STATIC_FUNCTION(NAME)

/*//////////////////////////////////////////////////////////////////*/



#endif // BOOST_BIMAP_DETAIL_CHECK_METADATA_HPP


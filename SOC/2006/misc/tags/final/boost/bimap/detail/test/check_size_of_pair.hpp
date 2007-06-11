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


#ifndef BOOST_BIMAP_DETAIL_TEST_CHECK_SIZE_OF_PAIR_HPP
#define BOOST_BIMAP_DETAIL_TEST_CHECK_SIZE_OF_PAIR_HPP

// Boost.MPL
#include <boost/mpl/bool.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/mpl/min_max.hpp>
#include <boost/mpl/arithmetic.hpp>
#include <boost/mpl/times.hpp>
#include <boost/mpl/assert.hpp>

#include <boost/bimap/detail/test/check_metadata.hpp>

#include <boost/preprocessor/cat.hpp>

namespace boost {
namespace bimap {
namespace detail {
namespace test {

template< class PairType, class Data_A, class Data_B >
struct is_size_of_pair_correct
{
    typedef typename mpl::max
    <
        mpl::int_<sizeof(Data_A)>,
        mpl::int_<sizeof(Data_B)>

    >::type maximum_data_size;

    typedef typename mpl::times
    <
        maximum_data_size,
        mpl::int_<2>

    >::type maximum_allowed_size;

    typedef typename ::boost::mpl::less_equal
    <
        ::boost::mpl::int_<sizeof(PairType)>,
        maximum_allowed_size

    >::type type;
};


// Build a descriptive name.
/*//////////////////////////////////////////////////////////////////*/

#define BOOST_BIMAP_WRONG_SIZE_OF_PAIR_MESSAGE(                      \
                                                                     \
        P_PAIR,                                                      \
        P_DATA_A,                                                    \
        P_DATA_B                                                     \
                                                                     \
    )                                                                \
                                                                     \
    BOOST_PP_CAT                                                     \
    (                                                                \
        WRONG_SIZE_OF_PAIR__,                                        \
        BOOST_PP_CAT                                                 \
        (                                                            \
            P_PAIR,                                                  \
            BOOST_PP_CAT                                             \
            (                                                        \
                __WITH__,                                            \
                BOOST_PP_CAT                                         \
                (                                                    \
                    P_DATA_A,                                        \
                    BOOST_PP_CAT                                     \
                    (                                                \
                        __AND__,                                     \
                        P_DATA_B                                     \
                    )                                                \
                )                                                    \
            )                                                        \
        )                                                            \
    )

/*//////////////////////////////////////////////////////////////////*/



template< class PairType, class Data_A, class Data_B >
class check_size_of_pair
{
    typedef typename is_size_of_pair_correct< PairType, Data_A, Data_B >::type test_passed;

    BOOST_BIMAP_MPL_ASSERT_MSG_ACS
    (
        test_passed::value,
        BOOST_BIMAP_WRONG_SIZE_OF_PAIR_MESSAGE(PairType,Data_A,Data_B),
        (PairType,Data_A,Data_B)
    );
};


} // namespace test
} // namespace detail
} // namespace bimap
} // namespace boost


#endif // BOOST_BIMAP_DETAIL_TEST_CHECK_SIZE_OF_PAIR_HPP

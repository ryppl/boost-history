// Boost.bimap
//
// Copyright 2006 Matias Capeletto
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/bimap for library home page.


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

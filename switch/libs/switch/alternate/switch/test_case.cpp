// test_case.hpp
//
// Copyright (c) 2008
// Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_AUTO_TEST_MAIN

#include "switch.hpp"
#include "case.hpp"

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/vector_c.hpp>

#include <boost/test/unit_test.hpp>

namespace {
    struct f {
        typedef int result_type;
        template<class T>
        int operator()(T) const {
            return(T::value * T::value);
        }
    };
}

BOOST_AUTO_TEST_CASE(test1) {
    using boost::control::case_;
    BOOST_CHECK_EQUAL(boost::control::switch_<int>(2, (case_<boost::mpl::range_c<int, 0, 5> >(f()))), 4);
}

BOOST_AUTO_TEST_CASE(test2) {
    using boost::control::case_;
    BOOST_CHECK_EQUAL(boost::control::switch_<int>(2,
        (case_<boost::mpl::vector_c<int, 0> >(f()),
        case_<boost::mpl::vector_c<int, 1> >(f()),
        case_<boost::mpl::vector_c<int, 2> >(f()),
        case_<boost::mpl::vector_c<int, 3> >(f()),
        case_<boost::mpl::vector_c<int, 4> >(f()),
        case_<boost::mpl::vector_c<int, 5> >(f()))
        ), 4);
}

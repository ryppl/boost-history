// test_switch.cpp
//
// Copyright (c) 2007
// Steven Watanabe
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_AUTO_TEST_MAIN

#include <boost/switch.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/range_c.hpp>
#include <boost/test/auto_unit_test.hpp>

namespace {
    typedef boost::mpl::vector_c<int, 0, 2, 1, 4, 5, 6, 3> values;
    struct f {
        typedef int result_type;
        template<class Index>
        int operator()(Index) {
            return(boost::mpl::at<values, Index>::type::value);
        }
    };
    int default_(int i) {
        return(i);
    }
    typedef boost::mpl::range_c<int, 0, 0> empty_range;
    typedef boost::mpl::vector_c<int, 1, 0, 5> non_sequential_range;
    typedef boost::mpl::range_c<int, 0, 7> test_range;

    struct limit_test {
        typedef int result_type;
        template<class Index>
        int operator()(Index) const {
            return(-Index::value);
        }
    };
    typedef boost::mpl::range_c<int, 0, BOOST_SWITCH_LIMIT> limit_range;
    const int limit_value = BOOST_SWITCH_LIMIT - 1;

    template<class T>
    struct void_return {
        typedef T result_type;
        template<class Index>
        T operator()(Index) const {
            // Do Nothing.
        }
    };

    int test_int;

    struct return_reference {
        typedef int& result_type;
        template<class Index>
        int& operator()(Index) const {
            return(test_int);
        }
    };
}

BOOST_AUTO_TEST_CASE(without_default) {
    BOOST_CHECK_EQUAL((boost::switch_<test_range>(5, f())), 6);
    BOOST_CHECK_EQUAL((boost::switch_<test_range>(1, f())), 2);

    BOOST_CHECK_THROW((boost::switch_<test_range>(-1, f())), boost::bad_switch);
    BOOST_CHECK_THROW((boost::switch_<test_range>(7, f())), boost::bad_switch);
}

BOOST_AUTO_TEST_CASE(with_default) {
    BOOST_CHECK_EQUAL((boost::switch_<test_range>(5, f(), &default_)), 6);
    BOOST_CHECK_EQUAL((boost::switch_<test_range>(1, f(), &default_)), 2);

    BOOST_CHECK_EQUAL((boost::switch_<test_range>(-1, f(), &default_)), -1);
    BOOST_CHECK_EQUAL((boost::switch_<test_range>(7, f(), &default_)), 7);
}

BOOST_AUTO_TEST_CASE(non_sequential_without_default) {
    BOOST_CHECK_EQUAL((boost::switch_<non_sequential_range>(5, f())), 6);
    BOOST_CHECK_EQUAL((boost::switch_<non_sequential_range>(1, f())), 2);

    BOOST_CHECK_THROW((boost::switch_<non_sequential_range>(-1, f())), boost::bad_switch);
    BOOST_CHECK_THROW((boost::switch_<non_sequential_range>(7, f())), boost::bad_switch);
}

BOOST_AUTO_TEST_CASE(non_sequential_with_default) {
    BOOST_CHECK_EQUAL((boost::switch_<non_sequential_range>(5, f(), &default_)), 6);
    BOOST_CHECK_EQUAL((boost::switch_<non_sequential_range>(1, f(), &default_)), 2);

    BOOST_CHECK_EQUAL((boost::switch_<non_sequential_range>(-1, f(), &default_)), -1);
    BOOST_CHECK_EQUAL((boost::switch_<non_sequential_range>(7, f(), &default_)), 7);
}

BOOST_AUTO_TEST_CASE(zero_params_without_default) {
    BOOST_CHECK_THROW((boost::switch_<empty_range>(5, f())), boost::bad_switch);
    BOOST_CHECK_THROW((boost::switch_<empty_range>(1, f())), boost::bad_switch);
    BOOST_CHECK_THROW((boost::switch_<empty_range>(0, f())), boost::bad_switch);

    BOOST_CHECK_THROW((boost::switch_<empty_range>(-1, f())), boost::bad_switch);
    BOOST_CHECK_THROW((boost::switch_<empty_range>(7, f())), boost::bad_switch);
}

BOOST_AUTO_TEST_CASE(zero_params_with_default) {
    BOOST_CHECK_EQUAL((boost::switch_<empty_range>(5, f(), &default_)), 5);
    BOOST_CHECK_EQUAL((boost::switch_<empty_range>(1, f(), &default_)), 1);
    BOOST_CHECK_EQUAL((boost::switch_<empty_range>(0, f(), &default_)), 0);

    BOOST_CHECK_EQUAL((boost::switch_<empty_range>(-1, f(), &default_)), -1);
    BOOST_CHECK_EQUAL((boost::switch_<empty_range>(7, f(), &default_)), 7);
}

BOOST_AUTO_TEST_CASE(limit_without_default) {
    BOOST_CHECK_EQUAL((boost::switch_<limit_range>(limit_value, limit_test())), -limit_value);
}

BOOST_AUTO_TEST_CASE(limit_with_default) {
    BOOST_CHECK_EQUAL((boost::switch_<limit_range>(limit_value, limit_test(), &default_)), -limit_value);
}

BOOST_AUTO_TEST_CASE(test_void) {
    boost::switch_<test_range>(0, void_return<void>());
    boost::switch_<test_range>(0, void_return<const void>());
    boost::switch_<test_range>(0, void_return<volatile void>());
    boost::switch_<test_range>(0, void_return<const volatile void>());
}

BOOST_AUTO_TEST_CASE(test_reference) {
    boost::switch_<test_range>(0, return_reference());
}

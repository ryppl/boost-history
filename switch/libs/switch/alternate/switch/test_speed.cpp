// test_speed.hpp
//
// Copyright (c) 2008
// Steven Watanabe
//
// Distriuted under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost/org/LICENSE_1_0.txt)

#define BOOST_SWITCH_LIMIT 250

#pragma warning(disable:4307)

#include "if_else.hpp"
#include "binary_search.hpp"
#include "switch.hpp"
#include "map.hpp"
#include "sorted_array.hpp"
#include "case.hpp"

#include <iostream>

#include <boost/mpl/range_c.hpp>
#include <boost/mpl/transform_view.hpp>
#include <boost/timer.hpp>

#include <boost/preprocessor/repetition/enum.hpp>

struct test_function {
    template<class N>
    int operator()(N) const {
        return(N::value * N::value);
    }
};

struct default_ {
    int operator()(int) {
        return(-1);
    }
};

int x;

#define CASE(z, n, data) boost::case_c<n * n>(data)

struct square {
    template<class T>
    struct apply {
        typedef boost::mpl::int_<T::value * T::value> type;
    };
};

template<class F>
void do_test(const char* name, F f) {

    std::cout << name << ":\n";

    {
        boost::timer timer;

        for(int i = 0; i < 100000000; ++i) {
            x = f(i * 13 % BOOST_SWITCH_LIMIT, boost::case_<boost::mpl::range_c<int, 0, BOOST_SWITCH_LIMIT> >(test_function()), default_());
        }

        std::cout << "consecutive: " << timer.elapsed() << std::endl;
    }

    {
        boost::timer timer;

        for(int i = 0; i < 100000000; ++i) {
            x = f((i * 13 % BOOST_SWITCH_LIMIT) * (i * 13 % BOOST_SWITCH_LIMIT), boost::case_<boost::mpl::transform_view<boost::mpl::range_c<int, 0, BOOST_SWITCH_LIMIT>, square> >(test_function()), default_());
            //x = f((i % BOOST_SWITCH_LIMIT) * (i % BOOST_SWITCH_LIMIT), (BOOST_PP_ENUM(BOOST_SWITCH_LIMIT, CASE, test_function())), default_());
        }

        std::cout << "spread out: " << timer.elapsed() << std::endl;

    }

}

struct test_switch {
    template<class I, class Case, class Default>
    int operator()(I i, const Case& case_, const Default& default_) {
        return(boost::switch_<int>(i, case_, default_));
    }
};

struct test_if_else {
    template<class I, class Case, class Default>
    int operator()(I i, const Case& case_, const Default& default_) {
        return(boost::if_else_switch::switch_<int>(i, case_, default_));
    }
};

struct test_binary_search {
    template<class I, class Case, class Default>
    int operator()(I i, const Case& case_, const Default& default_) {
        return(boost::binary_search_switch::switch_<int>(i, case_, default_));
    }
};

struct test_map {
    template<class I, class Case, class Default>
    int operator()(I i, const Case& case_, const Default& default_) {
        return(boost::map_switch::switch_<int>(i, case_, default_));
    }
};

struct test_sorted_array {
    template<class I, class Case, class Default>
    int operator()(I i, const Case& case_, const Default& default_) {
        return(boost::sorted_array_switch::switch_<int>(i, case_, default_));
    }
};

int main() {
    do_test("switch", test_switch());
    do_test("if/else", test_if_else());
    do_test("binary_search", test_binary_search());
    do_test("std::map", test_map());
    do_test("sorted_array", test_sorted_array());
}

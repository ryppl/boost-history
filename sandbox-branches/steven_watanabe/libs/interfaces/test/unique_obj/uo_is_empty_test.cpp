// (C) Copyright Jonathan Turkanis 2004
// Copyright (c) 2002, 2003 Peter Dimov
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Portions addapted from <libs/smart_ptr/shared_ptr_test.cpp>

#include <boost/interfaces/unique_obj.hpp>
#include "../detail/counter.hpp"
#include "../detail/hierarchies.hpp"
#include "../detail/unit_test_framework.hpp"

using boost::unit_test_framework::test_suite;  
        
void is_empty_test()
{
    using namespace boost;
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    unique_obj<ifirst> obj1;
    BOOST_CHECK(obj1 ? false : true);
    BOOST_CHECK(!obj1);
    BOOST_CHECK(get(obj1) == 0);

    unique_obj<ifirst> obj2(new cfirst<1>);
    BOOST_CHECK(obj2 ? true : false);
    BOOST_CHECK(!!obj2);
    BOOST_CHECK(get(obj2) != 0);

    unique_obj<ifirst> obj3(move(obj2));
    BOOST_CHECK(obj2 ? false : true);
    BOOST_CHECK(!obj2);
    BOOST_CHECK(get(obj2) == 0);
    BOOST_CHECK(obj3 ? true : false);
    BOOST_CHECK(!!obj3);
    BOOST_CHECK(get(obj3) != 0);

    reset(obj3);
    BOOST_CHECK(obj3 ? false : true);
    BOOST_CHECK(!obj3);
    BOOST_CHECK(get(obj3) == 0);

    reset(obj3, new cfirst<1>);
    BOOST_CHECK(obj3 ? true : false);
    BOOST_CHECK(!!obj3);
    BOOST_CHECK(get(obj3) != 0);

    unique_obj<ifirst> obj4;
    swap(obj4, obj3);
    BOOST_CHECK(obj3 ? false : true);
    BOOST_CHECK(!obj3);
    BOOST_CHECK(get(obj3) == 0);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("unique_obj is empty test");
    test->add(BOOST_TEST_CASE(&is_empty_test));
    return test;
}

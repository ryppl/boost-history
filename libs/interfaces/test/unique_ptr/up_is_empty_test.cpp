// (C) Copyright Jonathan Turkanis 2004
// Copyright (c) 2002, 2003 Peter Dimov
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Portions addapted from <libs/smart_ptr/shared_ptr_test.cpp>

#include <boost/interfaces/unique_ptr.hpp>
#include "../detail/counter.hpp"
#include "../detail/hierarchies.hpp"
#include "../detail/unit_test_framework.hpp"

using boost::unit_test_framework::test_suite;  
        
void is_empty_test()
{
    using namespace boost;
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;

    unique_ptr<ifirst> ptr1;
    BOOST_CHECK(ptr1 ? false : true);
    BOOST_CHECK(!ptr1);
    BOOST_CHECK(ptr1.get() == 0);

    unique_ptr<ifirst> ptr2(new cfirst<1>);
    BOOST_CHECK(ptr2 ? true : false);
    BOOST_CHECK(!!ptr2);
    BOOST_CHECK(ptr2.get() != 0);

    unique_ptr<ifirst> ptr3(move(ptr2));
    BOOST_CHECK(ptr2 ? false : true);
    BOOST_CHECK(!ptr2);
    BOOST_CHECK(ptr2.get() == 0);
    BOOST_CHECK(ptr3 ? true : false);
    BOOST_CHECK(!!ptr3);
    BOOST_CHECK(ptr3.get() != 0);

    ptr3.reset();
    BOOST_CHECK(ptr3 ? false : true);
    BOOST_CHECK(!ptr3);
    BOOST_CHECK(ptr3.get() == 0);

    ptr3.reset(new cfirst<1>);
    BOOST_CHECK(ptr3 ? true : false);
    BOOST_CHECK(!!ptr3);
    BOOST_CHECK(ptr3.get() != 0);

    unique_ptr<ifirst>().swap(ptr3);
    BOOST_CHECK(ptr3 ? false : true);
    BOOST_CHECK(!ptr3);
    BOOST_CHECK(ptr3.get() == 0);
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("unique_ptr is empty test");
    test->add(BOOST_TEST_CASE(&is_empty_test));
    return test;
}

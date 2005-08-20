// (C) Copyright Jonathan Turkanis 2004
// Copyright (c) 2002, 2003 Peter Dimov
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Portions addapted from <libs/smart_ptr/shared_ptr_test.cpp>

#include <boost/interfaces/shared_ptr.hpp>
#include "../detail/counter.hpp"
#include "../detail/hierarchies.hpp"
#include "../detail/unit_test_framework.hpp"

using boost::unit_test_framework::test_suite;  
        
void use_count_test()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;
    
    shared_ptr<ifirst> ptr1;
    BOOST_CHECK(ptr1.use_count() == 0);
    BOOST_CHECK(!ptr1.unique());
    {
        ptr1.reset(new cfirst<1>);
        BOOST_CHECK(ptr1.use_count() == 1);
        BOOST_CHECK(ptr1.unique());
        {
            shared_ptr<ifirst> ptr2(ptr1);
            BOOST_CHECK(ptr1.use_count() == 2);
            BOOST_CHECK(!ptr1.unique());
            {
                shared_ptr<ifirst> ptr3(ptr2);
                BOOST_CHECK(ptr1.use_count() == 3);
                BOOST_CHECK(!ptr1.unique());
                {
                    shared_ptr<ifirst> ptr4(ptr3);
                    BOOST_CHECK(ptr1.use_count() == 4);
                    BOOST_CHECK(!ptr1.unique());
                    {
                        shared_ptr<ifirst> ptr5(ptr4);
                        BOOST_CHECK(ptr1.use_count() == 5);
                        BOOST_CHECK(!ptr1.unique());
                   }
                    BOOST_CHECK(ptr1.use_count() == 4);
                    BOOST_CHECK(!ptr1.unique());
                }
                BOOST_CHECK(ptr1.use_count() == 3);
                BOOST_CHECK(!ptr1.unique());
            }
            BOOST_CHECK(ptr1.use_count() == 2);
            BOOST_CHECK(!ptr1.unique());
        }
        BOOST_CHECK(ptr1.use_count() == 1);
        BOOST_CHECK(ptr1.unique());
    }
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("shared_ptr use_count test");
    test->add(BOOST_TEST_CASE(&use_count_test));
    return test;
}

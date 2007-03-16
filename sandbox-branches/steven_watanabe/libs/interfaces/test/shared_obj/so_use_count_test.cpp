// (C) Copyright Jonathan Turkanis 2004
// Copyright (c) 2002, 2003 Peter Dimov
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt.)

// Portions addapted from <libs/smart_ptr/shared_ptr_test.cpp>

#include <boost/interfaces/shared_obj.hpp>
#include "../detail/counter.hpp"
#include "../detail/hierarchies.hpp"
#include "../detail/unit_test_framework.hpp"

using boost::unit_test_framework::test_suite;  
        
void use_count_test()
{
    using namespace boost::interfaces;
    using namespace boost::interfaces::test;
    
    shared_obj<ifirst> obj1;
    BOOST_CHECK(use_count(obj1) == 0);
    BOOST_CHECK(!unique(obj1));
    {
        reset(obj1, new cfirst<1>);
        BOOST_CHECK(use_count(obj1) == 1);
        BOOST_CHECK(unique(obj1));
        {
            shared_obj<ifirst> obj2(obj1);
            BOOST_CHECK(use_count(obj1) == 2);
            BOOST_CHECK(!unique(obj1));
            {
                shared_obj<ifirst> obj3(obj2);
                BOOST_CHECK(use_count(obj1) == 3);
                BOOST_CHECK(!unique(obj1));
                {
                    shared_obj<ifirst> obj4(obj3);
                    BOOST_CHECK(use_count(obj1) == 4);
                    BOOST_CHECK(!unique(obj1));
                    {
                        shared_obj<ifirst> obj5(obj4);
                        BOOST_CHECK(use_count(obj1) == 5);
                        BOOST_CHECK(!unique(obj1));
                   }
                    BOOST_CHECK(use_count(obj1) == 4);
                    BOOST_CHECK(!unique(obj1));
                }
                BOOST_CHECK(use_count(obj1) == 3);
                BOOST_CHECK(!unique(obj1));
            }
            BOOST_CHECK(use_count(obj1) == 2);
            BOOST_CHECK(!unique(obj1));
        }
        BOOST_CHECK(use_count(obj1) == 1);
        BOOST_CHECK(unique(obj1));
    }
}

test_suite* init_unit_test_suite(int, char* []) 
{
    test_suite* test = BOOST_TEST_SUITE("shared_obj use_count test");
    test->add(BOOST_TEST_CASE(&use_count_test));
    return test;
}

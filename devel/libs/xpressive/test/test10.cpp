///////////////////////////////////////////////////////////////////////////////
// test10.cpp
//
//  Copyright 2004 Eric Niebler. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <algorithm>
#include "./test10.hpp"

///////////////////////////////////////////////////////////////////////////////
// test_main
//   read the tests from the input file and execute them
void test_main()
{
    typedef std::string::const_iterator iterator_type;
    boost::iterator_range<xpr_test_case<iterator_type> const *> rng = get_test_cases<iterator_type>();
    std::for_each(rng.begin(), rng.end(), test_runner<iterator_type>());
}

///////////////////////////////////////////////////////////////////////////////
// init_unit_test_suite
//
test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite *test = BOOST_TEST_SUITE("test10");
    test->add(BOOST_TEST_CASE(&test_main));
    return test;
}

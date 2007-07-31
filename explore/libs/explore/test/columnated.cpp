// Boost.Print library

// Copyright Jeffrey Faust 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <string>
#include <sstream>
#include <vector>
#include "../../../boost/explore.hpp"

BOOST_AUTO_TEST_CASE( basic_array_print_test )
{
    BOOST_FAIL("Test not implemented");
}

BOOST_AUTO_TEST_CASE( basic_array_stream_test )
{
    using namespace explore;
    std::stringstream str_out;
    std::vector<int> vi;

    for( int i = 0; i < 9; ++i )
    {
        vi.push_back(i);
    }

    str_out << setrows(3) << start("") << explore::end("") << vi;

    BOOST_CHECK_EQUAL(str_out.str(),
        "0, 1, 2, \n"
        "3, 4, 5, \n"
        "6, 7, 8");
}

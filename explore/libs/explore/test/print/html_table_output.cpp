// Boost.Explore library

// Copyright Danny Havenith 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <string>
#include <sstream>
#include <vector>
#include <boost/explore.hpp>
#include <boost/explore/html_table_format.hpp>

BOOST_AUTO_TEST_CASE( vector_in_c_array_html_print_test )
{
    std::stringstream str_out;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    std::vector<int> arrvi1[] = {vi};
    explore::print(arrvi1, str_out, explore::html_table_format(), explore::default_container_policy());

    BOOST_CHECK_EQUAL(str_out.str(), 
        "<table>\n"
        "<tr><td>1</td><td>2</td><td>3</td></tr>\n"
        "</table>\n");

    str_out.str("");

    std::vector<int> arrvi3[] = {vi,vi,vi};
    explore::print(arrvi3, str_out, explore::html_table_format(), explore::default_container_policy());
    BOOST_CHECK_EQUAL(str_out.str(), 
        "<table>\n"
        "<tr><td>1</td><td>2</td><td>3</td></tr>\n"
        "<tr><td>1</td><td>2</td><td>3</td></tr>\n"
        "<tr><td>1</td><td>2</td><td>3</td></tr>\n"
        "</table>\n");
}

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

std::ostream& html_table_stream_format(std::ostream& ostr)
{
    using namespace explore;
    ostr << start("<table>\n")
         << separator("")
         << explore::end("</table>\n");

    ostr << start("<tr><td>", 1)
         << separator("</td><td>", 1)
         << explore::end("</td></tr>\n", 1);

    return ostr;
}

BOOST_AUTO_TEST_CASE( vector_in_c_array_html_stream_test )
{
    using namespace explore;
    std::stringstream str_out;
    str_out << html_table_stream_format;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    std::vector<int> arrvi1[] = {vi};
    str_out << arrvi1;

    BOOST_CHECK_EQUAL(str_out.str(), 
        "<table>\n"
        "<tr><td>1</td><td>2</td><td>3</td></tr>\n"
        "</table>\n");

    str_out.str("");

    std::vector<int> arrvi3[] = {vi,vi,vi};
    str_out << arrvi3;
    BOOST_CHECK_EQUAL(str_out.str(), 
        "<table>\n"
        "<tr><td>1</td><td>2</td><td>3</td></tr>\n"
        "<tr><td>1</td><td>2</td><td>3</td></tr>\n"
        "<tr><td>1</td><td>2</td><td>3</td></tr>\n"
        "</table>\n");
}


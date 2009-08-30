// Boost.Explore library

// Copyright Danny Havenith 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <boost/explore/manipulators.hpp>
#include <vector>
#include <boost/explore.hpp>
#include "boost_explore_test_tools.hpp"

std::ostream& html_table_stream_format(std::ostream& ostr)
{
    using namespace boost::explore;
    ostr << level(0);
    ostr << start("<table>\n")
         << separator("")
         << boost::explore::end("</table>\n");

    ostr << level(1);
    ostr << start("<tr><td>")
         << separator("</td><td>")
         << boost::explore::end("</td></tr>\n");

    return ostr;
}

std::wostream& html_table_stream_format(std::wostream& ostr)
{
    using namespace boost::explore;
    ostr << level(0);
    ostr << start(L"<table>\n")
         << separator(L"")
         << boost::explore::end(L"</table>\n");

    ostr << level(1);
    ostr << start(L"<tr><td>")
         << separator(L"</td><td>")
         << boost::explore::end(L"</td></tr>\n");

    return ostr;
}

BOOST_AUTO_TEST_CASE_TEMPLATE( vector_in_c_array_html_stream_test, C, test_types )
{
    using namespace boost::explore;
    test_traits<C>::stream_type str_out;
    str_out << html_table_stream_format;

    std::vector<int> vi;
    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    std::vector<int> arrvi1[] = {vi};
    str_out << arrvi1;

    BOOST_CHECK_EQUAL(output(str_out), 
        "<table>\n"
        "<tr><td>1</td><td>2</td><td>3</td></tr>\n"
        "</table>\n");

    reset(str_out);

    std::vector<int> arrvi3[] = {vi,vi,vi};
    str_out << arrvi3;
    BOOST_CHECK_EQUAL(output(str_out), 
        "<table>\n"
        "<tr><td>1</td><td>2</td><td>3</td></tr>\n"
        "<tr><td>1</td><td>2</td><td>3</td></tr>\n"
        "<tr><td>1</td><td>2</td><td>3</td></tr>\n"
        "</table>\n");
}


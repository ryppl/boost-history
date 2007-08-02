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

BOOST_AUTO_TEST_CASE( basic_columnate_print_test )
{
    //BOOST_FAIL("Test not implemented");
}

struct columnated_format
{
    columnated_format(std::size_t rows, std::size_t width) : m_rows(rows), m_width(width) {}
    std::size_t m_rows;
    std::size_t m_width;

    friend std::ostream& operator<<(std::ostream& ostr, const columnated_format& f)
    {
        using namespace explore;
        ostr << setrows(f.m_rows) << setitemwidth(f.m_width)
             << separator("") << start("") << explore::end("");
        return ostr;
    }
};

BOOST_AUTO_TEST_CASE( basic_columnate_stream_test )
{
    using namespace explore;
    std::stringstream str_out;
    std::vector<int> vi;

    for( int i = 0; i < 12; ++i )
    {
        vi.push_back(i);
    }

    str_out << setrows(3) << start("") << explore::end("") << vi;

    BOOST_CHECK_EQUAL(str_out.str(),
        "0, 1, 2, \n"
        "3, 4, 5, \n"
        "6, 7, 8, \n"
        "9, 10, 11");

    str_out.str("");

    str_out << setrows(3) << setitemwidth(5) << start("") << explore::end("") << vi;

    BOOST_CHECK_EQUAL(str_out.str(),
        "    0,     1,     2, \n"
        "    3,     4,     5, \n"
        "    6,     7,     8, \n"
        "    9,    10,    11");

    str_out.str("");

    str_out << columnated_format(3, 5) << vi;

    BOOST_CHECK_EQUAL(str_out.str(),
        "    0    1    2\n"
        "    3    4    5\n"
        "    6    7    8\n"
        "    9   10   11");
}

// Boost.Explore library

// Copyright Jeffrey Faust 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

// For more information, see http://www.boost.org

#define BOOST_TEST_MODULE PrintLib
#include <boost/test/unit_test.hpp>
#include <vector>
#include <boost/explore.hpp>
#include "boost_explore_test_tools.hpp"

template<typename C>
struct columnated_format
{
    columnated_format(std::size_t cols, std::size_t width) : m_cols(cols), m_width(width) {}
    std::size_t m_cols;
    std::size_t m_width;

    friend std::basic_ostream<C>& operator<<(std::basic_ostream<C>& ostr, const columnated_format<C>& f)
    {
        using namespace boost::explore;
        ostr << cols(f.m_cols) << item_width(f.m_width)
             << separator(str_to<C>("")) << start(str_to<C>("")) << boost::explore::end(str_to<C>(""));
        return ostr;
    }
};

BOOST_AUTO_TEST_CASE_TEMPLATE( basic_columnate_stream_test, C, test_types )
{
    using namespace boost::explore;
    typename test_traits<C>::stream_type str_out;
    std::vector<int> vi;

    for( int i = 0; i < 12; ++i )
    {
        vi.push_back(i);
    }

    str_out << cols(3) << start(str_to<C>("")) << boost::explore::end(str_to<C>("")) << vi;

    BOOST_CHECK_EQUAL(output(str_out),
        "0, 1, 2, \n"
        "3, 4, 5, \n"
        "6, 7, 8, \n"
        "9, 10, 11");

    reset(str_out);

    str_out << cols(3) << item_width(5) << start(str_to<C>("")) << boost::explore::end(str_to<C>("")) << vi;

    BOOST_CHECK_EQUAL(output(str_out),
        "    0,     1,     2, \n"
        "    3,     4,     5, \n"
        "    6,     7,     8, \n"
        "    9,    10,    11");

    reset(str_out);

    str_out << columnated_format<C>(3, 5) << vi;

    BOOST_CHECK_EQUAL(output(str_out),
        "    0    1    2\n"
        "    3    4    5\n"
        "    6    7    8\n"
        "    9   10   11");
}

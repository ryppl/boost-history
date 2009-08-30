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

std::ostream& format_2d(std::ostream& ostr)
{
    using namespace boost::explore;

    ostr << level(0);
    ostr << start("") << boost::explore::end("") << separator("\n");

    ostr << level(1);
    ostr << start("|") << boost::explore::end("|") << separator(" ");

    return ostr;
}

std::wostream& format_2d(std::wostream& ostr)
{
    using namespace boost::explore;

    ostr << level(0);
    ostr << start(L"") << boost::explore::end(L"") << separator(L"\n");

    ostr << level(1);
    ostr << start(L"|") << boost::explore::end(L"|") << separator(L" ");

    return ostr;
}

BOOST_AUTO_TEST_CASE_TEMPLATE( two_dimension_stream_test, C, test_types )
{
    test_traits<C>::stream_type str_out;

    std::vector<int> vi;
    std::vector<std::vector<int> > vvi;

    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    vvi.push_back(vi);
    vvi.push_back(vi);
    vvi.push_back(vi);

    str_out << format_2d << vvi;
    BOOST_CHECK_EQUAL(output(str_out),
        "|1 2 3|\n"
        "|1 2 3|\n"
        "|1 2 3|");
}

std::ostream& format_3d(std::ostream& ostr)
{
    using namespace boost::explore;

    ostr << level(0);
    ostr << start("") << boost::explore::end("") << separator("\n\n");

    ostr << level(1);
    ostr << start("") << boost::explore::end("") << separator("\n");

    ostr << level(2);
    ostr << start("|") << boost::explore::end("|") << separator(" ");

    return ostr;
}

std::wostream& format_3d(std::wostream& ostr)
{
    using namespace boost::explore;

    ostr << level(0);
    ostr << start(L"") << boost::explore::end(L"") << separator(L"\n\n");

    ostr << level(1);
    ostr << start(L"") << boost::explore::end(L"") << separator(L"\n");

    ostr << level(2);
    ostr << start(L"|") << boost::explore::end(L"|") << separator(L" ");

    return ostr;
}

BOOST_AUTO_TEST_CASE_TEMPLATE( three_dimension_stream_test, C, test_types )
{
    test_traits<C>::stream_type str_out;

    std::vector<int> vi;
    std::vector<std::vector<int> > vvi;
    std::vector<std::vector<std::vector<int> > > vvvi;

    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    vvi.push_back(vi);
    vvi.push_back(vi);
    vvi.push_back(vi);

    vvvi.push_back(vvi);
    vvvi.push_back(vvi);
    vvvi.push_back(vvi);

    str_out << format_3d << vvvi;
    BOOST_CHECK_EQUAL(output(str_out),
        "|1 2 3|\n"
        "|1 2 3|\n"
        "|1 2 3|\n"
        "\n"
        "|1 2 3|\n"
        "|1 2 3|\n"
        "|1 2 3|\n"
        "\n"
        "|1 2 3|\n"
        "|1 2 3|\n"
        "|1 2 3|"
    );
}

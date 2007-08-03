// Boost.Explore library

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
#include <boost/explore.hpp>

BOOST_AUTO_TEST_CASE( two_dimension_print_test )
{
    std::stringstream str_out;

    std::vector<int> vi;
    std::vector<std::vector<int> > vvi;

    vi.push_back(1);
    vi.push_back(2);
    vi.push_back(3);

    vvi.push_back(vi);
    vvi.push_back(vi);
    vvi.push_back(vi);

    explore::print(vvi, str_out/*, ??? */);
    BOOST_CHECK_EQUAL(str_out.str(),
        "|1 2 3|\n"
        "|1 2 3|\n"
        "|1 2 3|");
}

BOOST_AUTO_TEST_CASE( three_dimension_print_test )
{
    std::stringstream str_out;

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

    explore::print(vvvi, str_out/*, ??? */);
    BOOST_CHECK_EQUAL(str_out.str(),
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

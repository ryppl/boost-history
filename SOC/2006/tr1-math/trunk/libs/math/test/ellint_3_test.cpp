//  Copyright (c) 2006 Xiaogang Zhang
//
//  This code may be used under either of the following two licences:
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE. OF SUCH DAMAGE.
//
//  Or:
//
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include <boost/math/special_functions/ellint_3.hpp>
#include <boost/math/tools/testutils.hpp>
#include <boost/array.hpp>

template <typename T>
void test_ellint_3(T, const char* type_name)
{
   // TODO
}

template <typename T>
void test_spots(T, const char* type_name)
{
    // function values calculated on http://functions.wolfram.com/
    #define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
    static const boost::array<boost::array<T, 4>, 10> data1 = {
        SC_(-1), SC_(4), SC_(1), std::numeric_limits<T>::infinity(),
        SC_(1), SC_(-1), SC_(0), SC_(-1.557407724654902230506974807458360173087),
        SC_(0), SC_(-4), SC_(0.4), SC_(-4.153623371196831087495427530365430979011),
        SC_(0), SC_(8), SC_(-0.6), SC_(8.935930619078575123490612395578518914416),
        SC_(0.5), SC_(-1), SC_(0.5), SC_(-1.228014414316220642611298946293865487807),
        SC_(0.5), SC_(1e+10), SC_(0.5), SC_(1.536591003599172091573590441336982730551e+10),
        SC_(-1e+05), SC_(10), SC_(0.7), SC_(3.478624565944419630900942875776125238325e-02),
        SC_(-1e+10), SC_(10), SC_(0.8), SC_(1.099560394226404858867747175250612929917e-04),
        SC_(-1e+10), SC_(1e+20), SC_(0.9), SC_(1.000007059081123042096211956837192873489e+15),
        SC_(0.9999), SC_(1e+20), SC_(0.9), SC_(2.269895827363715783804411387842322904150e+22),
    };
    #undef SC_

    boost::math::tools::test_trivariate
        (data1, type_name, "boost::math::ellint_pi", boost::math::ellint_pi);

    // function values calculated on http://functions.wolfram.com/
    #define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
    static const boost::array<boost::array<T, 3>, 10> data2 = {
        SC_(-8), SC_(-1), std::numeric_limits<T>::infinity(),
        SC_(1), SC_(0.1), std::numeric_limits<T>::infinity(),
        SC_(0), SC_(0.2), SC_(1.586867847454166237308008033828114192951),
        SC_(0), SC_(0.4), SC_(1.639999865864511206865258329748601457626),
        SC_(0.5), SC_(0), SC_(2.221441469079183123507940495030346849307),
        SC_(-4), SC_(0.3), SC_(0.7127088709256200615979248581622602933052),
        SC_(-1e+05), SC_(-0.5), SC_(4.969445964850660558001091632561086046156e-03),
        SC_(-1e+10), SC_(-0.6), SC_(1.570799653392167047769596415018460890973e-05),
        SC_(1e-05), SC_(-0.8), SC_(1.995314011390755083356278758128356155910),
        SC_(0.9999), SC_(-0.9), SC_(3.565544027829987538062017902125643463427e+02),
    };
    #undef SC_

    boost::math::tools::test_bivariate
        (data2, type_name, "boost::math::ellint_pi", boost::math::ellint_pi);
}

int test_main(int, char* [])
{
    test_spots(0.0F, "float");
    test_spots(0.0, "double");
    test_spots(0.0L, "long double");

    test_ellint_3(0.0F, "float");
    test_ellint_3(0.0, "double");
    test_ellint_3(0.0L, "long double");

    return 0;
}

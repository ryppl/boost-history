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
#include <boost/math/special_functions/bessel_j0.hpp>
#include <boost/math/tools/testutils.hpp>
#include <boost/array.hpp>

template <typename T>
void test_bessel_j0(T, const char* type_name)
{
   // TODO
}

template <typename T>
void test_spots(T, const char* type_name)
{
    // function values calculated on http://functions.wolfram.com/
    #define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
    static const boost::array<boost::array<T, 2>, 10> data = {
        SC_(0), SC_(1),
        SC_(1), SC_(0.7651976865579665514497175261026632209093),
        SC_(-2), SC_(0.2238907791412356680518274546499486258252),
        SC_(4), SC_(-0.3971498098638473722865907684516980419756),
        SC_(-8), SC_(0.1716508071375539060908694078519720010684),
        SC_(1e-05), SC_(0.999999999975000000000156249999999565972),
        SC_(1e-10), SC_(0.999999999999999999997500000000000000000),
        SC_(-1e+01), SC_(-0.2459357644513483351977608624853287538296),
        SC_(1e+03), SC_(0.02478668615242017456133073111569370878617),
        SC_(1e+05), SC_(-0.001719201116235972192570601477073201747532),
    };
    #undef SC_

    boost::math::tools::test_univariate
        (data, type_name, "boost::math::bessel_j0", boost::math::bessel_j0);
}

int test_main(int, char* [])
{
    test_spots(0.0F, "float");
    test_spots(0.0, "double");
    test_spots(0.0L, "long double");

    test_bessel_j0(0.0F, "float");
    test_bessel_j0(0.0, "double");
    test_bessel_j0(0.0L, "long double");

    return 0;
}

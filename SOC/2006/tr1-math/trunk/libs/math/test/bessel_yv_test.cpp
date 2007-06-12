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
#include <boost/math/special_functions/bessel_yv.hpp>
#include <boost/math/tools/testutils.hpp>
#include <boost/array.hpp>

template <typename T>
void test_bessel_yv(T, const char* type_name)
{
   // TODO
}

template <typename T>
void test_spots(T, const char* type_name)
{
    // function values calculated on http://functions.wolfram.com/
    #define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
    static const boost::array<boost::array<T, 3>, 10> data = {
        SC_(-2.4), SC_(0), -std::numeric_limits<T>::infinity(),
        SC_(2.4), SC_(1e-02), SC_(-1.316772292110576602937691805433552633395e+05),
        SC_(5.5), SC_(3.1416), SC_(-2.558205897167535567702182324451308320967),
        SC_(-5.5), SC_(3.1416), SC_(-2.819324174208752668629781718572610128855e-02),
        SC_(-5.5), SC_(1e+04), SC_(-7.593435027226703613955851981548170471855e-03),
        SC_(-10.0003), SC_(1e-02), SC_(-1.185489791262876960891352181592988582975e+28),
        SC_(-10.0003), SC_(1e+02), SC_(5.830413916249670419752850909417042305240e-02),
        SC_(1.414e+02), SC_(1e+02), SC_(-3.965825067202950790162937002429818412238e+09),
        SC_(1.414e+02), SC_(2e+04), SC_(-5.401996477366901678039558068806048889161e-03),
        SC_(-1.414e+02), SC_(1e+02), SC_(1.225507342483879734493362979267286562073e+09),
    };
    #undef SC_

    boost::math::tools::test_bivariate
        (data, type_name, "boost::math::bessel_yv", boost::math::bessel_yv);
}

int test_main(int, char* [])
{
    test_spots(0.0F, "float");
    test_spots(0.0, "double");
    test_spots(0.0L, "long double");

    test_bessel_yv(0.0F, "float");
    test_bessel_yv(0.0, "double");
    test_bessel_yv(0.0L, "long double");

    return 0;
}

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
#include <boost/math/special_functions/ellint_1.hpp>
#include <boost/math/tools/testutils.hpp>
#include <boost/array.hpp>

template <typename T>
void test_ellint_1(T, const char* type_name)
{
   // TODO
}

template <typename T>
void test_spots(T, const char* type_name)
{
    // function values calculated on http://functions.wolfram.com/
    #define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
    static const boost::array<boost::array<T, 3>, 10> data1 = {
        SC_(0), SC_(0), SC_(0),
        SC_(-10), SC_(0), SC_(-10),
        SC_(-1), SC_(-1), SC_(-1.226191170883517070813060967471906752724),
        SC_(-4), SC_(0.9), SC_(-5.516159896432880332831709319420223014755),
        SC_(8), SC_(-0.6), SC_(8.935930619078575123490612395578518914416),
        SC_(1e-05), SC_(0.8), SC_(1.000000000010666666666760533333332439365e-05),
        SC_(1e+05), SC_(0.01), SC_(1.000025001415279653488070220634540269372e+05),
        SC_(1e-20), SC_(1), SC_(9.99999999999999945160000000000003007426e-21),
        SC_(1e+20), SC_(0.4), SC_(1.044056341289529060744402900435846080565e+20),
        SC_(1e+50), SC_(0.9), SC_(1.451842673375787722520796285757274540241e+50),
    };
    #undef SC_

    boost::math::tools::test_bivariate
        (data1, type_name, "boost::math::ellint_f", boost::math::ellint_f);

    // function values calculated on http://functions.wolfram.com/
    #define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
    static const boost::array<boost::array<T, 2>, 10> data2 = {
        SC_(-1), std::numeric_limits<T>::infinity(),
        SC_(0), SC_(1.570796326794896619231321691639751442099),
        SC_(0.1), SC_(1.574745561517355952669030688659860091647),
        SC_(0.2), SC_(1.586867847454166237308008033828114192951),
        SC_(0.3), SC_(1.608048619930512801267207222238687157112),
        SC_(0.4), SC_(1.639999865864511206865258329748601457626),
        SC_(-0.5), SC_(1.685750354812596042871203657799076989501),
        SC_(-0.6), SC_(1.750753802915752528975226046012148255767),
        SC_(-0.8), SC_(1.995302777664729387686211339372437349382),
        SC_(-0.9), SC_(2.280549138422770204613751944555530438743),
    };
    #undef SC_

    boost::math::tools::test_univariate
        (data2, type_name, "boost::math::ellint_k", boost::math::ellint_k);
}

int test_main(int, char* [])
{
    test_spots(0.0F, "float");
    test_spots(0.0, "double");
    test_spots(0.0L, "long double");

    test_ellint_1(0.0F, "float");
    test_ellint_1(0.0, "double");
    test_ellint_1(0.0L, "long double");

    return 0;
}

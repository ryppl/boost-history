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
#include <boost/math/special_functions/ellint_2.hpp>
#include <boost/math/tools/testutils.hpp>
#include <boost/array.hpp>

template <typename T>
void test_ellint_2(T, const char* type_name)
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
        SC_(-1), SC_(-1), SC_(-0.8414709848078965066525023216302989996226),
        SC_(-4), SC_(0.9), SC_(-3.121560379658397735957862817891269816990),
        SC_(8), SC_(-0.6), SC_(7.207463755621608678814797939680888563358),
        SC_(1e-05), SC_(0.8), SC_(9.99999999989333333333444266666667170540e-06),
        SC_(1e+05), SC_(0.1), SC_(9.974952919664291108398919242475795304485e+04),
        SC_(1e+10), SC_(-0.5), SC_(9.342154576648713703657674855529522225229e+09),
        SC_(1e+20), SC_(0.4), SC_(9.587122064595173113370621697155855966988e+19),
        SC_(1e+50), SC_(0.9), SC_(7.459255110255971366928641648073758583975e+49),
    };
    #undef SC_

    boost::math::tools::test_bivariate
        (data1, type_name, "boost::math::ellint_e", boost::math::ellint_e);

    // function values calculated on http://functions.wolfram.com/
    #define SC_(x) static_cast<T>(BOOST_JOIN(x, L))
    static const boost::array<boost::array<T, 2>, 10> data2 = {
        SC_(-1), SC_(1),
        SC_(0), SC_(1.570796326794896619231321691639751442099),
        SC_(0.1), SC_(1.566861942021668291220474975834679707221),
        SC_(0.2), SC_(1.554968546242529283474427007626648437453),
        SC_(0.3), SC_(1.534833464923249041645303537475267117197),
        SC_(0.4), SC_(1.505941612360040352112449140296748778403),
        SC_(-0.5), SC_(1.467462209339427155459795266990916136025),
        SC_(-0.6), SC_(1.418083394448724231567793195609859117163),
        SC_(-0.8), SC_(1.276349943169906423308933100249514569598),
        SC_(-0.9), SC_(1.171697052781614141185913957957410257425),
    };
    #undef SC_

    boost::math::tools::test_univariate
        (data2, type_name, "boost::math::ellint_e", boost::math::ellint_e);
}

int test_main(int, char* [])
{
    test_spots(0.0F, "float");
    test_spots(0.0, "double");
    test_spots(0.0L, "long double");

    test_ellint_2(0.0F, "float");
    test_ellint_2(0.0, "double");
    test_ellint_2(0.0L, "long double");

    return 0;
}

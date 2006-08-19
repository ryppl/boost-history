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

#ifndef BOOST_MATH_TOOLS_TESTUTILS_HPP
#define BOOST_MATH_TOOLS_TESTUTILS_HPP

#include <boost/math/tools/stats.hpp>
#include <boost/math/tools/test.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>

// Some basic utility routines for testing

namespace boost { namespace math { namespace tools {

template <typename T, typename Seq>
void print_test_result(const test_result<T>& result, 
                       const Seq& worst,
                       int row,
                       const char* type_name,
                       const char* func_name)
{
    using namespace std;

    T eps = pow(T(2), 1 - digits<T>());
    cout.precision(10);
    cout << func_name << "(" << type_name
         << "): Max = " << (result.stat.max)()/eps
         << "  RMS Mean = " << result.stat.rms()/eps << endl
         << "  Worst case at row " << row << ": {";
    for(unsigned i = 0; i < worst.size(); i++)
    {
      if (i) cout << ", ";
      cout << worst[i];
    }
    cout << "}" << endl;
}

// T must be a 2D boost::array here, we assume this is true
template <typename T>
struct compute_value_type
{
   typedef typename T::value_type vt;
   typedef typename vt::value_type type;
};

// define a short replacement to save some efforts
#define TVT compute_value_type<T>::type

template <typename T>
void test_univariate(const T& data,
                     const char* type_name,
                     const char* func_name,
                     typename TVT (*fp)(typename TVT))
{
    typedef typename TVT value_type;
    test_result<value_type> result;

    using namespace std;
    using namespace boost::lambda;

    cout << "Testing " << func_name << " with type " << type_name << endl
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    result = test(data,
                  bind(fp,
                       ret<value_type>(_1[0])),
                  ret<value_type>(_1[1]));
    print_test_result(result,
                      data[result.worst_case],
                      result.worst_case,
                      type_name,
                      func_name);
    cout << endl;
}

template <typename T>
void test_bivariate(const T& data,
                    const char* type_name,
                    const char* func_name,
                    typename TVT (*fp)(typename TVT, typename TVT))
{
    typedef typename TVT value_type;
    test_result<value_type> result;

    using namespace std;
    using namespace boost::lambda;

    cout << "Testing " << func_name << " with type " << type_name << endl
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    result = test(data,
                  bind(fp,
                       ret<value_type>(_1[0]),
                       ret<value_type>(_1[1])),
                  ret<value_type>(_1[2]));
    print_test_result(result,
                      data[result.worst_case],
                      result.worst_case,
                      type_name,
                      func_name);
    cout << endl;
}

template <typename T>
void test_trivariate(const T& data,
                     const char* type_name,
                     const char* func_name,
                     typename TVT (*fp)(typename TVT, typename TVT, typename TVT))
{
    typedef typename TVT value_type;
    test_result<value_type> result;

    using namespace std;
    using namespace boost::lambda;

    cout << "Testing " << func_name << " with type " << type_name << endl
         << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
    result = test(data,
                  bind(fp,
                       ret<value_type>(_1[0]),
                       ret<value_type>(_1[1]),
                       ret<value_type>(_1[2])),
                  ret<value_type>(_1[3]));
    print_test_result(result,
                      data[result.worst_case],
                      result.worst_case,
                      type_name,
                      func_name);
    cout << endl;
}

}}} // namespaces

#endif // BOOST_MATH_TOOLS_TESTUTILS_HPP

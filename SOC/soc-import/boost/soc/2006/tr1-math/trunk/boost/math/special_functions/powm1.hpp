//  (C) Copyright John Maddock 2006.
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

#ifndef BOOST_MATH_POWM1
#define BOOST_MATH_POWM1

#include <boost/math/tools/series.hpp>
#include <boost/math/tools/precision.hpp>
#include <boost/assert.hpp>

//
// This algorithm computes (x^y)-1, it's only called for small
// values of x and y, in fact x and y < 1, any other argument
// probably is not supported, and will not yield accurate results.
// Probably best not to use this unless you know that the conditions
// above are satisfied.
//

namespace boost{ namespace math{ namespace detail{

template <class T>
struct powm1_series
{
   typedef T result_type;
   powm1_series(T z, T a_) : k(1)/*, a(a_)*/
   {
      using namespace std;
      result = lz = log(z) * a_;
   }
   T operator()()
   {
      T r = result;
      result *= lz / ++k;
      return r;
   }
private:
   T result, lz/*, a*/;
   int k;
};

template <class T>
struct powp1m1_series
{
   typedef T result_type;

   powp1m1_series(T z_, T a_) : k(1), result(z_*a_), z(z_), a(a_){}
   T operator()()
   {
      T r = result;
      result *= (a-k)*z;
      result /= ++k;
      return r;
   }
private:
   int k;
   T result, z, a;
};

} // namespace detail

template <class T>
T powm1(const T a, const T z)
{
   using namespace std;

   T result = pow(a, z) - 1;

   if(fabs(result) < T(0.5))
   {
      if(fabs(a-1) < fabs(z))
      {
         detail::powp1m1_series<T> gen(a-1, z);
         result =  tools::kahan_sum_series(gen, ::boost::math::tools::digits<T>());
      }
      else
      {
         detail::powm1_series<T> gen(a, z);
         result =  tools::kahan_sum_series(gen, ::boost::math::tools::digits<T>());
      }
   }
   return result;
}

} } // namespaces

#endif // BOOST_MATH_POWM1




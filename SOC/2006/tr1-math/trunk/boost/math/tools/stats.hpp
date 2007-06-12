//  (C) Copyright John Maddock 2005-2006.
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

#ifndef BOOST_MATH_TOOLS_STATS_INCLUDED
#define BOOST_MATH_TOOLS_STATS_INCLUDED

#include <cmath>
#include <boost/cstdint.hpp>
#include <boost/math/tools/precision.hpp>

namespace boost{ namespace math{ namespace tools{

template <class T>
class stats
{
public:
   stats() 
      : m_min(tools::max_value<T>()),
        m_max(-tools::max_value<T>()),
        m_total(0),
        m_squared_total(0),
        m_count(0)
   {}
   void add(const T& val)
   {
      if(val < m_min)
         m_min = val;
      if(val > m_max)
         m_max = val;
      m_total += val;
      ++m_count;
      m_squared_total += val*val;
   }
   T min BOOST_PREVENT_MACRO_SUBSTITUTION()const{ return m_min; }
   T max BOOST_PREVENT_MACRO_SUBSTITUTION()const{ return m_max; }
   T total()const{ return m_total; }
   T mean()const{ return m_total / static_cast<T>(m_count); }
   boost::uintmax_t count()const{ return m_count; }
   T variance()const 
   {
      using namespace std;

      T m = mean();
      T t = m_squared_total - m_total * m_total / m_count;
      t /= m_count;
      return t;
   }
   T variance1()const 
   {
      using namespace std;

      T m = mean();
      T t = m_squared_total - m_total * m_total / m_count;
      t /= (m_count-1);
      return t;
   }
   T rms()const
   {
      using namespace std;

      return sqrt(m_squared_total / static_cast<T>(m_count));
   }
   stats& operator+=(const stats& s)
   {
      if(s.m_min < m_min)
         m_min = s.m_min;
      if(s.m_max > m_max)
         m_max = s.m_max;
      m_total += s.m_total;
      m_squared_total += s.m_squared_total;
      m_count += s.m_count;
      return *this;
   }
private:
   T m_min, m_max, m_total, m_squared_total;
   boost::uintmax_t m_count;
};

} } } // namespaces

#endif 

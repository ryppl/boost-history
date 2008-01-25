//  Copyright John Maddock 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_TOOLS_REAL_CAST_HPP
#define BOOST_MATH_TOOLS_REAL_CAST_HPP

#ifdef _MSC_VER
#pragma once
#endif

#include <boost/math/tools/config.hpp>

namespace boost{ namespace math
{
   namespace tools
   {
      template <class To, class T>
      To real_cast(T t);

      namespace detail{

         template <class To, class T>
         inline To real_cast(T t, const mpl::false_&)
         {
            return static_cast<To>(t);
         }
         template <class To, class T>
         inline To real_cast(T t, const mpl::true_&)
         {
            return boost::math::tools::real_cast<To>(median(t));
         }

      }

      template <class To, class T>
      inline To real_cast(T t)
      {
         typedef boost::numeric::is_interval<T> tag_type;
         return detail::real_cast<To>(t, tag_type());
      }
   } // namespace tools
} // namespace math
} // namespace boost

#endif // BOOST_MATH_TOOLS_REAL_CAST_HPP




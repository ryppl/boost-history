// integer_log2.hpp
//
// Gives the integer part of the logarithm, base 2, of a given number.
// Behavior is undefined if the argument is <= 0.
//
//
//  Permission to copy, use, modify, sell and distribute this software
//  is granted provided this copyright notice appears in all copies.
//  This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.
//
// (C) Gennaro Prota 2003

#ifndef BOOST_INTEGER_LOG2_HPP_GP_20030301
#define BOOST_INTEGER_LOG2_HPP_GP_20030301

#include <cassert>
#include "boost/limits.hpp"
#include "boost/cstdint.hpp"
#include "boost/config.hpp"


namespace boost {
 namespace detail {


  int integer_log2_impl(uintmax_t x, int n) {

      int result = 0;

      while (x != 1) {

        const uintmax_t t = x >> n;
        if (t) {
          result += n;
          x = t;
        }
        n /= 2;

      }

      return result;
  }



  // helper to find the maximum power of two
  // less than w (more involved than necessary,
  // to avoid PTS)
  //
  template <int w, int n>
  struct max_pow2_less {

      enum { c = 2*n < w };

      BOOST_STATIC_CONSTANT(int, value = c ? (max_pow2_less< c*w, 2*c*n>::value) : n);

  };

  template <>
  struct max_pow2_less<0, 0> {
     
      BOOST_STATIC_CONSTANT(int, value = 0);
  };

 } // detail


 // ---------
 // integer_log2
 // ---------------
 //
 template <typename T>
 int integer_log2(T x) {

     assert (x > 0);

     const int n = detail::max_pow2_less< std::numeric_limits<T> :: digits, 4 > :: value;

     return detail::integer_log2_impl(x, n);

 }



}



#endif // include guard

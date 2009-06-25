// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BITWISE_OPS_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_BITWISE_OPS_HPP

#include <boost/mp_math/integer/detail/base/bitwise_ops.hpp>

namespace boost {
namespace mp_math {
namespace detail {


template<class ApInt>
struct bitwise_ops
{
  typedef base::bitwise_ops<ApInt> base_bitwise_ops_type;

  static void or_bits (ApInt& z, const ApInt& x, const ApInt& y)
  {
    z.reserve(std::max(x.size(), y.size()));
    base_bitwise_ops_type::or_bits(z, x, y);
  }

  static void and_bits(ApInt& z, const ApInt& x, const ApInt& y)
  {
    z.reserve(std::min(x.size(), y.size()));
    base_bitwise_ops_type::and_bits(z, x, y);
  }

  static void xor_bits(ApInt& z, const ApInt& x, const ApInt& y)
  {
    z.reserve(std::max(x.size(), y.size()));
    base_bitwise_ops_type::xor_bits(z, x, y);
  }
};


} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

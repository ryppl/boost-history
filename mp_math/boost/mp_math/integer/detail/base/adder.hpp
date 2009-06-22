// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BASE_ADDER_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_BASE_ADDER_HPP

namespace boost {
namespace mp_math {
namespace detail {
namespace base {


template<class ApInt>
struct adder
{
  typedef typename ApInt::traits_type      traits_type;
  typedef typename traits_type::digit_type digit_type;
  typedef typename traits_type::size_type  size_type;
  typedef typename traits_type::ops_type   ops_type;

  static void add_smaller_magnitude(ApInt& z,
                                    const ApInt& x, const ApInt& y);

  static void subtract_smaller_magnitude(ApInt& z, const ApInt& x);
  static void subtract_smaller_magnitude(ApInt& z,
                                         const ApInt& x, const ApInt& y);
};


template<class ApInt>
void adder<ApInt>::add_smaller_magnitude(ApInt& z, const ApInt& x, const ApInt& y)
{
  assert(x.size() >= y.size());

  digit_type carry = ops_type::add_digits(z.digits(),
                                          x.digits(),
                                          y.digits(), y.size());

  size_type n = ops_type::ripple_carry(z.digits() + y.size(),
                                       x.digits() + y.size(),
                                       x.size() - y.size(), carry);
  n += y.size();

  if (n < x.size()) // this implies that there is no carry left
  {
    if (&x != &z)
    {
      std::memcpy(z.digits() + n,
                  x.digits() + n,
                  sizeof(digit_type) * (x.size() - n));
      z.set_size(x.size());
    }
    return;
  }
  else if (carry) // at this point n equals x.size()
    z[n++] = carry;

  z.set_size(n);
}

template<class ApInt>
void adder<ApInt>::subtract_smaller_magnitude(ApInt& z, const ApInt& x)
{
  ApInt::traits_type::ops_type::sub_smaller_magnitude(
      z.digits(), z.digits(), z.size(), x.digits(), x.size());

  z.clamp();
}

template<class ApInt>
void adder<ApInt>::subtract_smaller_magnitude(ApInt& z, const ApInt& x, const ApInt& y)
{
  ApInt::traits_type::ops_type::sub_smaller_magnitude(
      z.digits(), x.digits(), x.size(), y.digits(), y.size());

  z.clamp();
}


} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

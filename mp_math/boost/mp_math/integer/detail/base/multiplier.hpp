// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BASE_MULTIPLIER_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_BASE_MULTIPLIER_HPP

namespace boost {
namespace mp_math {
namespace detail {
namespace base {


template<class ApInt>
struct multiplier
{
  typedef ApInt                          int_type;
  typedef typename int_type::digit_type  digit_type;
  typedef typename int_type::traits_type traits_type;
  typedef typename traits_type::ops_type ops_type;

  static void multiply_or_square(ApInt& z, const ApInt& x, const ApInt& y);

  static void mul      (ApInt& z, const ApInt& x, const ApInt& y);
  static void comba_mul(ApInt& z, const ApInt& x, const ApInt& y);

  static void comba_sqr(ApInt& z, const ApInt& x);
};


template<class ApInt>
void
multiplier<ApInt>::multiply_or_square(ApInt& z, const ApInt& x, const ApInt& y)
{
  if (&x == &y)
    comba_sqr(z, x);
  else
    mul(z, x, y);
}

template<class ApInt>
void multiplier<ApInt>::mul(ApInt& z, const ApInt& x, const ApInt& y)
{
  // always multiply larger by smaller number
  const ApInt* a = &x;
  const ApInt* b = &y;
  if (a->size() < b->size())
    std::swap(a, b);

  if (b->size() == 1U)
  {
    if ((*b)[0] == 0U)
    {
      z[0] = 0;
      z.set_size(1);
      // TODO set_sign_bit(0) for signed ApInt!
      return;
    }
    else if ((*b)[0] == 1U)
    {
      z = *a;
      return;
    }

    ApInt::template integral_ops<digit_type>::multiply(z, *a, (*b)[0]);

    return;
  }

  comba_mul(z, *a, *b);
}

template<class ApInt>
void multiplier<ApInt>::comba_mul(ApInt& z, const ApInt& x, const ApInt& y)
{
  // We assert this because otherwise we would need to call z.clamp() below.
  assert(x);
  assert(y);

  if (x.size() == y.size())
    ops_type::comba_mul(z.digits(), x.digits(), y.digits(), y.size());
  else
    ops_type::comba_mul(z.digits(), x.digits(), x.size(),
                                    y.digits(), y.size());

  z.set_size(x.size() + y.size());
  z.clamp_high_digit();
}

template<class ApInt>
void multiplier<ApInt>::comba_sqr(ApInt& z, const ApInt& x)
{
  ops_type::comba_sqr(z.digits(), x.digits(), x.size());

  z.set_size(x.size() + x.size());

  z.clamp_high_digit();
}


} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost

#endif


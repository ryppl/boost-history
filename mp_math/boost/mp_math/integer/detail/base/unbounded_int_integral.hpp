// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BASE_UNBOUNDED_INT_INTEGRAL_OPS_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_BASE_UNBOUNDED_INT_INTEGRAL_OPS_HPP

#include <boost/mp_math/integer/detail/base/unbounded_int_fwd.hpp>
#include <boost/mp_math/integer/detail/base/unbounded_uint_integral.hpp>

// Here we optimize interaction with built in integral types.
// This code is hairy and subtle.

namespace boost {
namespace mp_math {
namespace detail {
namespace base {

// We create four partial specializations out of this class template:
// 1) IntegralT = digit_type,              is_signed = false
// 2) IntegralT = make_signed<digit_type>, is_signed = true
// 3) is_signed = false
// 4) is_signed = true
// The actual dispatching to the correct specialization is done in
// unbounded_int_integral_ops.
// All IntegralT that fit into a digit_type are dispatched to 1).
// All IntegralT that fit into a 'signed digit_type' are dispatched to 2).
// All IntegralT larger than a digit_type are dispatched to 3).
// All IntegralT larger than a 'signed digit_type' are dispatched to 4).
template<
  class UnboundedInt,
  typename IntegralT,
  bool IsSigned = std::numeric_limits<IntegralT>::is_signed
>
struct unbounded_int_integral_ops_impl;


// 1)
template<class UnboundedInt>
struct unbounded_int_integral_ops_impl<
  UnboundedInt,
  typename UnboundedInt::digit_type,
  false
>
{
  typedef UnboundedInt                             unbounded_int_type;
  typedef typename unbounded_int_type::digit_type  integral_type;
  typedef typename unbounded_int_type::traits_type traits_type;

  typedef unbounded_uint_integral_ops<
    unbounded_int_type, integral_type
  >                                        magnitude_type_integral_ops;

  static void assign(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs[0] = rhs;
    lhs.set_size(1);
    lhs.set_sign_bit(0);
  }

  static bool equal(const unbounded_int_type& lhs, integral_type rhs)
  {
    if (lhs.is_negative())
      return false;
    if (lhs.size() > 1)
      return false;
    return lhs[0] == rhs;
  }

  static bool less(const unbounded_int_type& lhs, integral_type rhs)
  {
    if (lhs.is_negative())
      return true;
    if (lhs.size() > 1)
      return false;
    return lhs[0] < rhs;
  }

  static void add     (unbounded_int_type& lhs, integral_type rhs);
  static void subtract(unbounded_int_type& lhs, integral_type rhs);

  static void multiply(unbounded_int_type& lhs, integral_type rhs)
  {
    multiply(lhs, lhs, rhs);
  }

  static void multiply(unbounded_int_type& z,
                       const unbounded_int_type& x, integral_type y);

  static void divide(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::divide(lhs, rhs);
    if (!lhs)
      lhs.set_sign_bit(0);
  }

  static void modulo(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::modulo(lhs, rhs);
    // TODO sign
  }

  static void bitwise_or(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs[0] |= rhs;
  }

  static void bitwise_and(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs[0] &= rhs;
    lhs.set_sign_bit(0);
  }

  static void bitwise_xor(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs[0] ^= rhs;
  }
};


template<class UnboundedInt>
void
unbounded_int_integral_ops_impl<
  UnboundedInt, typename UnboundedInt::digit_type, false
>::add(unbounded_int_type& lhs, integral_type rhs)
{
  if (lhs.is_positive())
    magnitude_type_integral_ops::add(lhs, rhs);
  else
  {
    if (lhs[0] > rhs) // example: -16 + 5 = -11
      lhs[0] -= rhs;
    else
    {
      if (lhs.size() == 1) // example: -1 + 5 = 4, or -5 + 5 = 0
      {
        lhs[0] = rhs - lhs[0];
        lhs.set_sign_bit(0);
      }
      else            // example -1000 + 5 = -995
      {
        traits_type::ops_type::subtract_single_digit(
            lhs.digits(), lhs.digits(), lhs.size(), rhs);
        lhs.clamp_high_digit();
      }
    }
  }
}

template<class UnboundedInt>
void
unbounded_int_integral_ops_impl<
  UnboundedInt, typename UnboundedInt::digit_type, false
>::subtract(unbounded_int_type& lhs, integral_type rhs)
{
  if (lhs.is_negative())
  {
    const integral_type carry =
      traits_type::ops_type::add_single_digit(
          lhs.digits(), lhs.digits(), lhs.size(), rhs);
    if (carry)
      lhs.push(carry);
    return;
  }

  if (lhs.size() == 1)
  {
    if (lhs[0] < rhs) // example: 2 - 6 = -4
    {
      lhs[0] = rhs - lhs[0];
      lhs.set_sign_bit(1);
    }
    else // example: 8 - 7 = 1 or 5 - 5 = 0
      lhs[0] -= rhs;
  }
  else
  {
    traits_type::ops_type::subtract_single_digit(
        lhs.digits(), lhs.digits(), lhs.size(), rhs);
    lhs.clamp_high_digit();
  }
}

template<class UnboundedInt>
void
unbounded_int_integral_ops_impl<
  UnboundedInt, typename UnboundedInt::digit_type, false
>::multiply(unbounded_int_type& z,
            const unbounded_int_type& x, integral_type y)
{
  if (y == 0)
  {
    assign(z, integral_type(0));
    return;
  }
  else if (y == 1)
    return;

  const integral_type carry =
    traits_type::ops_type::multiply_by_digit(
        z.digits(), x.digits(), x.size(), y);

  if (carry)
    z.push(carry);
}


// 2)
template<class UnboundedInt>
struct unbounded_int_integral_ops_impl<
  UnboundedInt,
  typename make_signed<typename UnboundedInt::digit_type>::type,
  true
>
{
  typedef UnboundedInt                             unbounded_int_type;
  typedef typename make_signed<
    typename unbounded_int_type::digit_type
  >::type                                          integral_type;
  typedef std::numeric_limits<integral_type>       integral_type_limits;
  typedef typename unbounded_int_type::traits_type traits_type;

  typedef unbounded_int_integral_ops_impl<
    unbounded_int_type,
    typename unbounded_int_type::digit_type
  >                                                impl_type;

  typedef unbounded_uint_integral_ops<
    typename unbounded_int_type::magnitude_type,
    typename unbounded_int_type::digit_type
  >                                                magnitude_type_integral_ops;

  static const unsigned radix_bits = traits_type::radix_bits;
  static const unsigned q =
    (integral_type_limits::digits + (radix_bits - 1))
    / radix_bits;

  static typename unbounded_int_type::digit_type get_absolute(integral_type x)
  {
    if (x >= 0)
      return static_cast<typename unbounded_int_type::digit_type>(x);
    else
      return static_cast<typename unbounded_int_type::digit_type>(-x);
  }

  static bool get_sign_bit(integral_type x)
  {
    if (x >= 0)
      return 0;
    else
      return 1;
  }

  static void assign(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs[0] = get_absolute(rhs);
    lhs.set_size(1);
    lhs.set_sign_bit(get_sign_bit(rhs));
  }

  static bool equal(const unbounded_int_type& lhs, integral_type rhs)
  {
    if (lhs.sign_bit() == get_sign_bit(rhs))
    {
      return magnitude_type_integral_ops::equal(lhs, get_absolute(rhs));
    }
    else
      return false;
  }

  static bool less(const unbounded_int_type& lhs, integral_type rhs);

  static void add(unbounded_int_type& lhs, integral_type rhs)
  {
    if (lhs.sign_bit() == get_sign_bit(rhs))
      magnitude_type_integral_ops::add(lhs, get_absolute(rhs));
    else
    {
      if (lhs.is_positive())
      {
        const typename unbounded_int_type::digit_type x = get_absolute(rhs);
        if (lhs.size() == 1)
        {
          if (lhs[0] >= x)
            lhs[0] -= x;
          else
          {
            lhs[0] = x - lhs[0];
            lhs.set_sign_bit(1);
          }
        }
        else
          magnitude_type_integral_ops::subtract(lhs, x);
      }
      else
      {
        if (lhs.size() == 1)
        {
          if (lhs[0] > rhs)
            lhs[0] -= rhs;
          else
          {
            lhs[0] = rhs - lhs[0];
            lhs.set_sign_bit(0);
          }
        }
        else
          magnitude_type_integral_ops::add(lhs, rhs);
      }
    }
  }

  static void subtract(unbounded_int_type& lhs, integral_type rhs);

  static void multiply(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::multiply(lhs, get_absolute(rhs));
    lhs.set_sign_bit(lhs.sign_bit() ^ get_sign_bit(rhs));
  }

  static void divide(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::divide(lhs, get_absolute(rhs));
    if (lhs)
      lhs.set_sign_bit(lhs.sign_bit() ^ get_sign_bit(rhs));
    else
      lhs.set_sign_bit(0);
  }

  static void modulo(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::modulo(lhs, get_absolute(rhs));
    // TODO sign
  }

  static void bitwise_or(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::bitwise_or(lhs, get_absolute(rhs));
    lhs.set_sign_bit(lhs.sign_bit() | get_sign_bit(rhs));
  }

  static void bitwise_and(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::bitwise_and(lhs, get_absolute(rhs));
    lhs.set_sign_bit(lhs.sign_bit() & get_sign_bit(rhs));
  }

  static void bitwise_xor(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::bitwise_xor(lhs, get_absolute(rhs));
    lhs.set_sign_bit(lhs.sign_bit() ^ get_sign_bit(rhs));
  }
};


template<class UnboundedInt>
bool
unbounded_int_integral_ops_impl<
  UnboundedInt,
  typename make_signed<typename UnboundedInt::digit_type>::type,
  true
>::less(const unbounded_int_type& lhs, integral_type rhs)
{
  if (lhs.is_positive())
  {
    if (rhs < 0)
      return false;
    else
      return magnitude_type_integral_ops::less(lhs, get_absolute(rhs));
  }
  else
  {
    if (rhs >= 0)
      return true;
    else
    {
      if (lhs.size() <= q)
      {
        typedef to_integral_converter<
          unbounded_int_type, integral_type> converter_type;

        converter_type c;

        const integral_type x = c.convert(lhs);

        return x < rhs;
      }
      else
        return true;
    }
  }

}

template<class UnboundedInt>
void
unbounded_int_integral_ops_impl<
  UnboundedInt,
  typename make_signed<typename UnboundedInt::digit_type>::type,
  true
>::subtract(unbounded_int_type& lhs, integral_type rhs)
{
  if (lhs.is_positive())
  {
    if (rhs >= 0)
      impl_type::subtract(lhs, get_absolute(rhs));
    else
      magnitude_type_integral_ops::add(lhs, get_absolute(rhs));
  }
  else
  {
    if (rhs >= 0)
      magnitude_type_integral_ops::add(lhs, get_absolute(rhs));
    else
      impl_type::subtract(lhs, get_absolute(rhs));
  }
}


// 3
template<class UnboundedInt, typename IntegralT>
struct unbounded_int_integral_ops_impl<
  UnboundedInt, IntegralT, false
>
:
  unbounded_uint_integral_ops_impl<UnboundedInt, IntegralT, false>
{
  typedef UnboundedInt                             unbounded_int_type;
  typedef IntegralT                                integral_type;
  typedef typename unbounded_int_type::traits_type traits_type;

  typedef unbounded_uint_integral_ops<
    typename unbounded_int_type::magnitude_type,
    integral_type
  >                                     magnitude_type_integral_ops;

  static void assign(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::assign(lhs, rhs);
    lhs.set_sign_bit(0);
  }

  static bool equal(const unbounded_int_type& lhs, integral_type rhs)
  {
    if (lhs.is_negative())
      return false;
    return magnitude_type_integral_ops::equal(lhs, rhs);
  }

  static bool less(const unbounded_int_type& lhs, integral_type rhs)
  {
    if (lhs.is_negative())
      return true;
    return magnitude_type_integral_ops::less(lhs, rhs);
  }

  static void add(unbounded_int_type& lhs, integral_type rhs)
  {
    if (lhs.is_positive())
      magnitude_type_integral_ops::add(lhs, rhs);
    else
      subtract_smaller(lhs, rhs, 0);
  }

  static void subtract(unbounded_int_type& lhs, integral_type rhs)
  {
    if (lhs.is_negative())
      magnitude_type_integral_ops::add(lhs, rhs);
    else
      subtract_smaller(lhs, rhs, ~lhs.sign_bit());
  }

  static void multiply(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::multiply(lhs, rhs);
    if (!lhs)
      lhs.set_sign_bit(0);
  }

  static void divide(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::divide(lhs, rhs);
    if (!lhs)
      lhs.set_sign_bit(0);
  }

  static void modulo(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::modulo(lhs, rhs);
    if (!lhs)
      lhs.set_sign_bit(0);
  }

  static void bitwise_or(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::bitwise_or(lhs, rhs);
  }

  static void bitwise_and(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::bitwise_and(lhs, rhs);
    lhs.set_sign_bit(0);
  }

  static void bitwise_xor(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::bitwise_xor(lhs, rhs);
    lhs.set_sign_bit(lhs.sign_bit() ^ 0);
  }

  static void subtract_smaller(unbounded_int_type& lhs,
                               integral_type rhs,
                               bool final_sign);
};


template<class UnboundedInt, typename IntegralT>
void
unbounded_int_integral_ops_impl<
  UnboundedInt, IntegralT, false
>::subtract_smaller(unbounded_int_type& lhs,
                    integral_type rhs,
                    bool final_sign)
{
  static const unsigned radix_bits = traits_type::radix_bits;

  static const unsigned q =
    (std::numeric_limits<integral_type>::digits + (radix_bits - 1))
    / radix_bits;

  typedef unbounded_uint<traits_type> unbounded_uint_type;

  typename traits_type::digit_type tmp_digits[q];

  unbounded_uint_type tmp(tmp_digits, q, q);

  from_integral_converter<
    unbounded_uint_type, integral_type
  >::convert(tmp, rhs);

  const unbounded_uint_type* x;
  const unbounded_uint_type* y;

  if (!unbounded_uint_integral_ops<
        unbounded_int_type, integral_type>::less(lhs, rhs)) // lhs >= rhs
  {
    x = &lhs;
    y = &tmp;
  }
  else
  {
    x = &tmp;
    y = &lhs;
    lhs.set_sign_bit(final_sign);
  }

  traits_type::ops_type::sub_smaller_magnitude(
      lhs.digits(), x->digits(), x->size(),
                    y->digits(), y->size());

  lhs.set_size(x->size());
  lhs.clamp();

  if (!lhs)
    lhs.set_sign_bit(0);
}


// 4
template<class UnboundedInt, typename IntegralT>
struct unbounded_int_integral_ops_impl<
  UnboundedInt, IntegralT, true
>
{
  typedef UnboundedInt                                unbounded_int_type;
  typedef IntegralT                                   integral_type;
  typedef std::numeric_limits<integral_type>          integral_type_limits;
  typedef typename unbounded_int_type::traits_type    traits_type;
  typedef typename make_unsigned<integral_type>::type unsigned_integral_type;

  typedef unbounded_uint_integral_ops<
    typename unbounded_int_type::magnitude_type,
    unsigned_integral_type
  >                                          magnitude_type_integral_ops;

  static const unsigned radix_bits = traits_type::radix_bits;
  static const unsigned q =
    (integral_type_limits::digits + (radix_bits - 1))
    / radix_bits;

  static unsigned_integral_type get_absolute(integral_type x)
  {
    if (x >= 0)
      return static_cast<unsigned_integral_type>(x);
    else
      return static_cast<unsigned_integral_type>(-x);
  }

  static bool get_sign_bit(integral_type x)
  {
    if (x >= 0)
      return 0;
    else
      return 1;
  }

  static void assign(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::assign(lhs, get_absolute(rhs));
    lhs.set_sign_bit(get_sign_bit(rhs));
  }

  static bool equal(const unbounded_int_type& lhs, integral_type rhs)
  {
    if (lhs.sign_bit() != get_sign_bit(rhs))
      return false;
    return magnitude_type_integral_ops::equal(lhs, get_absolute(rhs));
  }

  static bool less(const unbounded_int_type& lhs, integral_type rhs);

  static void add     (unbounded_int_type& lhs, integral_type rhs);
  static void subtract(unbounded_int_type& lhs, integral_type rhs);

  static void multiply(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::multiply(lhs, get_absolute(rhs));
    if (lhs)
      lhs.set_sign_bit(lhs.sign_bit() ^ get_sign_bit(rhs));
    else
      lhs.set_sign_bit(0);
  }

  static void multiply(unbounded_int_type& z,
                       const unbounded_int_type& x, integral_type y)
  {
    magnitude_type_integral_ops::multiply(z, x, get_absolute(y));
    if (z)
      z.set_sign_bit(x.sign_bit() ^ get_sign_bit(y));
    else
      z.set_sign_bit(0);
  }

  static void divide(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::divide(lhs, get_absolute(rhs));
    lhs.set_sign_bit(lhs.sign_bit() ^ get_sign_bit(rhs));
  }

static void modulo(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::modulo(lhs, get_absolute(rhs));
    // TODO sign
  }

  static void bitwise_or(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::bitwise_or(lhs, rhs);
    lhs.set_sign_bit(lhs.sign_bit() | get_sign_bit(rhs));
  }

  static void bitwise_and(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::bitwise_and(lhs, rhs);
    lhs.set_sign_bit(lhs.sign_bit() & get_sign_bit(rhs));
  }

  static void bitwise_xor(unbounded_int_type& lhs, integral_type rhs)
  {
    magnitude_type_integral_ops::bitwise_xor(lhs, rhs);
    lhs.set_sign_bit(lhs.sign_bit() ^ get_sign_bit(rhs));
  }
};


template<class UnboundedInt, typename IntegralT>
bool
unbounded_int_integral_ops_impl<
  UnboundedInt, IntegralT, true>::less(const unbounded_int_type& lhs,
                                       integral_type rhs)
{
  if (lhs.is_positive())
  {
    if (rhs < 0)
      return false;
    else
      return magnitude_type_integral_ops::less(lhs, get_absolute(rhs));
  }
  else
  {
    if (rhs >= 0)
      return true;
    else
    {
      if (lhs.size() <= q)
      {
        typedef to_integral_converter<
          unbounded_int_type, integral_type> converter_type;

        converter_type c;

        const integral_type x = c.convert(lhs);

        return x < rhs;
      }
      else
        return true;
    }
  }
}

template<class UnboundedInt, typename IntegralT>
void
unbounded_int_integral_ops_impl<
  UnboundedInt, IntegralT, true>::add(unbounded_int_type& lhs,
                                      integral_type rhs)
{
  if (lhs.sign_bit() == get_sign_bit(rhs))
    magnitude_type_integral_ops::add(lhs, get_absolute(rhs));
  else
    subtract_smaller(lhs, rhs, get_sign_bit(rhs));
}

template<class UnboundedInt, typename IntegralT>
void
unbounded_int_integral_ops_impl<
  UnboundedInt, IntegralT, true>::subtract(unbounded_int_type& lhs,
                                           integral_type rhs)
{
  if (lhs.sign_bit() != get_sign_bit(rhs))
    magnitude_type_integral_ops::add(lhs, get_absolute(rhs));
  else
    subtract_smaller(lhs, rhs, ~lhs.sign_bit());
}




////////////////////////////////////////////////////////////////////

template<
  class UnboundedInt,
  typename IntegralT,
  bool less_equal =
      std::numeric_limits<IntegralT>::is_signed ?
      (
        std::numeric_limits<IntegralT>::digits <=
        std::numeric_limits<
          typename make_signed<typename UnboundedInt::digit_type>::type
        >::digits
      ):
      (
        std::numeric_limits<IntegralT>::digits <=
        std::numeric_limits<typename UnboundedInt::digit_type>::digits
      )
>
struct unbounded_int_integral_ops;


// Dispatches integral types that fit into a digit_type by casting it to
// digit_type or signed digit_type and using the specialization at the top.
template<
  class UnboundedInt,
  typename IntegralT
>
struct unbounded_int_integral_ops<UnboundedInt,IntegralT,true>
{
  BOOST_STATIC_ASSERT(is_integral<IntegralT>::value);

  typedef UnboundedInt unbounded_int_type;
  typedef IntegralT    integral_type;

  typedef unbounded_int_integral_ops_impl<
    unbounded_int_type,
    typename mpl::if_<
      is_signed<integral_type>,
      typename make_signed<
        typename unbounded_int_type::digit_type
      >::type,
      typename unbounded_int_type::digit_type
    >::type
  >                    impl_type;

  typedef typename impl_type::integral_type  to_integral_type;

  static void assign(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::assign(lhs, static_cast<to_integral_type>(rhs));
  }

  static bool equal(const unbounded_int_type& lhs, integral_type rhs)
  {
    return impl_type::equal(lhs, static_cast<to_integral_type>(rhs));
  }

  static bool less(const unbounded_int_type& lhs, integral_type rhs)
  {
    return impl_type::less(lhs, static_cast<to_integral_type>(rhs));
  }

  static void add(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::add(lhs, static_cast<to_integral_type>(rhs));
  }

  static void subtract(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::subtract(lhs, static_cast<to_integral_type>(rhs));
  }

  static void multiply(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::multiply(lhs, static_cast<to_integral_type>(rhs));
  }

  static void multiply(unbounded_int_type& z,
                       const unbounded_int_type& x, integral_type y)
  {
    impl_type::multiply(z, x, static_cast<to_integral_type>(y));
  }

  static void divide(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::divide(lhs, static_cast<to_integral_type>(rhs));
  }

  static void modulo(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::modulo(lhs, static_cast<to_integral_type>(rhs));
  }

  static void bitwise_or(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_or(lhs, static_cast<to_integral_type>(rhs));
  }

  static void bitwise_and(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_and(lhs, static_cast<to_integral_type>(rhs));
  }

  static void bitwise_xor(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_xor(lhs, static_cast<to_integral_type>(rhs));
  }
};


template<
  class UnboundedInt,
  typename IntegralT
>
struct unbounded_int_integral_ops<UnboundedInt,IntegralT,false>
{
  BOOST_STATIC_ASSERT(is_integral<IntegralT>::value);

  typedef UnboundedInt unbounded_int_type;
  typedef IntegralT    integral_type;

  typedef unbounded_int_integral_ops_impl<
      unbounded_int_type, integral_type
  >                     impl_type;

  static void assign(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::assign(lhs, rhs);
  }

  static bool equal(const unbounded_int_type& lhs, integral_type rhs)
  {
    return impl_type::equal(lhs, rhs);
  }

  static bool less(const unbounded_int_type& lhs, integral_type rhs)
  {
    return impl_type::less(lhs, rhs);
  }

  static void add(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::add(lhs, rhs);
  }

  static void subtract(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::subtract(lhs, rhs);
  }

  static void multiply(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::multiply(lhs, rhs);
  }

  static void multiply(unbounded_int_type& z,
                       const unbounded_int_type& x, integral_type y)
  {
    impl_type::multiply(z, x, y);
  }

  static void divide(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::divide(lhs, rhs);
  }

  static void modulo(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::modulo(lhs, rhs);
  }

  static void bitwise_or(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_or(lhs, rhs);
  }

  static void bitwise_and(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_and(lhs, rhs);
  }

  static void bitwise_xor(unbounded_int_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_xor(lhs, rhs);
  }
};





} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost

#endif


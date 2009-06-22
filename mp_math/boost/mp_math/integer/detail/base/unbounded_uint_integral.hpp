// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BASE_UNBOUNDED_UINT_INTEGRAL_OPS_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_BASE_UNBOUNDED_UINT_INTEGRAL_OPS_HPP

#include <boost/static_assert.hpp>
#include <boost/mpl/less_equal.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/type_traits/make_signed.hpp>
#include <boost/type_traits/make_unsigned.hpp>
#include <boost/mp_math/integer/detail/base/from_integral.hpp>
#include <boost/mp_math/integer/detail/base/to_integral.hpp>
#include <boost/mp_math/integer/detail/base/unbounded_uint_fwd.hpp>

// Here we optimize interaction with built in integral types.

namespace boost {
namespace mp_math {
namespace detail {
namespace base {

template<
  typename IntegralT,
  bool IsSigned = std::numeric_limits<IntegralT>::is_signed
>
struct integral_info;


template<typename IntegralT>
struct integral_info<IntegralT, false>
{
  typedef IntegralT integral_type;
  typedef typename make_signed<integral_type>::type signed_integral_type;

  static const bool is_signed = false;
};


template<typename IntegralT>
struct integral_info<IntegralT, true>
{
  typedef IntegralT integral_type;
  typedef typename make_unsigned<integral_type>::type unsigned_integral_type;

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

  static const bool is_signed = true;
};


// We create four partial specializations out of this class template:
// 1) IntegralT = digit_type,              is_signed = false
// 2) IntegralT = make_signed<digit_type>, is_signed = true
// 3) is_signed = false
// 4) is_signed = true
// The actual dispatching to the correct specialization is done in
// unbounded_uint_integral_ops.
// All IntegralT that fit into a digit_type are dispatched to 1).
// All IntegralT that fit into a 'signed digit_type' are dispatched to 2).
// All IntegralT larger than a digit_type are dispatched to 3).
// All IntegralT larger than a 'signed digit_type' are dispatched to 4).
template<
  class UnboundedUInt,
  typename IntegralT,
  bool IsSigned = std::numeric_limits<IntegralT>::is_signed
>
struct unbounded_uint_integral_ops_impl;


// 1)
template<class UnboundedUInt>
struct unbounded_uint_integral_ops_impl<
  UnboundedUInt,
  typename UnboundedUInt::digit_type,
  false
>
{
  typedef UnboundedUInt                             unbounded_uint_type;
  typedef typename unbounded_uint_type::digit_type  integral_type;
  typedef typename unbounded_uint_type::traits_type traits_type;

  static void assign(unbounded_uint_type& lhs, integral_type rhs)
  {
    lhs[0] = rhs;
    lhs.set_size(1);
  }

  static bool equal(const unbounded_uint_type& lhs, integral_type rhs)
  {
    if (lhs.size() > 1)
      return false;
    return lhs[0] == rhs;
  }

  static bool less(const unbounded_uint_type& lhs, integral_type rhs)
  {
    if (lhs.size() > 1)
      return false;
    return lhs[0] < rhs;
  }

  static void add(unbounded_uint_type& lhs, integral_type rhs)
  {
    const integral_type carry =
      traits_type::ops_type::add_single_digit(
          lhs.digits(), lhs.digits(), lhs.size(), rhs);
    if (carry)
      lhs.push(carry);
  }

  static void subtract(unbounded_uint_type& lhs, integral_type rhs);

  static void multiply(unbounded_uint_type& lhs, integral_type rhs)
  {
    multiply(lhs, lhs, rhs);
  }

  static void multiply(unbounded_uint_type& z,
                       const unbounded_uint_type& x, integral_type y);

  static void divide  (unbounded_uint_type& lhs, integral_type rhs);
  static void modulo  (unbounded_uint_type& lhs, integral_type rhs);

  static void bitwise_or(unbounded_uint_type& lhs, integral_type rhs)
  {
    lhs[0] |= rhs;
  }

  static void bitwise_and(unbounded_uint_type& lhs, integral_type rhs)
  {
    lhs[0] &= rhs;
  }

  static void bitwise_xor(unbounded_uint_type& lhs, integral_type rhs)
  {
    lhs[0] ^= rhs;
  }
};


template<class UnboundedUInt>
void
unbounded_uint_integral_ops_impl<
  UnboundedUInt, typename UnboundedUInt::digit_type, false
>::subtract(unbounded_uint_type& lhs, integral_type rhs)
{
  if (!less(lhs, rhs))
  {
    if (lhs.size() == 1)
      lhs[0] -= rhs;
    else
    {
      traits_type::ops_type::subtract_single_digit(
          lhs.digits(), lhs.digits(), lhs.size(), rhs);
      lhs.clamp_high_digit();
    }
  }
  else
    throw std::runtime_error("unbounded_uint_integral_ops_impl::subtract: "
        "result is negative");
}

template<class UnboundedUInt>
void
unbounded_uint_integral_ops_impl<
  UnboundedUInt, typename UnboundedUInt::digit_type, false
>::multiply(unbounded_uint_type& z,
            const unbounded_uint_type& x, integral_type y)
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

template<class UnboundedUInt>
void
unbounded_uint_integral_ops_impl<
  UnboundedUInt, typename UnboundedUInt::digit_type, false
>::divide(unbounded_uint_type& lhs, integral_type rhs)
{
  if (rhs == 0)
    throw std::domain_error(
        "unbounded_uint_integral_ops_impl::divide: division by zero");

  if (rhs == 1 || !lhs)
    return;

  const bool is_power_of_two = (rhs & (rhs-1)) == 0;

  if (!is_power_of_two)
    traits_type::ops_type::divide_by_digit(
        lhs.digits(), lhs.digits(), lhs.size(), rhs);
  else
  {
    integral_type i = 2;
    while ((i < traits_type::radix_bits) && (rhs != integral_type(1) << i))
      ++i;

    traits_type::ops_type::shift_bits_right(lhs.digits(), lhs.size(), i);
  }

  lhs.clamp_high_digit();
}

template<class UnboundedUInt>
void
unbounded_uint_integral_ops_impl<
  UnboundedUInt, typename UnboundedUInt::digit_type, false
>::modulo(unbounded_uint_type& lhs, integral_type rhs)
{
  if (rhs == 0)
    throw std::domain_error(
        "unbounded_uint_integral_ops_impl::modulo: division by zero");

  if (rhs == 1 || !lhs)
  {
    assign(lhs, 0);
    return;
  }

  const bool is_power_of_two = (rhs & (rhs-1)) == 0;

  if (!is_power_of_two)
    lhs[0] =
      traits_type::ops_type::divide_by_digit(
          lhs.digits(), lhs.digits(), lhs.size(), rhs);
  else
  {
    integral_type i = 2;
    while ((i < traits_type::radix_bits) && (rhs != integral_type(1) << i))
      ++i;

    lhs[0] &= ((integral_type(1) << i) - 1);
  }

  lhs.set_size(1);
}



// 2)
// just casts to digit_type and dispatches to above specialization
template<class UnboundedUInt>
struct unbounded_uint_integral_ops_impl<
  UnboundedUInt,
  typename make_signed<typename UnboundedUInt::digit_type>::type,
  true
>
{
  typedef UnboundedUInt                             unbounded_uint_type;
  typedef typename make_signed<
    typename unbounded_uint_type::digit_type
  >::type                                           integral_type;

  typedef typename unbounded_uint_type::digit_type  unsigned_integral_type;

  typedef unbounded_uint_integral_ops_impl<
    unbounded_uint_type,
    unsigned_integral_type
  > impl_type;

  static unsigned_integral_type promote(integral_type rhs)
  {
    // TODO if (rhs < 0)
    //  throw integral_promotion_error();
    return static_cast<unsigned_integral_type>(rhs);
  }

  static void assign(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::assign(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static bool equal(const unbounded_uint_type& lhs, integral_type rhs)
  {
    return impl_type::equal(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static bool less(const unbounded_uint_type& lhs, integral_type rhs)
  {
    return impl_type::less(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void add(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::add(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void subtract(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::subtract(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void multiply(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::multiply(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void multiply(unbounded_uint_type& z,
                       const unbounded_uint_type& x, integral_type y)
  {
    impl_type::multiply(z, x, static_cast<unsigned_integral_type>(y));
  }

  static void divide(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::divide(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void modulo(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::modulo(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void bitwise_or(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_or(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void bitwise_and(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_and(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void bitwise_xor(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_xor(lhs, static_cast<unsigned_integral_type>(rhs));
  }
};


// 3
template<class UnboundedUInt, typename IntegralT>
struct unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, false
>
{
  typedef UnboundedUInt                             unbounded_uint_type;
  typedef IntegralT                                 integral_type;
  typedef std::numeric_limits<integral_type>        integral_type_limits;
  typedef typename unbounded_uint_type::traits_type traits_type;

  static const unsigned radix_bits      = traits_type::radix_bits;
  static const unsigned max_digit_value = traits_type::max_digit_value;

  static const unsigned q =
    (integral_type_limits::digits + (radix_bits - 1))
    / radix_bits;

  static void assign  (unbounded_uint_type& lhs, integral_type rhs);
  static bool equal   (const unbounded_uint_type& lhs, integral_type rhs);
  static bool less    (const unbounded_uint_type& lhs, integral_type rhs);
  static void add     (unbounded_uint_type& lhs, integral_type rhs);
  static void subtract(unbounded_uint_type& lhs, integral_type rhs);
  static void multiply(unbounded_uint_type& lhs, integral_type rhs);
  static void multiply(unbounded_uint_type& z,
                       const unbounded_uint_type& x, integral_type y);
  static void divide  (unbounded_uint_type& lhs, integral_type rhs);
  static void modulo  (unbounded_uint_type& lhs, integral_type rhs);

  static void bitwise_or (unbounded_uint_type& lhs, integral_type rhs);
  static void bitwise_and(unbounded_uint_type& lhs, integral_type rhs);
  static void bitwise_xor(unbounded_uint_type& lhs, integral_type rhs);

  typedef typename traits_type::digit_type digit_type;
};


template<class UnboundedUInt, typename IntegralT>
const unsigned unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, false>::q;

template<class UnboundedUInt, typename IntegralT>
void
unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, false>::assign(unbounded_uint_type& lhs,
                                           integral_type rhs)
{
  if (rhs <= max_digit_value)
  {
    lhs[0] = static_cast<digit_type>(rhs);
    lhs.set_size(1);
  }
  else
  {
    from_integral_converter<
      unbounded_uint_type, integral_type
    >::convert(lhs, rhs);
  }
}

template<class UnboundedUInt, typename IntegralT>
bool
unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, false>::equal(const unbounded_uint_type& lhs,
                                          integral_type rhs)
{
  if (lhs.size() <= q)
  {
    typedef to_integral_converter<
      unbounded_uint_type, integral_type> converter_type;

    converter_type c;

    // TODO it's faster to convert from integral type but comparison is faster
    // between integral types, measure which one is better!
    const integral_type x = c.convert(lhs);

    return x == rhs;
  }

  return false;
}

template<class UnboundedUInt, typename IntegralT>
bool
unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, false>::less(const unbounded_uint_type& lhs,
                                         integral_type rhs)
{
  // TODO reroute to 1) if rhs <= digit_max
  if (lhs.size() <= q)
  {
    typedef to_integral_converter<
      unbounded_uint_type, integral_type> converter_type;

    converter_type c;

    const integral_type x = c.convert(lhs);

    return x < rhs;
  }

  return false;
}

template<class UnboundedUInt, typename IntegralT>
void
unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, false>::add(unbounded_uint_type& lhs,
                                        integral_type rhs)
{
  if (rhs <= max_digit_value)
  {
    unbounded_uint_integral_ops_impl<
      UnboundedUInt, digit_type
    >::add(lhs, static_cast<digit_type>(rhs));
  }
  else
  {
    digit_type tmp_digits[q];

    unbounded_uint_type tmp(tmp_digits, q, q);

    from_integral_converter<
      unbounded_uint_type, integral_type
    >::convert(tmp, rhs);

    lhs += tmp;
  }
}

template<class UnboundedUInt, typename IntegralT>
void
unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, false>::subtract(unbounded_uint_type& lhs,
                                             integral_type rhs)
{
  if (rhs <= max_digit_value)
  {
    unbounded_uint_integral_ops_impl<
      UnboundedUInt, digit_type
    >::subtract(lhs, static_cast<digit_type>(rhs));
  }
  else
  {
    digit_type tmp_digits[q];

    unbounded_uint_type tmp(tmp_digits, q, q);

    from_integral_converter<
      unbounded_uint_type, integral_type
    >::convert(tmp, rhs);

    lhs -= tmp;
  }
}

template<class UnboundedUInt, typename IntegralT>
void
unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, false>::multiply(unbounded_uint_type& lhs,
                                             integral_type rhs)
{
  if (rhs <= max_digit_value)
  {
    unbounded_uint_integral_ops_impl<
      UnboundedUInt, digit_type
    >::multiply(lhs, static_cast<digit_type>(rhs));
  }
  else
  {
    digit_type tmp_digits[q];

    unbounded_uint_type tmp(tmp_digits, q, q);

    from_integral_converter<
      unbounded_uint_type, integral_type
    >::convert(tmp, rhs);

    unbounded_uint_type *a, *b;
    if (lhs.size() > tmp.size())
    {
      a = &lhs;
      b = &tmp;
    }
    else
    {
      a = &tmp;
      b = &lhs;
    }

    digit_type workspace[q];
    traits_type::ops_type::comba_mul(lhs.digits(),
                                     a->digits(), a->size(),
                                     b->digits(), b->size(), workspace);
    lhs.set_size(lhs.size() + tmp.size());
    lhs.clamp_high_digit();
  }
}

template<class UnboundedUInt, typename IntegralT>
void
unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, false>::multiply(unbounded_uint_type& z,
                                             const unbounded_uint_type& x,
                                             integral_type y)
{
  if (y <= max_digit_value)
  {
    unbounded_uint_integral_ops_impl<
      UnboundedUInt, digit_type
    >::multiply(z, x, static_cast<digit_type>(y));
  }
  else
  {
    digit_type tmp_digits[q];

    unbounded_uint_type tmp(tmp_digits, q, q);

    from_integral_converter<
      unbounded_uint_type, integral_type
    >::convert(tmp, y);

    unbounded_uint_type *a, *b;
    if (x.size() > tmp.size())
    {
      a = &x;
      b = &tmp;
    }
    else
    {
      a = &tmp;
      b = &x;
    }

    digit_type workspace[q];
    traits_type::ops_type::comba_mul(z.digits(), a->digits(), a->size(),
                                                 b->digits(), b->size());
    z.set_size(x.size() + tmp.size());
    z.clamp_high_digit();
  }
}

template<class UnboundedUInt, typename IntegralT>
void
unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, false>::divide(unbounded_uint_type& lhs,
                                           integral_type rhs)
{
  if (rhs <= max_digit_value)
  {
    unbounded_uint_integral_ops_impl<
      UnboundedUInt, digit_type
    >::divide(lhs, static_cast<digit_type>(rhs));
  }
  else
  {
    digit_type tmp_digits[q];

    unbounded_uint_type tmp(tmp_digits, q, q);

    from_integral_converter<
      unbounded_uint_type, integral_type
    >::convert(tmp, rhs);

    digit_type remainder_digits[q] = {0};

    unbounded_uint_type remainder(remainder_digits, q, q);

    //divide(lhs, &remainder, lhs, tmp);

    remainder.clamp();

    typedef to_integral_converter<
      unbounded_uint_type, integral_type> converter_type;

    converter_type c;

    assign(lhs, c.convert_without_check(remainder, remainder.precision()));
  }
}

template<class UnboundedUInt, typename IntegralT>
void
unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, false>::modulo(unbounded_uint_type& lhs,
                                           integral_type rhs)
{
  if (rhs <= max_digit_value)
  {
    unbounded_uint_integral_ops_impl<
      UnboundedUInt, digit_type
    >::divide(lhs, static_cast<digit_type>(rhs));
  }
  else
  {
    digit_type tmp_digits[q];

    unbounded_uint_type tmp(tmp_digits, q, q);

    from_integral_converter<
      unbounded_uint_type, integral_type
    >::convert(tmp, rhs);

    digit_type remainder_digits[q] = {0};

    unbounded_uint_type remainder(remainder_digits, q, q);

    //divide(&lhs, &remainder, lhs, tmp);

    remainder.clamp();

    typedef to_integral_converter<
      unbounded_uint_type, integral_type> converter_type;

    converter_type c;

    assign(lhs, c.convert_without_check(remainder, remainder.precision()));
  }
}

// TODO first need to write
// divide(unbounded_uint_type* q, unbounded_uint_type* r,
//        const unbounded_uint_type& x, const unbounded_uint_type& y);
// or don't implement this function if it is not possible to do so without the
// use of temporary memory.


template<class UnboundedUInt, typename IntegralT>
void
unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, false>::bitwise_or(unbounded_uint_type& lhs,
                                               integral_type rhs)
{
  if (rhs <= max_digit_value)
  {
    unbounded_uint_integral_ops_impl<
      UnboundedUInt, digit_type
    >::bitwise_or(lhs, static_cast<digit_type>(rhs));
  }
  else
  {
    digit_type tmp_digits[q];

    unbounded_uint_type tmp(tmp_digits, q, q);

    from_integral_converter<
      unbounded_uint_type, integral_type
    >::convert(tmp, rhs);

    lhs |= tmp;
  }
}

template<class UnboundedUInt, typename IntegralT>
void
unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, false>::bitwise_and(unbounded_uint_type& lhs,
                                                integral_type rhs)
{
  if (rhs <= max_digit_value)
  {
    unbounded_uint_integral_ops_impl<
      UnboundedUInt, digit_type
    >::bitwise_and(lhs, static_cast<digit_type>(rhs));
  }
  else
  {
    digit_type tmp_digits[q];

    unbounded_uint_type tmp(tmp_digits, q, q);

    from_integral_converter<
      unbounded_uint_type, integral_type
    >::convert(tmp, rhs);

    lhs &= tmp;
  }
}

template<class UnboundedUInt, typename IntegralT>
void
unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, false>::bitwise_xor(unbounded_uint_type& lhs,
                                                integral_type rhs)
{
  if (rhs <= max_digit_value)
  {
    unbounded_uint_integral_ops_impl<
      UnboundedUInt, digit_type
    >::bitwise_xor(lhs, static_cast<digit_type>(rhs));
  }
  else
  {
    digit_type tmp_digits[q];

    unbounded_uint_type tmp(tmp_digits, q, q);

    from_integral_converter<
      unbounded_uint_type, integral_type
    >::convert(tmp, rhs);

    lhs ^= tmp;
  }
}


// 4
template<class UnboundedUInt, typename IntegralT>
struct unbounded_uint_integral_ops_impl<
  UnboundedUInt, IntegralT, true
>
{
  typedef UnboundedUInt unbounded_uint_type;
  typedef IntegralT     integral_type;

  typedef typename make_unsigned<integral_type>::type unsigned_integral_type;

  // this is spec 3
  typedef unbounded_uint_integral_ops_impl<
    unbounded_uint_type,
    unsigned_integral_type
  > impl_type;

  static void assign(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::assign(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static bool equal(const unbounded_uint_type& lhs, integral_type rhs)
  {
    return impl_type::equal(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static bool less(const unbounded_uint_type& lhs, integral_type rhs)
  {
    return impl_type::less(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void add(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::add(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void subtract(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::subtract(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void multiply(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::multiply(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void multiply(unbounded_uint_type& z,
                       const unbounded_uint_type& x, integral_type y)
  {
    impl_type::multiply(z, x, static_cast<unsigned_integral_type>(y));
  }

  static void divide(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::divide(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void modulo(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::modulo(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void bitwise_or(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_or(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void bitwise_and(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_and(lhs, static_cast<unsigned_integral_type>(rhs));
  }

  static void bitwise_xor(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_xor(lhs, static_cast<unsigned_integral_type>(rhs));
  }
};




////////////////////////////////////////////////////////////////////

template<
  class UnboundedUInt,
  typename IntegralT,
  bool less_equal =
      std::numeric_limits<IntegralT>::is_signed ?
      (
        std::numeric_limits<IntegralT>::digits <=
        std::numeric_limits<
          typename make_signed<typename UnboundedUInt::digit_type>::type
        >::digits
      ):
      (
        std::numeric_limits<IntegralT>::digits <=
        std::numeric_limits<typename UnboundedUInt::digit_type>::digits
      )
>
struct unbounded_uint_integral_ops;


// Dispatches integral types that fit into a digit_type by casting it to
// digit_type or signed digit_type and using the specialization at the top.
template<
  class UnboundedUInt,
  typename IntegralT
>
struct unbounded_uint_integral_ops<UnboundedUInt,IntegralT,true>
{
  BOOST_STATIC_ASSERT(is_integral<IntegralT>::value);

  typedef UnboundedUInt unbounded_uint_type;
  typedef IntegralT     integral_type;

  typedef unbounded_uint_integral_ops_impl<
    unbounded_uint_type,
    typename mpl::if_<
      is_signed<integral_type>,
      typename make_signed<
        typename unbounded_uint_type::digit_type
      >::type,
      typename unbounded_uint_type::digit_type
    >::type
  >                     impl_type;

  typedef typename impl_type::integral_type  to_integral_type;

  static void assign(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::assign(lhs, static_cast<to_integral_type>(rhs));
  }

  static bool equal(const unbounded_uint_type& lhs, integral_type rhs)
  {
    return impl_type::equal(lhs, static_cast<to_integral_type>(rhs));
  }

  static bool less(const unbounded_uint_type& lhs, integral_type rhs)
  {
    return impl_type::less(lhs, static_cast<to_integral_type>(rhs));
  }

  static void add(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::add(lhs, static_cast<to_integral_type>(rhs));
  }

  static void subtract(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::subtract(lhs, static_cast<to_integral_type>(rhs));
  }

  static void multiply(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::multiply(lhs, static_cast<to_integral_type>(rhs));
  }

  static void multiply(unbounded_uint_type& z,
                       const unbounded_uint_type& x, integral_type y)
  {
    impl_type::multiply(z, x, static_cast<to_integral_type>(y));
  }

  static void divide(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::divide(lhs, static_cast<to_integral_type>(rhs));
  }

  static void modulo(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::modulo(lhs, static_cast<to_integral_type>(rhs));
  }

  static void bitwise_or(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_or(lhs, static_cast<to_integral_type>(rhs));
  }

  static void bitwise_and(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_and(lhs, static_cast<to_integral_type>(rhs));
  }

  static void bitwise_xor(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_xor(lhs, static_cast<to_integral_type>(rhs));
  }
};


template<
  class UnboundedUInt,
  typename IntegralT
>
struct unbounded_uint_integral_ops<UnboundedUInt,IntegralT,false>
{
  BOOST_STATIC_ASSERT(is_integral<IntegralT>::value);

  typedef UnboundedUInt unbounded_uint_type;
  typedef IntegralT     integral_type;

  typedef unbounded_uint_integral_ops_impl<
      unbounded_uint_type, integral_type
  >                     impl_type;

  static void assign(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::assign(lhs, rhs);
  }

  static bool equal(const unbounded_uint_type& lhs, integral_type rhs)
  {
    return impl_type::equal(lhs, rhs);
  }

  static bool less(const unbounded_uint_type& lhs, integral_type rhs)
  {
    return impl_type::less(lhs, rhs);
  }

  static void add(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::add(lhs, rhs);
  }

  static void subtract(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::subtract(lhs, rhs);
  }

  static void multiply(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::multiply(lhs, rhs);
  }

  static void multiply(unbounded_uint_type& z,
                       const unbounded_uint_type& x, integral_type y)
  {
    impl_type::multiply(z, x, y);
  }

  static void divide(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::divide(lhs, rhs);
  }

  static void modulo(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::modulo(lhs, rhs);
  }

  static void bitwise_or(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_or(lhs, rhs);
  }

  static void bitwise_and(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_and(lhs, rhs);
  }

  static void bitwise_xor(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_xor(lhs, rhs);
  }
};



} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost

#endif


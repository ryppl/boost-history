// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_UNBOUNDED_INT_INTEGRAL_OPS_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_UNBOUNDED_INT_INTEGRAL_OPS_HPP

#include <boost/mp_math/integer/detail/base/unbounded_int_integral.hpp>
#include <boost/mp_math/integer/detail/unbounded_uint_integral.hpp>
#include <boost/mp_math/integer/unbounded_uint.hpp>

// Here we optimize interaction with built in integral types.
// This code is even hairier and more subtle.

namespace boost {
namespace mp_math {
namespace detail {

template<
  class UnboundedInt,
  typename IntegralT,
  bool is_signed = std::numeric_limits<IntegralT>::is_signed
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

  typedef base::unbounded_int_integral_ops<
    unbounded_int_type, integral_type
  >                                                base_type;

  typedef unbounded_uint_integral_ops<
    unbounded_int_type, integral_type
  >                                                magnitude_type_integral_ops;

  static void assign(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs.reserve(1);
    base_type::assign(lhs, rhs);
  }

  static bool equal(const unbounded_int_type& lhs, integral_type rhs)
  {
    return base_type::equal(lhs, rhs);
  }

  static bool less(const unbounded_int_type& lhs, integral_type rhs)
  {
    return base_type::less(lhs, rhs);
  }

  static void add(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs.reserve(lhs.size() + 1);
    base_type::add(lhs, rhs);
  }

  static void subtract(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs.reserve(lhs.size() + 1);
    base_type::subtract(lhs, rhs);
  }

  static void multiply(unbounded_int_type& lhs, integral_type rhs);

  static void multiply(unbounded_int_type& z,
                       const unbounded_int_type& x, integral_type y);

  static void divide(unbounded_int_type& lhs, integral_type rhs)
  {
    base_type::divide(lhs, rhs);
  }

  static void modulo(unbounded_int_type& lhs, integral_type rhs)
  {
    base_type::modulo(lhs, rhs);
  }

  static void bitwise_or(unbounded_int_type& lhs, integral_type rhs)
  {
    base_type::bitwise_or(lhs, rhs);
  }

  static void bitwise_and(unbounded_int_type& lhs, integral_type rhs)
  {
    base_type::bitwise_and(lhs, rhs);
  }

  static void bitwise_xor(unbounded_int_type& lhs, integral_type rhs)
  {
    base_type::bitwise_xor(lhs, rhs);
  }
};


template<class UnboundedInt>
void
unbounded_int_integral_ops_impl<
  UnboundedInt, typename UnboundedInt::digit_type, false
>::multiply(unbounded_int_type& lhs, integral_type rhs)
{
  if (rhs == 0)
  {
    assign(lhs, integral_type(0));
    return;
  }
  else if (rhs == 1)
    return;

  lhs.reserve(lhs.size() + 1);

  const integral_type carry =
    traits_type::ops_type::multiply_by_digit(
        lhs.digits(), lhs.digits(), lhs.size(), rhs);

  if (carry)
    lhs.push(carry);
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
  {
    z = x;
    return;
  }

  z.reserve(x.size() + 1);

  const integral_type carry =
    traits_type::ops_type::multiply_by_digit(
        z.digits(), x.digits(), x.size(), y);

  z.set_size(x.size());
  z.set_sign_bit(x.sign_bit());

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
  typedef UnboundedInt                            unbounded_int_type;
  typedef typename make_signed<
    typename unbounded_int_type::digit_type
  >::type                                         integral_type;

  typedef typename unbounded_int_type::digit_type unsigned_integral_type;

  typedef base::unbounded_int_integral_ops<
    unbounded_int_type,
    integral_type
  >                                               base_integral_ops_type;

  static bool get_sign_bit(integral_type x)
  {
    if (x >= 0)
      return 0;
    else
      return 1;
  }

  static unsigned_integral_type get_absolute(integral_type x)
  {
    if (x >= 0)
      return static_cast<unsigned_integral_type>(x);
    else
      return static_cast<unsigned_integral_type>(-x);
  }

  static void assign(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs.reserve(1);
    base_integral_ops_type::assign(lhs, rhs);
  }

  static bool equal(const unbounded_int_type& lhs, integral_type rhs)
  {
    return base_integral_ops_type::equal(lhs, rhs);
  }

  static bool less(const unbounded_int_type& lhs, integral_type rhs)
  {
    return base_integral_ops_type::less(lhs, rhs);
  }

  static void add(unbounded_int_type& lhs, integral_type rhs)
  {
    if (rhs >= 0)
    {
      lhs.reserve(lhs.size() + 1);
      base::unbounded_int_integral_ops<
        unbounded_int_type,
        unsigned_integral_type
      >::add(lhs, static_cast<unsigned_integral_type>(rhs));
    }
    else
      base::unbounded_int_integral_ops<
        unbounded_int_type,
        unsigned_integral_type
      >::subtract(lhs, static_cast<unsigned_integral_type>(-rhs));
  }

  static void subtract(unbounded_int_type& lhs, integral_type rhs)
  {
    if (rhs >= 0)
      base_integral_ops_type::subtract(lhs, static_cast<unsigned_integral_type>(rhs));
    else
      base_integral_ops_type::add(lhs, static_cast<unsigned_integral_type>(-rhs));
  }

  static void multiply(unbounded_int_type& lhs, integral_type rhs)
  {
    unbounded_int_integral_ops_impl<
      unbounded_int_type,
      unsigned_integral_type
    >::multiply(lhs, get_absolute(rhs));
    lhs.set_sign_bit(lhs.sign_bit() ^ get_sign_bit(rhs));
  }

  static void multiply(unbounded_int_type& z,
                       const unbounded_int_type& x, integral_type y)
  {
    unbounded_int_integral_ops_impl<
      unbounded_int_type,
      unsigned_integral_type
    >::multiply(z, x, get_absolute(y));
    z.set_sign_bit(x.sign_bit() ^ get_sign_bit(y));
  }

  static void divide(unbounded_int_type& lhs, integral_type rhs)
  {
    base_integral_ops_type::divide(lhs, rhs);
  }

  static void modulo(unbounded_int_type& lhs, integral_type rhs)
  {
    base_integral_ops_type::modulo(lhs, rhs);
  }

  static void bitwise_or(unbounded_int_type& lhs, integral_type rhs)
  {
    base_integral_ops_type::bitwise_or(lhs, rhs);
  }

  static void bitwise_and(unbounded_int_type& lhs, integral_type rhs)
  {
    base_integral_ops_type::bitwise_and(lhs, rhs);
  }

  static void bitwise_xor(unbounded_int_type& lhs, integral_type rhs)
  {
    base_integral_ops_type::bitwise_xor(lhs, rhs);
  }
};


// 3
template<class UnboundedInt, typename IntegralT>
struct unbounded_int_integral_ops_impl<
  UnboundedInt, IntegralT, false
>
{
  typedef UnboundedInt                             unbounded_int_type;
  typedef typename unbounded_int_type::traits_type traits_type;
  typedef IntegralT                                integral_type;
  typedef std::numeric_limits<integral_type>       integral_type_limits;

  typedef base::unbounded_int_integral_ops<
    unbounded_int_type, integral_type
  >                                          base_unbounded_int_integral_ops_type;

  typedef base::unbounded_uint_integral_ops<
    unbounded_int_type, integral_type
  >                                          magnitude_type_integral_ops;

  static const unsigned radix_bits      = traits_type::radix_bits;
  static const unsigned max_digit_value = traits_type::max_digit_value;

  static const unsigned q =
    (integral_type_limits::digits + (radix_bits - 1))
    / radix_bits;

  static void assign(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs.reserve(q);
    magnitude_type_integral_ops::assign(lhs, rhs);
    lhs.set_sign_bit(0);
  }

  static bool equal(const unbounded_int_type& lhs, integral_type rhs)
  {
    return base_unbounded_int_integral_ops_type::equal(lhs, rhs);
  }

  static bool less(const unbounded_int_type& lhs, integral_type rhs)
  {
    return base_unbounded_int_integral_ops_type::less(lhs, rhs);
  }

  static void add(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs.reserve(std::max(lhs.size(), q) + 1);
    base_unbounded_int_integral_ops_type::add(lhs, rhs);
  }

  static void subtract(unbounded_int_type& lhs, integral_type rhs)
  {
    base_unbounded_int_integral_ops_type::subtract(lhs, rhs);
  }

  static void multiply(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs.reserve(lhs.size() + q);
    base_unbounded_int_integral_ops_type::multiply(lhs, rhs);
  }

  static void multiply(unbounded_int_type& z,
                       const unbounded_int_type& x, integral_type y)
  {
    z.reserve(x.size() + q);
    base_unbounded_int_integral_ops_type::multiply(z, x, y);
  }

  static void divide(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs /= unbounded_int_type(rhs);
  }

  static void modulo(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs %= unbounded_int_type(rhs);
  }

  static void bitwise_or(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs.reserve(q);
    base_unbounded_int_integral_ops_type::bitwise_or(lhs, rhs);
  }

  static void bitwise_and(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs.reserve(q);
    base_unbounded_int_integral_ops_type::bitwise_and(lhs, rhs);
  }

  static void bitwise_xor(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs.reserve(q);
    base_unbounded_int_integral_ops_type::bitwise_xor(lhs, rhs);
  }
};


template<class UnboundedInt, typename IntegralT>
const unsigned unbounded_int_integral_ops_impl<
  UnboundedInt, IntegralT, false>::q;


// 4)
template<class UnboundedInt, typename IntegralT>
struct unbounded_int_integral_ops_impl<
  UnboundedInt, IntegralT, true
>
{
  typedef UnboundedInt                             unbounded_int_type;
  typedef typename unbounded_int_type::traits_type traits_type;
  typedef IntegralT                                integral_type;
  typedef typename make_unsigned<
    integral_type
  >::type                                    unsigned_integral_type;

  typedef base::unbounded_int<traits_type>   unbounded_int_base_type;

  typedef unbounded_uint_integral_ops<
    unbounded_int_type,
    unsigned_integral_type
  >                                          unbounded_uint_integral_ops_type;

  typedef base::unbounded_int_integral_ops<
    unbounded_int_type,
    integral_type
  >                                          base_unbounded_int_integral_ops_type;

  typedef std::numeric_limits<integral_type> integral_type_limits;
  static const unsigned radix_bits      = traits_type::radix_bits;
  static const unsigned max_digit_value = traits_type::max_digit_value;

  static const unsigned q =
    (integral_type_limits::digits + (radix_bits - 1))
    / radix_bits;

  static bool get_sign_bit(integral_type x)
  {
    if (x >= 0)
      return 0;
    else
      return 1;
  }

  static unsigned_integral_type get_absolute(integral_type x)
  {
    if (x >= 0)
      return static_cast<unsigned_integral_type>(x);
    else
      return static_cast<unsigned_integral_type>(-x);
  }

  static void assign(unbounded_int_type& lhs, integral_type rhs)
  {
    unbounded_uint_integral_ops_type::assign(lhs, get_absolute(rhs));
    lhs.set_sign_bit(get_sign_bit(rhs));
  }

  static bool equal(const unbounded_int_type& lhs, integral_type rhs)
  {
    return base::unbounded_int_integral_ops<
      unbounded_int_type, integral_type>::equal(lhs, rhs);
  }

  static bool less(const unbounded_int_type& lhs, integral_type rhs)
  {
    return base::unbounded_int_integral_ops<
      unbounded_int_type, integral_type>::less(lhs, rhs);
  }

  static void add(unbounded_int_type& lhs, integral_type rhs)
  {
    if (lhs.sign_bit() == get_sign_bit(rhs))
      unbounded_uint_integral_ops_type::add(lhs, get_absolute(rhs));
    else
      subtract_smaller(lhs, get_absolute(rhs), get_sign_bit(rhs));
  }

  static void subtract(unbounded_int_type& lhs, integral_type rhs)
  {
    if (lhs.sign_bit() != get_sign_bit(rhs))
      unbounded_uint_integral_ops_type::add(lhs, get_absolute(rhs));
    else
      subtract_smaller(lhs, get_absolute(rhs), ~lhs.sign_bit());
  }

  static void multiply(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs.reserve(lhs.size() + q);
    base_unbounded_int_integral_ops_type::multiply(lhs, rhs);
  }

  static void multiply(unbounded_int_type& z,
                       const unbounded_int_type& x, integral_type y)
  {
    z.reserve(x.size() + q);
    base_unbounded_int_integral_ops_type::multiply(z, x, y);
  }

  static void divide(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs /= unbounded_int_type(rhs);
    //unbounded_uint_integral_ops_type::divide(lhs, get_absolute(rhs));
    //lhs.set_sign_bit(lhs.sign_bit() ^ get_sign_bit(rhs));
  }

  static void modulo(unbounded_int_type& lhs, integral_type rhs)
  {
    lhs %= unbounded_int_type(rhs);
  }

  static void bitwise_or(unbounded_int_type& lhs, integral_type rhs)
  {
    unbounded_int_integral_ops_impl<
      UnboundedInt, unsigned_integral_type
    >::bitwise_or(lhs, get_absolute(rhs));
    lhs.set_sign_bit(lhs.sign_bit() | get_sign_bit(rhs));
  }

  static void bitwise_and(unbounded_int_type& lhs, integral_type rhs)
  {
    unbounded_int_integral_ops_impl<
      UnboundedInt, unsigned_integral_type
    >::bitwise_and(lhs, get_absolute(rhs));
    lhs.set_sign_bit(lhs.sign_bit() & get_sign_bit(rhs));
  }

  static void bitwise_xor(unbounded_int_type& lhs, integral_type rhs)
  {
    unbounded_int_integral_ops_impl<
      UnboundedInt, unsigned_integral_type
    >::bitwise_xor(lhs, get_absolute(rhs));
    lhs.set_sign_bit(lhs.sign_bit() ^ get_sign_bit(rhs));
  }

  static void subtract_smaller(unbounded_int_type& lhs,
                               unsigned_integral_type rhs,
                               bool final_sign);
};


template<class UnboundedInt, typename IntegralT>
const unsigned unbounded_int_integral_ops_impl<
  UnboundedInt, IntegralT, true>::q;


template<class UnboundedInt, typename IntegralT>
void
unbounded_int_integral_ops_impl<
  UnboundedInt, IntegralT, true
>::subtract_smaller(unbounded_int_type& lhs,
                    unsigned_integral_type rhs,
                    bool final_sign)
{
  static const unsigned radix_bits = traits_type::radix_bits;

  static const unsigned q =
    (std::numeric_limits<integral_type>::digits + (radix_bits - 1))
    / radix_bits;

  typedef base::unbounded_uint<traits_type> unbounded_uint_base_type;

  typename traits_type::digit_type tmp_digits[q];

  unbounded_uint_base_type tmp(tmp_digits, q, q);

  base::from_integral_converter<
    unbounded_uint_base_type, unsigned_integral_type
  >::convert(tmp, rhs);

  const unbounded_uint_base_type* x;
  const unbounded_uint_base_type* y;

  if (static_cast<const unbounded_uint_base_type&>(lhs) >= tmp)
  {
    x = &lhs;
    y = &tmp;
  }
  else
  {
    x = &tmp;
    y = &lhs;
    lhs.reserve(tmp.size());
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


template<
  class UnboundedInt,
  typename IntegralT,
  bool less_equal =
      std::numeric_limits<IntegralT>::is_signed ?
      (
        std::numeric_limits<IntegralT>::digits <=
        std::numeric_limits<
          typename make_signed<
            typename UnboundedInt::traits_type::digit_type
          >::type
        >::digits
      ):
      (
        std::numeric_limits<IntegralT>::digits <=
        std::numeric_limits<
          typename UnboundedInt::traits_type::digit_type
        >::digits
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

  typedef UnboundedInt                             unbounded_int_type;
  typedef IntegralT                                integral_type;
  typedef typename unbounded_int_type::traits_type traits_type;

  typedef typename mpl::if_<
    is_signed<IntegralT>,
    unbounded_int_integral_ops_impl<
      unbounded_int_type,
      typename make_signed<
        typename unbounded_int_type::digit_type
      >::type
    >,
    unbounded_int_integral_ops_impl<
      unbounded_int_type,
      typename unbounded_int_type::digit_type
    >
  >::type                                          impl_type;

  typedef typename impl_type::integral_type        to_integral_type;

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

  typedef UnboundedInt                             unbounded_int_type;
  typedef IntegralT                                integral_type;
  typedef typename unbounded_int_type::traits_type traits_type;

  typedef unbounded_int_integral_ops_impl<
      unbounded_int_type, integral_type
  >                                                impl_type;

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






} // namespace detail
} // namespace mp_math
} // namespace boost

#endif


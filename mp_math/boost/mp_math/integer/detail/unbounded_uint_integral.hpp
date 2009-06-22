// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_UNBOUNDED_UINT_INTEGRAL_OPS_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_UNBOUNDED_UINT_INTEGRAL_OPS_HPP

#include <boost/mp_math/integer/detail/base/unbounded_uint_integral.hpp>

// Here we optimize interaction with built in integral types.
// This code is even hairier and more subtle.

namespace boost {
namespace mp_math {
namespace detail {

// Generally we will try to dispatch calls to unbounded_uint_integral_ops
// possibly with a call to reserve() before that. If more optimization is
// possible by calling reserve() only at certain points inside a function then
// we reimplement the functionality of the corresponding
// unbounded_uint_integral_ops function here. That means in any case the
// functions closely correlate and improvements should be applied to both.

template<
  class UnboundedUint,
  typename IntegralT,
  bool is_signed = std::numeric_limits<IntegralT>::is_signed
>
struct unbounded_uint_integral_ops_impl;


// 1)
template<class UnboundedUint>
struct unbounded_uint_integral_ops_impl<
  UnboundedUint,
  typename UnboundedUint::digit_type,
  false
>
{
  typedef UnboundedUint                             unbounded_uint_type;
  typedef typename unbounded_uint_type::traits_type traits_type;
  typedef typename unbounded_uint_type::digit_type  integral_type;

  typedef base::unbounded_uint_integral_ops<
    base::unbounded_uint<traits_type>,
    integral_type
  >                                                 base_type;

  static void assign(unbounded_uint_type& lhs, integral_type rhs)
  {
    lhs.reserve(1);
    base_type::assign(lhs, rhs);
  }

  static bool equal(const unbounded_uint_type& lhs, integral_type rhs)
  {
    return base_type::equal(lhs, rhs);
  }

  static bool less(const unbounded_uint_type& lhs, integral_type rhs)
  {
    return base_type::less(lhs, rhs);
  }

  static void add(unbounded_uint_type& lhs, integral_type rhs)
  {
    lhs.reserve(lhs.size() + 1);
    base_type::add(lhs, rhs);
  }

  static void subtract(unbounded_uint_type& lhs, integral_type rhs)
  {
    base_type::subtract(lhs, rhs);
  }

  static void multiply(unbounded_uint_type& lhs, integral_type rhs);

  static void multiply(unbounded_uint_type& z,
                       const unbounded_uint_type& x, integral_type y);

  static void divide(unbounded_uint_type& lhs, integral_type rhs)
  {
    base_type::divide(lhs, rhs);
  }

  static void modulo(unbounded_uint_type& lhs, integral_type rhs)
  {
    base_type::modulo(lhs, rhs);
  }

  static void bitwise_or(unbounded_uint_type& lhs, integral_type rhs)
  {
    base_type::bitwise_or(lhs, rhs);
  }

  static void bitwise_and(unbounded_uint_type& lhs, integral_type rhs)
  {
    base_type::bitwise_and(lhs, rhs);
  }

  static void bitwise_xor(unbounded_uint_type& lhs, integral_type rhs)
  {
    base_type::bitwise_xor(lhs, rhs);
  }
};


template<class UnboundedUint>
void
unbounded_uint_integral_ops_impl<
  UnboundedUint, typename UnboundedUint::digit_type, false
>::multiply(unbounded_uint_type& lhs, integral_type rhs)
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

template<class UnboundedUint>
void
unbounded_uint_integral_ops_impl<
  UnboundedUint, typename UnboundedUint::digit_type, false
>::multiply(unbounded_uint_type& z,
            const unbounded_uint_type& x, integral_type y)
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

  if (carry)
    z.push(carry);
}


// 2)
// just casts to digit_type and dispatches to above specialization
template<class UnboundedUint>
struct unbounded_uint_integral_ops_impl<
  UnboundedUint,
  typename make_signed<typename UnboundedUint::digit_type>::type,
  true
>
{
  typedef UnboundedUint                             unbounded_uint_type;
  typedef typename unbounded_uint_type::traits_type traits_type;
  typedef typename make_signed<
    typename unbounded_uint_type::digit_type
  >::type                                            integral_type;

  typedef typename unbounded_uint_type::digit_type   unsigned_integral_type;
  typedef unbounded_uint_integral_ops_impl<
    unbounded_uint_type,
    unsigned_integral_type
  >                                                  impl_type;

  static unsigned_integral_type get_absolute(integral_type x)
  {
    if (x >= 0)
      return static_cast<unsigned_integral_type>(x);
    else
      return static_cast<unsigned_integral_type>(-x);
  }

  static void assign(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::assign(lhs, get_absolute(rhs));
  }

  static bool equal(const unbounded_uint_type& lhs, integral_type rhs)
  {
    return impl_type::equal(lhs, get_absolute(rhs));
  }

  static bool less(const unbounded_uint_type& lhs, integral_type rhs)
  {
    return impl_type::less(lhs, get_absolute(rhs));
  }

  static void add(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::add(lhs, get_absolute(rhs));
  }

  static void subtract(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::subtract(lhs, get_absolute(rhs));
  }

  static void multiply(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::multiply(lhs, get_absolute(rhs));
  }

  static void multiply(unbounded_uint_type& z,
                       const unbounded_uint_type& x, integral_type y)
  {
    impl_type::multiply(z, x, get_absolute(y));
  }

  static void divide(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::divide(lhs, get_absolute(rhs));
  }

  static void modulo(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::modulo(lhs, get_absolute(rhs));
  }

  static void bitwise_or(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_or(lhs, get_absolute(rhs));
  }

  static void bitwise_and(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_and(lhs, get_absolute(rhs));
  }

  static void bitwise_xor(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::bitwise_xor(lhs, get_absolute(rhs));
  }
};


// 3
template<class UnboundedUint, typename IntegralT>
struct unbounded_uint_integral_ops_impl<
  UnboundedUint, IntegralT, false
>
{
  typedef UnboundedUint                             unbounded_uint_type;
  typedef typename unbounded_uint_type::traits_type traits_type;
  typedef IntegralT                                 integral_type;
  typedef std::numeric_limits<integral_type>        integral_type_limits;

  typedef base::unbounded_uint_integral_ops_impl<
    base::unbounded_uint<traits_type>,
    integral_type
  >                                                 impl_type;

  static const unsigned radix_bits      = traits_type::radix_bits;
  static const unsigned max_digit_value = traits_type::max_digit_value;

  static const unsigned q =
    (integral_type_limits::digits + (radix_bits - 1))
    / radix_bits;

  static void assign(unbounded_uint_type& lhs, integral_type rhs);

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
    lhs.reserve(std::max(lhs.size(), q) + 1);
    impl_type::add(lhs, rhs);
  }

  static void subtract(unbounded_uint_type& lhs, integral_type rhs)
  {
    impl_type::subtract(lhs, rhs);
  }

  static void multiply(unbounded_uint_type& lhs, integral_type rhs)
  {
    lhs.reserve(lhs.size() + q);
    impl_type::multiply(lhs, rhs);
  }

  static void multiply(unbounded_uint_type& z,
                       const unbounded_uint_type& x, integral_type y)
  {
    z.reserve(x.size() + q);
    impl_type::multiply(z, x, y);
  }

  static void divide(unbounded_uint_type& lhs, integral_type rhs)
  {
    lhs /= unbounded_uint_type(rhs);
  }

  static void modulo(unbounded_uint_type& lhs, integral_type rhs)
  {
    lhs %= unbounded_uint_type(rhs);
  }

  static void bitwise_or(unbounded_uint_type& lhs, integral_type rhs)
  {
    lhs.reserve(q);
    impl_type::bitwise_or(lhs, rhs);
  }

  static void bitwise_and(unbounded_uint_type& lhs, integral_type rhs)
  {
    lhs.reserve(q);
    impl_type::bitwise_and(lhs, rhs);
  }

  static void bitwise_xor(unbounded_uint_type& lhs, integral_type rhs)
  {
    lhs.reserve(q);
    impl_type::bitwise_xor(lhs, rhs);
  }
};


template<class UnboundedUint, typename IntegralT>
const unsigned unbounded_uint_integral_ops_impl<
  UnboundedUint, IntegralT, false>::q;


template<class UnboundedUint, typename IntegralT>
void
unbounded_uint_integral_ops_impl<
  UnboundedUint, IntegralT, false>::assign(unbounded_uint_type& lhs,
                                           integral_type rhs)
{
  if (rhs <= max_digit_value)
  {
    lhs.reserve(1);
    lhs[0] = static_cast<typename traits_type::digit_type>(rhs);
    lhs.set_size(1);
  }
  else
  {
    lhs.reserve(q);

    base::from_integral_converter<
      unbounded_uint_type, integral_type
    >::convert(lhs, rhs);
  }
}


// 4)
template<class UnboundedUint, typename IntegralT>
struct unbounded_uint_integral_ops_impl<
  UnboundedUint, IntegralT, true
>
{
  typedef UnboundedUint                               unbounded_uint_type;
  typedef IntegralT                                   integral_type;
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




template<
  class UnboundedUint,
  typename IntegralT,
  bool less_equal =
      std::numeric_limits<IntegralT>::is_signed ?
      (
        std::numeric_limits<IntegralT>::digits <=
        std::numeric_limits<
          typename make_signed<
            typename UnboundedUint::traits_type::digit_type
          >::type
        >::digits
      ):
      (
        std::numeric_limits<IntegralT>::digits <=
        std::numeric_limits<
          typename UnboundedUint::traits_type::digit_type
        >::digits
      )
>
struct unbounded_uint_integral_ops
{};


// Dispatches integral types that fit into a digit_type by casting it to
// digit_type or signed digit_type and calling the specializations 1) and 2).
template<
  class UnboundedUint,
  typename IntegralT
>
struct unbounded_uint_integral_ops<UnboundedUint,IntegralT,true>
{
  BOOST_STATIC_ASSERT(is_integral<IntegralT>::value);

  typedef UnboundedUint                             unbounded_uint_type;
  typedef IntegralT                                 integral_type;
  typedef typename unbounded_uint_type::traits_type traits_type;

  typedef typename mpl::if_<
    is_signed<IntegralT>,
    unbounded_uint_integral_ops_impl<
      unbounded_uint_type,
      typename make_signed<
        typename unbounded_uint_type::digit_type
      >::type
    >,
    unbounded_uint_integral_ops_impl<
      unbounded_uint_type,
      typename unbounded_uint_type::digit_type
    >
  >::type                                           impl_type;

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
  class UnboundedUint,
  typename IntegralT
>
struct unbounded_uint_integral_ops<UnboundedUint,IntegralT,false>
{
  BOOST_STATIC_ASSERT(is_integral<IntegralT>::value);

  typedef UnboundedUint                             unbounded_uint_type;
  typedef IntegralT                                 integral_type;

  typedef unbounded_uint_integral_ops_impl<
      unbounded_uint_type, IntegralT
  >                                                 impl_type;

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






} // namespace detail
} // namespace mp_math
} // namespace boost

#endif


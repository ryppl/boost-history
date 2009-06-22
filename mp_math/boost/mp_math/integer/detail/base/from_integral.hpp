// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BASE_FROM_INTEGRAL_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_BASE_FROM_INTEGRAL_HPP


namespace boost {
namespace mp_math {
namespace detail {
namespace base {


template<
  class ApInt,
  typename IntegralT,
  bool ApIntIsSigned = ApInt::is_signed,
  bool IsSigned = std::numeric_limits<IntegralT>::is_signed,
  bool fits_into_digit_type =
    std::numeric_limits<IntegralT>::digits <=
    std::numeric_limits<typename ApInt::digit_type>::digits
    // TODO shouldn't we cmp against signed digit_type for signed integral types?
>
struct from_integral_converter;


// Specializations for unsigned ApInts //
/////////////////////////////////////////

template<class ApInt, typename IntegralT>
struct from_integral_converter<ApInt, IntegralT, false, false, true>
{
  static void convert(ApInt& z, IntegralT x)
  {
    z[0] = x;
    z.set_size(1);
  }
};


template<class ApInt, typename IntegralT>
struct from_integral_converter<ApInt, IntegralT, false, true, true>
{
  static void convert(ApInt& z, IntegralT x)
  {
    if (x >= 0)
    {
      z[0] = static_cast<typename ApInt::digit_type>(x);
      z.set_size(1);
    }
    else
      throw std::domain_error("from_integral_converter::convert: "
          "cannot convert negative integral value to unsigned integer value");
  }
};


template<class ApInt, typename IntegralT>
struct from_integral_converter<ApInt, IntegralT, false, false, false>
{
  static void convert(ApInt& z, IntegralT x);
};


template<class ApInt, typename IntegralT>
void from_integral_converter<ApInt, IntegralT, false, false, false>::
convert(ApInt& z, IntegralT x)
{
  static const IntegralT mask = ~(~IntegralT(0) << ApInt::traits_type::radix_bits);

  if (x == 0)
  {
    z[0] = 0;
    z.set_size(1);
    return;
  }

  typename ApInt::iterator z_iter = z.begin();
  while (x)
  {
    *z_iter++ = static_cast<typename ApInt::digit_type>(x & mask);
    x >>= ApInt::traits_type::radix_bits;
  }

  z.set_size(z_iter - z.begin());
}


template<class ApInt, typename IntegralT>
struct from_integral_converter<ApInt, IntegralT, false, true, false>
{
  typedef typename make_unsigned<IntegralT>::type unsigned_integral_type;

  static void convert(ApInt& z, IntegralT x)
  {
    if (x >= 0)
      from_integral_converter<
        ApInt, unsigned_integral_type
      >::convert(z, static_cast<unsigned_integral_type>(x));
    else
      throw std::domain_error("from_integral_converter::convert: "
          "cannot convert negative integral value to unsigned integer value");
  }
};


// Specializations for signed ApInts //
///////////////////////////////////////

template<class ApInt, typename IntegralT>
struct from_integral_converter<ApInt, IntegralT, true, false, true>
{
  static void convert(ApInt& z, IntegralT x)
  {
    z[0] = x;
    z.set_size(1);
    z.set_sign_bit(1);
  }
};


template<class ApInt, typename IntegralT>
struct from_integral_converter<ApInt, IntegralT, true, true, true>
{
  static void convert(ApInt& z, IntegralT x)
  {
    if (x >= 0)
    {
      z[0] = static_cast<typename ApInt::digit_type>(x);
      z.set_sign_bit(0);
    }
    else
    {
      z[0] = static_cast<typename ApInt::digit_type>(-x);
      z.set_sign_bit(1);
    }
    z.set_size(1);
  }
};


template<class ApInt, typename IntegralT>
struct from_integral_converter<ApInt, IntegralT, true, false, false>
{
  static void convert(ApInt& z, IntegralT x)
  {
    from_integral_converter<ApInt, IntegralT, false, false, false>::
      convert(z, x);
    z.set_sign_bit(0);
  }
};


template<class ApInt, typename IntegralT>
struct from_integral_converter<ApInt, IntegralT, true, true, false>
{
  typedef typename make_unsigned<IntegralT>::type unsigned_integral_type;

  static void convert(ApInt& z, IntegralT x)
  {
    if (x >= 0)
    {
      from_integral_converter<
        ApInt, unsigned_integral_type, false, true, false
      >::convert(z, static_cast<unsigned_integral_type>(x));
      z.set_sign_bit(0);
    }
    else
    {
      from_integral_converter<
        ApInt, IntegralT, false, true, false
      >::convert(z, static_cast<unsigned_integral_type>(-x));
      z.set_sign_bit(1);
    }
  }
};





} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost


#endif


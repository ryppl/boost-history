// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BASE_TO_INTEGRAL_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_BASE_TO_INTEGRAL_HPP


namespace boost {
namespace mp_math {
namespace detail {
namespace base {

// TODO throw std::overflow_error?

template<
  class ApInt,
  typename IntegralT,
  bool fits = static_cast<unsigned int>(std::numeric_limits<IntegralT>::digits)
            <= ApInt::traits_type::radix_bits
>
struct converter_base;


template<class ApInt, typename IntegralT>
struct converter_base<ApInt, IntegralT, false>
{
  typedef typename ApInt::size_type size_type;
  static IntegralT convert(const ApInt& x, size_type x_precision);
};


template<class ApInt, typename IntegralT>
IntegralT
converter_base<ApInt, IntegralT, false>::convert(
    const ApInt& x, size_type x_precision)
{
  IntegralT y = 0;
  const typename ApInt::digit_type* d = x.digits() + x.size() - 1;

  for (size_type i = 0; i < x_precision; i += ApInt::traits_type::radix_bits)
  {
    y <<= ApInt::traits_type::radix_bits;
    y |= *d--;
  }
  return y;
}


template<class ApInt, typename IntegralT>
struct converter_base<ApInt, IntegralT, true>
{
  typedef typename ApInt::size_type size_type;
  static IntegralT convert(const ApInt& x, size_type /*x_precision*/)
  {
    return static_cast<IntegralT>(x[0]);
  }
};


template<
  class ApInt,
  typename IntegralT,
  bool Is_ApInt_Signed = ApInt::is_signed,
  bool Is_IntegralT_Signed = std::numeric_limits<IntegralT>::is_signed
>
struct to_integral_converter;


template<class ApInt, typename IntegralT>
struct to_integral_converter<ApInt, IntegralT, false, false>
{
  typedef typename ApInt::size_type size_type;

  static IntegralT convert_without_check(const ApInt& x, size_type x_precision)
  {
    return converter_base<ApInt, IntegralT>::convert(x, x_precision);
  }

  static void check(size_type p)
  {
    if (p > static_cast<size_type>(std::numeric_limits<IntegralT>::digits))
      throw std::runtime_error("to_integral_converter::check: "
          "integral type does not have enough precision to hold result");
  }

  static IntegralT convert(const ApInt& x)
  {
    const size_type p = x.precision();
    check(p);
    return convert_without_check(x, p);
  }
};


template<class ApInt, typename IntegralT>
struct to_integral_converter<ApInt, IntegralT, false, true>
{
  typedef typename ApInt::size_type size_type;
  typedef typename make_unsigned<IntegralT>::type unsigned_type;

  static void check(size_type p)
  {
    to_integral_converter<ApInt, IntegralT, false, false>::check(p);
  }

  static IntegralT convert(const ApInt& x)
  {
    return static_cast<IntegralT>(
        to_integral_converter<ApInt, unsigned_type, false, false>::convert(x));
  }
};


template<class ApInt, typename IntegralT>
struct to_integral_converter<ApInt, IntegralT, true, false>
{
  typedef typename ApInt::size_type size_type;

  static void check(size_type p)
  {
    to_integral_converter<ApInt, IntegralT, false, false>::check(p);
  }

  static IntegralT convert(const ApInt& x)
  {
    const size_type p = x.precision();
    check(p);
    if (x.is_positive())
      return to_integral_converter<ApInt, IntegralT, false, false>::
        convert_without_check(x, p);
    else
      throw std::runtime_error("to_integral_converter::convert: "
          "cannot convert negative number to unsigned integral type");
  }
};


template<class ApInt, typename IntegralT>
struct to_integral_converter<ApInt, IntegralT, true, true>
{
  typedef typename ApInt::size_type size_type;

  static void check(size_type p)
  {
    // TODO static_cast<unsigned_type>(-IntegralT::min())
    // check if x fits into the unsigned type,
    // if yes then convert and check against the absolute min value and the max
    // else throw exception
    //
    // It is not the right thing to check only the precision, change to non
    // static member function, add reset member function, store precision as
    // member variable and change signature to check(const ApInt& x);
  }

  static IntegralT convert(const ApInt& x)
  {
    const size_type p = x.precision();
    check(p);

    typedef typename make_unsigned<IntegralT>::type unsigned_type;
    const unsigned_type y =
      to_integral_converter<ApInt, unsigned_type, false, false>::
        convert_without_check(x, p);

    if (x.is_positive())
      return static_cast<IntegralT>(y);
    else
      return -static_cast<IntegralT>(y);
  }
};



} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost


#endif


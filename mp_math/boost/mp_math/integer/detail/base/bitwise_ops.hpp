// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BASE_BITWISE_OPS_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_BASE_BITWISE_OPS_HPP

namespace boost {
namespace mp_math {
namespace detail {
namespace base {


template<class ApInt, bool IsSigned = ApInt::is_signed>
struct bitwise_ops;


template<class ApInt>
struct bitwise_ops<ApInt,false>
{
  typedef typename ApInt::traits_type::digit_type digit_type;
  typedef typename ApInt::traits_type::size_type  size_type;

  static void or_bits (ApInt& z, const ApInt& x, const ApInt& y);
  static void and_bits(ApInt& z, const ApInt& x, const ApInt& y);
  static void xor_bits(ApInt& z, const ApInt& x, const ApInt& y);
  static void compl_bits(ApInt& z, const ApInt& x);
};


template<class ApInt>
void
bitwise_ops<ApInt,false>::or_bits(ApInt& z, const ApInt& x, const ApInt& y)
{
  if (x.size() < y.size())
  {
    std::memcpy(z.digits() + x.size(),
                y.digits() + x.size(),
                (y.size() - x.size()) * sizeof(digit_type));
    z.set_size(y.size());
  }

  for (size_type i = 0; i < std::min(x.size(), y.size()); ++i)
    z[i] = x[i] | y[i];
}

template<class ApInt>
void
bitwise_ops<ApInt,false>::and_bits(ApInt& z, const ApInt& x, const ApInt& y)
{
  const size_type m = std::min(x.size(), y.size());

  for (size_type i = 0; i < m; ++i)
    z[i] = z[i] & y[i];

  z.set_size(m);
  z.clamp();
}

template<class ApInt>
void
bitwise_ops<ApInt,false>::xor_bits(ApInt& z, const ApInt& x, const ApInt& y)
{
  const size_type m = std::min(x.size(), y.size());

  if (x.size() < y.size())
  {
    std::memcpy(z.digits() + x.size(),
                y.digits() + x.size(),
                (y.size() - x.size()) * sizeof(digit_type));
    z.set_size(y.size());
  }

  for (size_type i = 0; i < m; ++i)
    z[i] = x[i] ^ y[i];

  z.clamp();
}

template<class ApInt>
void
bitwise_ops<ApInt,false>::compl_bits(ApInt& z, const ApInt& x)
{
  size_type i = 0;
  for (; i < x.size() - 1; ++i)
    z[i] = ~x[i];

  unsigned count = 0;
  digit_type bit = digit_type(1) << (ApInt::traits_type::radix_bits - 1);

  if (x[i])
    while (!(x[i] & bit))
    {
      bit >>= 1;
      ++count;
      if (count > 40)
        break;
    }
  else
    count = ApInt::traits_type::radix_bits - 1;

  digit_type mask = ~digit_type(0);
  mask >>= count;
  z[i] = ~x[i] & mask;

  z.set_size(x.size());
  z.clamp_high_digit();
}


template<class ApInt>
struct bitwise_ops<ApInt,true>
{
  static void or_bits (ApInt& z, const ApInt& x, const ApInt& y)
  {
    bitwise_ops<ApInt,false>::or_bits(z, x, y);
    z.set_sign_bit(x.sign_bit() | y.sign_bit());
  }

  static void and_bits(ApInt& z, const ApInt& x, const ApInt& y)
  {
    bitwise_ops<ApInt,false>::and_bits(z, x, y);
    z.set_sign_bit(x.sign_bit() & y.sign_bit());
  }

  static void xor_bits(ApInt& z, const ApInt& x, const ApInt& y)
  {
    bitwise_ops<ApInt,false>::xor_bits(z, x, y);
    z.set_sign_bit(x.sign_bit() ^ y.sign_bit());
  }

  static void compl_bits(ApInt& z, const ApInt& x)
  {
    bitwise_ops<ApInt,false>::compl_bits(z, x);
    z.set_sign_bit(z && x.is_positive() ? 1 : 0);
  }
};




} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost

#endif

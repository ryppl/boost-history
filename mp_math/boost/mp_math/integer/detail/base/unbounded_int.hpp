// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BASE_UNBOUNDED_INT_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_BASE_UNBOUNDED_INT_HPP

#include <boost/mp_math/integer/detail/base/unbounded_int_integral.hpp>
#include <boost/mp_math/integer/detail/base/unbounded_uint.hpp>

namespace boost {
namespace mp_math {
namespace detail {
namespace base {

// This class template inherits from unbounded_uint and adds functionality
// which is meaningful to signed integers.

// We store the sign in the high bit of capacity_ since capacity_ isn't updated
// as often as size_. This also allows unbounded_uint to function
// uninhibitedly since it will only update size_ but not capacity_.

template<class Traits, bool Debug>
struct unbounded_int
:
  unbounded_uint<Traits, Debug>
{
protected:

  template<typename IntegralT>
  struct integral_ops
  :
    unbounded_int_integral_ops<unbounded_int<Traits>, IntegralT>
  {};

public:

  typedef Traits                              traits_type;
  typedef unbounded_uint<Traits, Debug>       magnitude_type;
  typedef typename magnitude_type::digit_type digit_type;
  typedef typename magnitude_type::size_type  size_type;

  static const bool is_signed = true;

private:

  static const size_type sign_bit_ =
    size_type(1) << (std::numeric_limits<size_type>::digits - 1U);

public:

  unbounded_int(){}

  unbounded_int(digit_type* d, size_type size, size_type capacity)
  :
    magnitude_type(d, size, capacity)
  {}

  bool is_positive() const
  {
    return !(magnitude_type::capacity_ & sign_bit_);
  }

  bool is_negative() const
  {
    return magnitude_type::capacity_ & sign_bit_;
  }

  size_type capacity() const
  {
    return magnitude_type::capacity_ & ~sign_bit_;
  }

  void set_capacity(size_type c)
  {
    magnitude_type::capacity_ = c | (magnitude_type::capacity_ & sign_bit_);
  }

  int sign() const
  {
    return (magnitude_type::capacity_ & sign_bit_) ? -1 : 1;
  }

  void set_sign(int s)
  {
    if (s == 1)
      magnitude_type::capacity_ &= ~sign_bit_;
    else
      magnitude_type::capacity_ |= sign_bit_;
  }

  bool sign_bit() const { return magnitude_type::capacity_ & sign_bit_; }

  void set_sign_bit(bool s)
  {
    magnitude_type::capacity_ &= ~sign_bit_;
    magnitude_type::capacity_ |= static_cast<size_type>(s)
                              << (std::numeric_limits<size_type>::digits - 1U);
  }

  unbounded_int& operator ++()
  {
    integral_ops<digit_type>::add(*this, 1);
    return *this;
  }

  unbounded_int& operator --()
  {
    integral_ops<digit_type>::subtract(*this, 1);
    return *this;
  }

  unbounded_int& operator |= (const unbounded_int& rhs)
  {
    bitwise_ops<unbounded_int>::or_bits(*this, *this, rhs);
    return *this;
  }

  unbounded_int& operator &= (const unbounded_int& rhs)
  {
    bitwise_ops<unbounded_int>::and_bits(*this, *this, rhs);
    return *this;
  }

  unbounded_int& operator ^= (const unbounded_int& rhs)
  {
    bitwise_ops<unbounded_int>::xor_bits(*this, *this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator += (IntegralT rhs)
  {
    integral_ops<IntegralT>::add(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator -= (IntegralT rhs)
  {
    integral_ops<IntegralT>::subtract(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator *= (IntegralT rhs)
  {
    integral_ops<IntegralT>::multiply(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator /= (IntegralT rhs)
  {
    integral_ops<IntegralT>::divide(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator %= (IntegralT rhs)
  {
    integral_ops<IntegralT>::modulo(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator |= (IntegralT rhs)
  {
    integral_ops<IntegralT>::bitwise_or(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator &= (IntegralT rhs)
  {
    integral_ops<IntegralT>::bitwise_and(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator ^= (IntegralT rhs)
  {
    integral_ops<IntegralT>::bitwise_xor(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  IntegralT to_integral() const
  {
    return to_integral_converter<
      unbounded_int<traits_type, Debug>, IntegralT>::convert(*this);
  }

  void truncate(size_type prec)
  {
    magnitude_type::truncate(prec);
    if (magnitude_type::is_uninitialized())
      set_sign_bit(0);
  }

  void print(bool all = false) const
  {
    if (is_negative())
      std::cout << "-";
    magnitude_type::print(all);
  }
};



} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost

#endif


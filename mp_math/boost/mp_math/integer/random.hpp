// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_RANDOM_HPP
#define BOOST_MP_MATH_INTEGER_RANDOM_HPP

#include <boost/random.hpp>
#include <boost/mp_math/integer/integer_fwd.hpp>

namespace boost {
namespace mp_math {

// this class is modeled after boost::uniform_int
// in fact it uses boost::uniform_int internally
template<class Integer = integer<> >
struct uniform_integer
{
  typedef Integer input_type;
  typedef Integer result_type;

  // is min() and max() known at compile time?
  static const bool has_fixed_range = false;

  uniform_integer(const Integer& min, const Integer& max)
  :
    min_(min),
    max_(max),
    d_(0, Integer::traits_type::max_digit_value)
  {}

  result_type min() const { return min_; }
  result_type max() const { return max_; }

  void reset() { d_.reset(); }

  template<class Engine>
  result_type operator()(Engine& e);

private:

  Integer min_, max_;
  uniform_int<typename Integer::digit_type> d_;
};

template<class Integer>
const bool uniform_integer<Integer>::has_fixed_range;


template<class Integer>
template<class Engine>
typename uniform_integer<Integer>::result_type
uniform_integer<Integer>::operator()(Engine& e)
{
  result_type tmp;
  tmp.reserve(max_.size());

  for (typename result_type::size_type i = 0; i < max_.size(); ++i)
    tmp[i] = d_(e);

  tmp.set_size(max_.size());
  tmp.clamp();

  if (tmp > max_)
    tmp %= max_; // TODO tmp.truncate(max_.precision()), tmp -= max_!

  return tmp;
}


template<class Integer = integer<> >
struct uniform_integer_bits
{
  typedef Integer input_type;
  typedef Integer result_type;
  typedef typename Integer::size_type size_type;

  static const bool has_fixed_range = false;

  explicit uniform_integer_bits(size_type bits)
  :
    d_(0, Integer::traits_type::max_digit_value),
    bits_(bits)
  {}

  result_type min() const;
  result_type max() const;

  size_type precision() const { return bits_; }

  void reset() { d_.reset(); }

  template<class Engine>
  result_type operator()(Engine& e);

private:

  uniform_int<typename Integer::digit_type> d_;
  size_type bits_;
};

template<class Integer>
const bool uniform_integer_bits<Integer>::has_fixed_range;


template<class Integer>
template<class Engine>
typename uniform_integer_bits<Integer>::result_type
uniform_integer_bits<Integer>::operator()(Engine& e)
{
  result_type tmp;

  const size_type offset = bits_ / Integer::traits_type::radix_bits + 1;

  tmp.reserve(offset);

  for (size_type i = 0; i < offset; ++i)
    tmp[i] = d_(e);

  tmp.set_size(offset);

  const typename Integer::digit_type mask =
      (~typename Integer::digit_type(0))
    << (bits_ % Integer::traits_type::radix_bits);

  tmp[offset-1] &= ~mask;

  tmp.set_bit(bits_-1); // make exactly bits_ bits long
  tmp.clamp_high_digit();

  return tmp;
}

template<class Integer>
typename uniform_integer_bits<Integer>::result_type
uniform_integer_bits<Integer>::min() const
{
  result_type tmp;

  const size_type offset = bits_ / Integer::traits_type::radix_bits + 1;

  tmp.reserve(offset);

  std::memset(tmp.digits(), 0,
      offset * sizeof(typename Integer::digit_type));

  tmp.set_size(offset);
  tmp.set_bit(bits_ - 1);
  tmp.clamp_high_digit();

  return tmp;
}

template<class Integer>
typename uniform_integer_bits<Integer>::result_type
uniform_integer_bits<Integer>::max() const
{
  result_type tmp;
  const typename Integer::size_type offset =
    bits_ / Integer::traits_type::radix_bits + 1;

  tmp.reserve(offset);

  for (size_type i = 0; i < offset; ++i)
    tmp[i] = Integer::traits_type::max_digit_value;

  tmp.set_size(offset);

  const typename Integer::digit_type mask =
      (~typename Integer::digit_type(0))
    << (bits_ % Integer::traits_type::radix_bits);

  tmp[offset-1] &= ~mask;
  tmp.clamp_high_digit();

  return tmp;
}


} // namespace mp_math
} // namespace boost

#endif


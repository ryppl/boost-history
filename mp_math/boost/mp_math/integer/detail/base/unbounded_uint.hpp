// Copyright Tom St Denis 2002 - 2007.
// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_BASE_UNBOUNDED_UINT_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_BASE_UNBOUNDED_UINT_HPP

#include <algorithm> // swap
#include <cassert>
#include <iostream>
#include <iterator> // reverse_iterator
#include <boost/config.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mp_math/integer/detail/base/adder.hpp>
#include <boost/mp_math/integer/detail/base/bitwise_ops.hpp>
#include <boost/mp_math/integer/detail/base/shifter.hpp>
#include <boost/mp_math/integer/detail/base/to_integral.hpp>
#include <boost/mp_math/integer/detail/base/unbounded_uint_integral.hpp>

namespace boost {
namespace mp_math {
namespace detail {
namespace base {

// The unbounded_uint_common class template provide all the functions that
// don't need to allocate memory. This is useful for the optimization of some
// high level algorithms where you allocate a pool of memory once and then
// construct some unbounded_uint objects in it.

// Internally the least significant digit is stored at digits_[0], while the
// most significant is stored at digits_[size_-1].

template<class Traits>
struct unbounded_uint_common
{
protected:

  template<typename IntegralT>
  struct integral_ops
  :
    unbounded_uint_integral_ops<unbounded_uint_common<Traits>, IntegralT>
  {};

public:

  typedef Traits                           traits_type;
  typedef typename traits_type::digit_type digit_type;
  typedef typename traits_type::size_type  size_type;

  typedef digit_type&                           reference;
  typedef const digit_type&                     const_reference;
  typedef digit_type*                           iterator;
  typedef const digit_type*                     const_iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  static const bool is_signed = false;

  unbounded_uint_common(){}

  unbounded_uint_common(digit_type* d, size_type size, size_type capacity)
  :
    digits_(d), size_(size), capacity_(capacity)
  {}

  #ifdef BOOST_HAS_RVALUE_REFS
  void swap(unbounded_uint_common&& rhs);
  #else
  void swap(unbounded_uint_common& rhs);
  #endif

  iterator       begin()       { return digits_;         }
  iterator       end  ()       { return digits_ + size_; }
  const_iterator begin() const { return digits_;         }
  const_iterator end  () const { return digits_ + size_; }
  reverse_iterator       rbegin()       { return reverse_iterator(end());   }
  reverse_iterator       rend  ()       { return reverse_iterator(begin()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end());   }
  const_reverse_iterator rend  () const { return const_reverse_iterator(begin()); }

  digit_type&       operator[](size_type i)       { return digits_[i]; }
  const digit_type& operator[](size_type i) const { return digits_[i]; }

  digit_type*       digits()       { return digits_; }
  const digit_type* digits() const { return digits_; }

#ifdef BOOST_NO_EXPLICIT_CONVERSION_OPERATORS
private:

  typedef size_type unbounded_uint_common::*unspecified_bool_type;

public:

  operator unspecified_bool_type() const
  {
	  return !(size_ == 1 && digits_[0] == 0) ?
           &unbounded_uint_common::size_ : 0;
  }
#else
  explicit operator bool() const { return !(size_ == 1 && digits_[0] == 0); }
#endif

  bool is_even() const { return (digits_[0] & 1U) == 0U; }
  bool is_odd () const { return (digits_[0] & 1U) == 1U; }
  bool is_initialized  () const { return size_ != 0U; }
  bool is_uninitialized() const { return size_ == 0U; }

  size_type size() const          { return size_; }
  void      set_size(size_type s) { size_ = s;    }

  size_type capacity() const          { return capacity_; }
  void      set_capacity(size_type c) { capacity_ = c;    }

  void push(digit_type x) { digits_[size_++] = x; }
  void pop() { --size_; }

  reference       least_significant()       { return digits_[0]; }
  const_reference least_significant() const { return digits_[0]; }

  reference       most_significant()       { return digits_[size_ - 1]; }
  const_reference most_significant() const { return digits_[size_ - 1]; }

  void clamp();
  void clamp_high_digit();

  void set_bit(size_type bit)
  {
    digits_[bit / traits_type::radix_bits] |=
      digit_type(1) << (bit % traits_type::radix_bits);
  }

  void clear_bit(size_type bit)
  {
    digits_[bit / traits_type::radix_bits] &=
      ~(digit_type(1) << (bit % traits_type::radix_bits));
  }

  void set_bits  (size_type beg, size_type end);
  void clear_bits(size_type beg, size_type end);

  void truncate(size_type prec);

  size_type precision() const;

  void set_precision(size_type bits)
  {
    size_ = (bits + (traits_type::radix_bits - 1)) / traits_type::radix_bits;
  }

  void zero();

  size_type count_trailing_zero_bits() const;

  unbounded_uint_common& operator ++()
  {
    integral_ops<digit_type>::add(*this, 1);
    return *this;
  }

  unbounded_uint_common& operator --()
  {
    integral_ops<digit_type>::subtract(*this, 1);
    return *this;
  }

  unbounded_uint_common& operator <<= (size_type);
  unbounded_uint_common& operator >>= (size_type);

  unbounded_uint_common& operator += (const unbounded_uint_common&);
  unbounded_uint_common& operator -= (const unbounded_uint_common&);

  unbounded_uint_common& operator |= (const unbounded_uint_common& rhs)
  {
    bitwise_ops<unbounded_uint_common>::or_bits(*this, *this, rhs);
    return *this;
  }

  unbounded_uint_common& operator &= (const unbounded_uint_common& rhs)
  {
    bitwise_ops<unbounded_uint_common>::and_bits(*this, *this, rhs);
    return *this;
  }

  unbounded_uint_common& operator ^= (const unbounded_uint_common& rhs)
  {
    bitwise_ops<unbounded_uint_common>::xor_bits(*this, *this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint_common&>::type
  operator += (IntegralT rhs)
  {
    integral_ops<IntegralT>::add(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint_common&>::type
  operator -= (IntegralT rhs)
  {
    integral_ops<IntegralT>::subtract(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint_common&>::type
  operator *= (IntegralT rhs)
  {
    integral_ops<IntegralT>::multiply(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint_common&>::type
  operator /= (IntegralT rhs)
  {
    integral_ops<IntegralT>::divide(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint_common&>::type
  operator %= (IntegralT rhs)
  {
    integral_ops<IntegralT>::modulo(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint_common&>::type
  operator |= (IntegralT rhs)
  {
    integral_ops<IntegralT>::bitwise_or(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint_common&>::type
  operator &= (IntegralT rhs)
  {
    integral_ops<IntegralT>::bitwise_and(*this, rhs);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint_common&>::type
  operator ^= (IntegralT rhs)
  {
    integral_ops<IntegralT>::bitwise_xor(*this, rhs);
    return *this;
  }

  // TODO it may not be possible to do the string stuff correctly because we
  // can't just call *this += uint_type(s). We could implement some kind of
  // digit_type iterator that iterates over the string and returns a digit_type.
  // and then write the algorithm in such a way that it retrieves the next digit
  // on demand. This may be too slow for things like *= wich jumps all over the
  // place with its memory accesses.
  template<typename charT> unbounded_uint_common& operator += (const charT*);
  template<typename charT> unbounded_uint_common& operator -= (const charT*);
  template<typename charT> unbounded_uint_common& operator |= (const charT*);
  template<typename charT> unbounded_uint_common& operator &= (const charT*);
  template<typename charT> unbounded_uint_common& operator ^= (const charT*);

  template<typename charT, class traits, class alloc>
  unbounded_uint_common&
  operator += (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_uint_common&
  operator -= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_uint_common&
  operator |= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_uint_common&
  operator &= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_uint_common&
  operator ^= (const std::basic_string<charT,traits,alloc>&);

  void print(bool all = false) const;

  template<typename IntegralT>
  IntegralT to_integral() const
  {
    return to_integral_converter<
      unbounded_uint_common<traits_type>, IntegralT>::convert(*this);
  }

protected:

  digit_type* digits_;
  size_type   size_;
  size_type   capacity_;
};


template<class Traits>
#ifdef BOOST_HAS_RVALUE_REFS
void unbounded_uint_common<Traits>::swap(unbounded_uint_common&& rhs)
#else
void unbounded_uint_common<Traits>::swap(unbounded_uint_common& rhs)
#endif
{
  std::swap(digits_,   rhs.digits_  );
  std::swap(size_,     rhs.size_    );
  std::swap(capacity_, rhs.capacity_);
}

template<class Traits>
inline void swap(unbounded_uint_common<Traits>& x,
                 unbounded_uint_common<Traits>& y)
{
  x.swap(y);
}

#ifdef BOOST_HAS_RVALUE_REFS
template<class Traits>
inline void swap(unbounded_uint_common<Traits>&& x,
                 unbounded_uint_common<Traits>& y)
{
  x.swap(y);
}

template<class Traits>
inline void swap(unbounded_uint_common<Traits>& x,
                 unbounded_uint_common<Traits>&& y)
{
  x.swap(y);
}
#endif

// This is used to ensure that leading zero digits are trimmed.
template<class Traits>
void unbounded_uint_common<Traits>::clamp()
{
  while (size_ > 1U && digits_[size_-1] == 0U)
    --size_;
}

// For when we know that only one leading zero digit may exist.
template<class Traits>
inline void unbounded_uint_common<Traits>::clamp_high_digit()
{
  if (size_ > 1U && digits_[size_-1] == 0U)
    --size_;
}

template<class Traits>
void
unbounded_uint_common<Traits>::set_bits(size_type beg, size_type end)
{
  const size_type beg_index  = beg / traits_type::digit_bits;
  const size_type end_index  = end / traits_type::digit_bits;
  const size_type first_bits = beg % traits_type::digit_bits;
  const size_type last_bits  = end % traits_type::digit_bits;

  static const digit_type z = ~digit_type(0);

  digit_type mask = z << first_bits;
  if (beg_index == end_index && last_bits)
    mask &= z >> (traits_type::digit_bits - last_bits);

  digits_[beg_index] |= mask;

  for (size_type i = beg_index + ((beg % traits_type::digit_bits) ? 1 : 0);
       i < end_index; ++i)
    digits_[i] = traits_type::max_digit_value;

  if (beg_index != end_index && last_bits)
    digits_[end_index] |= z >> (traits_type::digit_bits - last_bits);
}

template<class Traits>
void
unbounded_uint_common<Traits>::clear_bits(size_type beg, size_type end)
{
  const size_type beg_index  = beg / traits_type::digit_bits;
  const size_type end_index  = end / traits_type::digit_bits;
  const size_type first_bits = beg % traits_type::digit_bits;
  const size_type last_bits  = end % traits_type::digit_bits;

  static const digit_type z = ~digit_type(0);

  digit_type mask;
  if (first_bits)
    mask = z >> (traits_type::digit_bits - first_bits);
  else
    mask = 0;

  if (beg_index == end_index)
    mask |= z << last_bits;

  digits_[beg_index] &= mask;

  if (beg_index != end_index)
  {
    std::memset(digits_ + beg_index + 1, 0,
        sizeof(digit_type) * (end_index - beg_index - 1));

    if (last_bits)
      digits_[end_index] &= z << last_bits;
  }
}

// TODO no need to call clamp after truncate since we call set_precision here
template<class Traits>
void unbounded_uint_common<Traits>::truncate(size_type prec)
{
  set_precision(prec);
  const size_type last_bits = prec % traits_type::radix_bits;
  if (last_bits)
  {
    static const digit_type z = ~digit_type(0);
    const digit_type mask = z >> (traits_type::radix_bits - last_bits);
    digits_[size_ - 1] &= mask;
  }
}

template<class Traits>
typename unbounded_uint_common<Traits>::size_type
unbounded_uint_common<Traits>::precision() const
{
  if (is_initialized())
  {
    size_type p = (size_ - 1) * traits_type::radix_bits;

    // count bits in most significant digit
    digit_type q = digits_[size_ - 1];
    while (q > 0U)
    {
      ++p;
      q >>= 1;
    }

    return p;
  }
  else
    return 0;
}

// TODO remove this function?
template<class Traits>
inline void unbounded_uint_common<Traits>::zero()
{
  digits_[0] = 0;
  size_ = 1;
}

// Counts the number of lsbs which are zero before the first one bit
template<class Traits>
typename unbounded_uint_common<Traits>::size_type
unbounded_uint_common<Traits>::count_trailing_zero_bits() const
{
  static const size_type lnz[16] = {
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
  };

  if (!*this)
    return 0;

  size_type bits = 0;

  // scan lower digits until non-zero
  const_iterator d = begin();
  while (bits < size_ && *d == 0)
  {
    ++bits;
    ++d;
  }

  bits *= traits_type::radix_bits;

  digit_type q = *d;
  // now scan this digit until a 1 is found
  if ((q & 1) == 0)
  {
    digit_type qq;
    do
    {
      qq    = q & 15;
      bits += lnz[qq];
      q >>= 4;
    } while (qq == 0);
  }

  return bits;
}

template<class Traits>
void unbounded_uint_common<Traits>::print(bool all) const
{
  using std::cout;
  cout << size_ << "{";
  for (size_type i = 0; i < size(); ++i)
  {
    cout << static_cast<typename traits_type::word_type>(digits()[i]);
    if (i < size()  - 1)
      cout << ",";
  }
  cout << "}";

  if (all)
  {
    cout << capacity() - size() << "{";
    for (size_type i = size(); i < capacity(); ++i)
    {
      cout << static_cast<typename traits_type::word_type>(digits()[i]);
      if (i < capacity()  - 1)
        cout << ",";
    }
    cout << "}";
  }
  cout << "\n";
}


template<class Traits>
unbounded_uint_common<Traits>&
unbounded_uint_common<Traits>::operator <<= (const size_type n)
{
  if (*this != digit_type(0))
    shifter<unbounded_uint_common<Traits> >::shift_bits_left(*this, n);
  return *this;
}

template<class Traits>
unbounded_uint_common<Traits>&
unbounded_uint_common<Traits>::operator >>= (const size_type n)
{
  shifter<unbounded_uint_common<Traits> >::shift_bits_right(*this, n);
  return *this;
}

template<class Traits>
unbounded_uint_common<Traits>&
unbounded_uint_common<Traits>::
operator += (const unbounded_uint_common<Traits>& rhs)
{
  const unbounded_uint_common<Traits> *a, *b;
  if (size() > rhs.size())
  {
    a = this;
    b = &rhs;
  }
  else
  {
    a = &rhs;
    b = this;
  }

  typedef adder<unbounded_uint_common<Traits> > adder_type;

  adder_type::add_smaller_magnitude(*this, *a, *b);

  return *this;
}

template<class Traits>
unbounded_uint_common<Traits>&
unbounded_uint_common<Traits>::
operator -= (const unbounded_uint_common<Traits>& rhs)
{
  const unbounded_uint_common<Traits> *a, *b;
  if (size() > rhs.size())
  {
    a = this;
    b = &rhs;
  }
  else
  {
    a = &rhs;
    b = this;
  }

  typedef adder<unbounded_uint_common<Traits> > adder_type;

  adder_type::subtract_smaller_magnitude(*this, *a, *b);

  return *this;
}

/*
template<class Traits>
unbounded_uint_common<Traits>&
unbounded_uint_common<Traits>::
operator |= (const unbounded_uint_common<Traits>& rhs)
{
  if (size() < rhs.size())
  {
    std::memcpy(digits() + size(),
                rhs.digits() + size(),
                (rhs.size() - size()) * sizeof(digit_type));
    set_size(rhs.size());
  }

  for (size_type i = 0; i < std::min(size(), rhs.size()); ++i)
    digits()[i] |= rhs[i];

  return *this;
}

template<class Traits>
unbounded_uint_common<Traits>&
unbounded_uint_common<Traits>::
operator &= (const unbounded_uint_common<Traits>& rhs)
{
  const size_type m = std::min(size(), rhs.size());

  for (size_type i = 0; i < m; ++i)
    digits()[i] &= rhs[i];

  set_size(m);
  clamp();

  return *this;
}

template<class Traits>
unbounded_uint_common<Traits>&
unbounded_uint_common<Traits>::
operator ^= (const unbounded_uint_common<Traits>& rhs)
{
  const size_type min = std::min(size(), rhs.size());

  if (size() < rhs.size())
  {
    std::memcpy(digits() + size(),
                rhs.digits() + size(),
                (rhs.size() - size()) * sizeof(digit_type));
    set_size(rhs.size());
  }

  for (size_type i = 0; i < min; ++i)
    digits()[i] ^= rhs[i];

  clamp();

  return *this;
}*/







/*

// x += y
template<class Traits, bool Debug>
void add_smaller_magnitude(unbounded_uint<Traits,Debug>& x,
                           const unbounded_uint<Traits,Debug>& y)
{
  assert(x.size() > y.size());

  const digit_type carry =
    traits_type::ops_type::add_digits(x.digits(),
                                      x.digits(),
                                      y.digits(), y.size());

  size_type n =
    traits_type::ops_type::ripple_carry(x.digits() + y.size(),
                                        x.digits() + y.size(),
                                        x.size() - y.size(), carry);
  n += y.size();

  if (n < x.size()) // this implies that there is no carry left
    return;
  else if (carry) // at this point n equals x->size_
    x[n++] = carry;

  x.set_size(n);
}

// x -= y
template<class Traits, bool Debug>
void sub_smaller_magnitude()
{
  traits_type::ops_type::sub_smaller_magnitude(digits(),
                                               x->digits(), x->size(),
                                               y->digits(), y->size());

  set_size(x->size());

  clamp();
}*/


template<class Traits>
inline
bool operator < (const unbounded_uint_common<Traits>& lhs,
                 const unbounded_uint_common<Traits>& rhs)
{
  const int b = Traits::ops_type::compare_magnitude(lhs.digits(), lhs.size(),
                                                    rhs.digits(), rhs.size());
  return b == -1;
}

template<class Traits>
inline
bool operator > (const unbounded_uint_common<Traits>& lhs,
                 const unbounded_uint_common<Traits>& rhs)
{
  const int b = Traits::ops_type::compare_magnitude(lhs.digits(), lhs.size(),
                                                    rhs.digits(), rhs.size());
  return b == 1;
}

template<class Traits>
inline
bool operator <= (const unbounded_uint_common<Traits>& lhs,
                  const unbounded_uint_common<Traits>& rhs)
{
  const int b = Traits::ops_type::compare_magnitude(lhs.digits(), lhs.size(),
                                                    rhs.digits(), rhs.size());
  return b != 1;
}

template<class Traits>
inline
bool operator >= (const unbounded_uint_common<Traits>& lhs,
                  const unbounded_uint_common<Traits>& rhs)
{
  const int b = Traits::ops_type::compare_magnitude(lhs.digits(), lhs.size(),
                                                    rhs.digits(), rhs.size());
  return b != -1;
}







// Now we add a layer of debug functionality. This functionality
// is meant to be used by developers that develop new algorithms for this
// library.
template<class Traits>
struct unbounded_uint<Traits, false>
:
  unbounded_uint_common<Traits>
{
  typedef unbounded_uint_common<Traits> base_type;

  typedef Traits traits_type;
  typedef typename traits_type::digit_type digit_type;
  typedef typename traits_type::size_type  size_type;

  unbounded_uint(){}

  unbounded_uint(digit_type* d, size_type size, size_type capacity)
  :
    base_type(d, size, capacity)
  {}

  void assert_invariants() const{}
};


template<class Traits>
struct unbounded_uint<Traits, true>
:
  unbounded_uint_common<Traits>
{
  typedef unbounded_uint_common<Traits> base_type;

  typedef Traits traits_type;
  typedef typename traits_type::digit_type digit_type;
  typedef typename traits_type::size_type  size_type;

  unbounded_uint(){}

  unbounded_uint(digit_type* d, size_type size, size_type capacity)
  :
    base_type(d, size, capacity)
  {}

  // replace some inherited functions from the base class with debug versions
/*  digit_type&       operator[](size_type i)
  {
    assert(i < base_type::size_);
    return digits_[i];
  }

  const digit_type& operator[](size_type i) const
  {
    assert(i < size_)
    return digits_[i];
  }

  void push(digit_type x)
  {
    assert(size_ < capacity());
    digits_[size_++] = x;
  }

  void pop()
  {
    assert(size_ > 0);
    --size_;
  }

  void set_bit  (size_type bit);
  void clear_bit(size_type bit);

  void set_bits  (size_type beg, size_type end);
  void clear_bits(size_type beg, size_type end);

  void set_precision(size_type bits);*/

  void assert_invariants() const;
};

/*
template<class Traits>
void unbounded_uint<Traits,true>::set_bit(size_type bit)
{
  const size_type index = bit / traits_type::radix_bits;
  assert(index < size_);
  base_type::set_bit(bit);
}

template<class Traits>
void unbounded_uint<Traits,true>::clear_bit(size_type bit)
{
  const size_type index = bit / traits_type::radix_bits;
  assert(index < size_);
  base_type::clear_bit(bit);
}

template<class Traits>
void unbounded_uint<Traits,true>::set_bits(size_type beg, size_type end)
{
  assert(beg < end);
  const size_type index = bit / traits_type::radix_bits;
  assert(index < size_);
  base_type::set_bits(beg, end);
}

template<class Traits>
void unbounded_uint<Traits,true>::clear_bits(size_type beg, size_type end)
{
  assert(beg < end);
  const size_type index = bit / traits_type::radix_bits;
  assert(index < size_);
  base_type::clear_bits(beg, end);
}

template<class Traits>
void unbounded_uint<Traits,true>::set_precision(size_type bits)
{
  const size_type new_size = bits / traits_type::radix_bits;
  assert(new_size < this->capacity());
  base_type::set_precision();
}*/

template<class Traits>
void unbounded_uint<Traits,true>::assert_invariants() const
{
  if (base_type::is_initialized())
  {
    assert(base_type::size() <= base_type::capacity());
    if (base_type::size() > 1U)
      assert(base_type::digits()[base_type::size()-1] != 0U);
  }
}




} // namespace base
} // namespace detail
} // namespace mp_math
} // namespace boost

#endif


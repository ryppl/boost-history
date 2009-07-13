// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_UNBOUNDED_INT_HPP
#define BOOST_MP_MATH_INTEGER_UNBOUNDED_INT_HPP

#include <boost/config.hpp>
#include <boost/mp_math/integer/contexts.hpp>
#include <boost/mp_math/integer/multiprecision_integer_tag.hpp>
#include <boost/mp_math/integer/unbounded_traits.hpp>
#include <boost/mp_math/integer/detail/adder.hpp>
#include <boost/mp_math/integer/detail/bitwise_ops.hpp>
#include <boost/mp_math/integer/detail/digit_converter.hpp>
#include <boost/mp_math/integer/detail/divider.hpp>
#include <boost/mp_math/integer/detail/gcd.hpp>
#include <boost/mp_math/integer/detail/jacobi.hpp>
#include <boost/mp_math/integer/detail/lcm.hpp>
#include <boost/mp_math/integer/detail/modinv.hpp>
#include <boost/mp_math/integer/detail/modpow.hpp>
#include <boost/mp_math/integer/detail/power.hpp>
#include <boost/mp_math/integer/detail/root.hpp>
#include <boost/mp_math/integer/detail/stream_io.hpp>
#include <boost/mp_math/integer/detail/string_conversion.hpp>
#include <boost/mp_math/integer/detail/unbounded_int_integral.hpp>
#include <boost/mp_math/integer/detail/base/unbounded_int.hpp>

namespace boost {
namespace mp_math {

template<
  class Alloc = std::allocator<void>,
  class Traits = unbounded_traits<>
>
struct unbounded_int
:
  Alloc::template rebind<typename Traits::digit_type>::other,
  detail::base::unbounded_int<Traits>
{
protected:

  typedef detail::base::unbounded_int<Traits> base_type;

  typedef typename Alloc::template
    rebind<typename Traits::digit_type>::other base_allocator_type;

public:

  typedef multiprecision_integer_tag tag;

  template<typename IntegralT>
  struct integral_ops
  :
    detail::unbounded_int_integral_ops<unbounded_int, IntegralT>
  {};

  typedef Alloc                              allocator_type;
  typedef Traits                             traits_type;
  typedef typename traits_type::digit_type   digit_type;
  typedef typename allocator_type::size_type size_type;

  typedef typename base_type::iterator               iterator;
  typedef typename base_type::const_iterator         const_iterator;
  typedef typename base_type::reverse_iterator       reverse_iterator;
  typedef typename base_type::const_reverse_iterator const_reverse_iterator;
  typedef typename base_type::magnitude_type         magnitude_type;

  unbounded_int();

  explicit unbounded_int(const allocator_type& a);

  template<typename IntegralT>
  unbounded_int(IntegralT,
         const allocator_type& a = allocator_type(),
         typename enable_if<is_integral<IntegralT> >::type* dummy = 0);

  template<typename charT>
  unbounded_int(const charT*, const allocator_type& a = allocator_type());

  template<typename charT>
  unbounded_int(const charT*,
                std::ios_base::fmtflags,
                const allocator_type& a = allocator_type());

  template<typename charT, class traits, class alloc>
  unbounded_int(const std::basic_string<charT,traits,alloc>&,
                const allocator_type& a = allocator_type());

  template<typename charT, class traits, class alloc>
  unbounded_int(const std::basic_string<charT,traits,alloc>&,
                std::ios_base::fmtflags,
                const allocator_type& a = allocator_type());

  template<typename RandomAccessIterator>
  unbounded_int(RandomAccessIterator first,
                RandomAccessIterator last,
                const allocator_type& a = allocator_type());

  template<typename RandomAccessIterator>
  unbounded_int(RandomAccessIterator first,
                RandomAccessIterator last,
                std::ios_base::fmtflags f,
                const allocator_type& a = allocator_type());

  template<class ApInt>
  unbounded_int(const ApInt& x,
                typename enable_if<
                  mpl::and_<
                    mpl::not_<is_same<traits_type, typename ApInt::traits_type> >,
                    is_same<typename ApInt::tag, multiprecision_integer_tag>
                  >
                  >::type* dummy = 0);

  unbounded_int(const unbounded_int& copy);

  #ifndef BOOST_NO_RVALUE_REFERENCES
  unbounded_int(unbounded_int&& copy);
  #endif

  ~unbounded_int();

  unbounded_int& operator = (const unbounded_int& rhs);

  #ifndef BOOST_NO_RVALUE_REFERENCES
  unbounded_int& operator = (unbounded_int&& rhs);
  #endif

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator = (IntegralT rhs);

  template<typename charT>
  unbounded_int& operator = (const charT*);

  template<typename charT, class traits, class alloc>
  unbounded_int& operator = (const std::basic_string<charT,traits,alloc>&);

  template<typename charT>
  void assign(const charT*, std::ios_base::fmtflags);

  template<typename charT, class traits, class alloc>
  void assign(const std::basic_string<charT,traits,alloc>&,
              std::ios_base::fmtflags);

  template<typename RandomAccessIterator>
  void assign(RandomAccessIterator first, RandomAccessIterator last,
              std::ios_base::fmtflags);

  void swap(unbounded_int& other)
  {
    base_type::swap(other);
  }

  allocator_type get_allocator() const { return allocator_type(); }

  void reserve(size_type n);

  unbounded_int& operator ++();
  unbounded_int& operator --();
  unbounded_int  operator ++(int);
  unbounded_int  operator --(int);
  unbounded_int& operator <<= (size_type);
  unbounded_int& operator >>= (size_type);
  unbounded_int& operator - ();

  unbounded_int& operator += (const unbounded_int&);
  unbounded_int& operator -= (const unbounded_int&);
  unbounded_int& operator *= (const unbounded_int&);
  unbounded_int& operator /= (const unbounded_int&);
  unbounded_int& operator %= (const unbounded_int&);
  unbounded_int& operator |= (const unbounded_int&);
  unbounded_int& operator &= (const unbounded_int&);
  unbounded_int& operator ^= (const unbounded_int&);

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator += (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator -= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator *= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator /= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator %= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator |= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator &= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_int&>::type
  operator ^= (IntegralT);

  template<typename charT> unbounded_int& operator += (const charT*);
  template<typename charT> unbounded_int& operator -= (const charT*);
  template<typename charT> unbounded_int& operator *= (const charT*);
  template<typename charT> unbounded_int& operator /= (const charT*);
  template<typename charT> unbounded_int& operator %= (const charT*);
  template<typename charT> unbounded_int& operator |= (const charT*);
  template<typename charT> unbounded_int& operator &= (const charT*);
  template<typename charT> unbounded_int& operator ^= (const charT*);

  template<typename charT, class traits, class alloc>
  unbounded_int& operator += (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_int& operator -= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_int& operator *= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_int& operator /= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_int& operator %= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_int& operator |= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_int& operator &= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_int& operator ^= (const std::basic_string<charT,traits,alloc>&);

  template<class StringT>
  StringT to_string(std::ios_base::fmtflags f = std::ios_base::dec) const
  {
    StringT tmp;
    detail::to_string_converter<unbounded_int> conv;
    conv.convert(tmp, *this, f);
    return tmp;
  }

protected:

  template<typename Iter>
  void init_from_string(Iter first, Iter last)
  {
    detail::from_string_converter<unbounded_int> conv;
    conv.detect_properties(first, last);
    conv.convert(*this, first, last);
    if (*this)
      base_type::set_sign_bit(conv.is_positive ? 0 : 1);
  }

  template<typename Iter>
  void init_from_string(Iter first, Iter last, std::ios_base::fmtflags f)
  {
    detail::from_string_converter<unbounded_int> conv;
    conv.detect_properties(first, last, f);
    conv.convert(*this, first, last);
    if (*this)
      base_type::set_sign_bit(conv.is_positive ? 0 : 1);
  }
};


template<class A, class T>
inline void swap(unbounded_int<A,T>& lhs, unbounded_int<A,T>& rhs)
{
  lhs.swap(rhs);
}


template<class A, class T>
unbounded_int<A,T>::unbounded_int()
:
  base_type(0, 0, 0)
{
}

template<class A, class T>
unbounded_int<A,T>::unbounded_int(const allocator_type& a)
:
  base_allocator_type(a),
  base_type(0, 0, 0)
{
}

template<class A, class T>
template<typename IntegralT>
unbounded_int<A,T>::unbounded_int(
    IntegralT b,
    const allocator_type& a,
    typename enable_if<is_integral<IntegralT> >::type*)
:
  base_allocator_type(a),
  base_type(0, 0, 0)
{
  integral_ops<IntegralT>::assign(*this, b);
}

template<class A, class T>
template<typename RandomAccessIterator>
unbounded_int<A,T>::unbounded_int(RandomAccessIterator first,
                                  RandomAccessIterator last,
                                  const allocator_type& a)
:
  base_allocator_type(a),
  base_type(0, 0, 0)
{
  init_from_string(first, last);
}

template<class A, class T>
template<typename charT>
unbounded_int<A,T>::unbounded_int(const charT* s, const allocator_type& a)
:
  base_allocator_type(a),
  base_type(0, 0, 0)
{
  init_from_string(s, s + std::char_traits<charT>::length(s));
}

template<class A, class T>
template<typename charT>
unbounded_int<A,T>::unbounded_int(const charT* s,
                                  std::ios_base::fmtflags f,
                                  const allocator_type& a)
:
  base_allocator_type(a),
  base_type(0, 0, 0)
{
  init_from_string(s, s + std::char_traits<charT>::length(s), f);
}

template<class A, class T>
template<typename charT, class traits, class Alloc>
unbounded_int<A,T>::unbounded_int(
    const std::basic_string<charT,traits,Alloc>& s,
    const allocator_type& a)
:
  base_allocator_type(a),
  base_type(0, 0, 0)
{
  init_from_string(s.begin(), s.end());
}

template<class A, class T>
template<typename charT, class traits, class Alloc>
unbounded_int<A,T>::unbounded_int(
    const std::basic_string<charT,traits,Alloc>& s,
    std::ios_base::fmtflags f,
    const allocator_type& a)
:
  base_allocator_type(a),
  base_type(0, 0, 0)
{
  init_from_string(s.begin(), s.end(), f);
}

template<class A, class T>
template<class ApInt>
unbounded_int<A,T>::unbounded_int(
    const ApInt& x,
    typename enable_if<
      mpl::and_<
        mpl::not_<is_same<traits_type, typename ApInt::traits_type> >,
        is_same<typename ApInt::tag, multiprecision_integer_tag>
      >
    >::type*)
:
  base_type(0, 0, 0)
{
  detail::digit_converter<unbounded_int<A,T>, ApInt>::convert(*this, x);
  //set_sign_bit(x.sign_bit());
}


template<class A, class T>
unbounded_int<A,T>::unbounded_int(const unbounded_int& copy)
:
  base_allocator_type(copy.get_allocator())
{
  base_type::digits_ = this->allocate(copy.size());
  std::memcpy(base_type::digits_,
              copy.digits(), copy.size() * sizeof(digit_type));
  base_type::set_size(copy.size());
  base_type::set_capacity(copy.size());
  base_type::set_sign_bit(copy.sign_bit());
}

#ifndef BOOST_NO_RVALUE_REFERENCES
template<class A, class T>
unbounded_int<A,T>::unbounded_int(unbounded_int&& copy)
:
  base_type(copy.digits(), copy.size(), copy.capacity_)
{
  copy.digits_   = 0;
  copy.size_     = 0;
  copy.capacity_ = 0;
}
#endif


template<class A, class T>
unbounded_int<A,T>::~unbounded_int()
{
  base_type::assert_invariants();
  if (base_type::digits())
    this->deallocate(base_type::digits(), base_type::capacity());
}

template<class A, class T>
unbounded_int<A,T>&
unbounded_int<A,T>::operator = (const unbounded_int<A,T>& rhs)
{
  if (this != &rhs)
  {
    if ((base_type::capacity() == 0) || (base_type::capacity() < rhs.size()))
      unbounded_int(rhs).swap(*this);
    else
    {
      std::memcpy(base_type::digits_,
                  rhs.digits(), rhs.size() * sizeof(digit_type));
      base_type::set_size(rhs.size());
      base_type::set_sign_bit(rhs.sign_bit());
    }
  }
  return *this;
}

#ifndef BOOST_NO_RVALUE_REFERENCES
template<class A, class T>
unbounded_int<A,T>& unbounded_int<A,T>::operator = (unbounded_int<A,T>&& rhs)
{
  if (this != &rhs)
  {
    if (base_type::digits())
      this->deallocate(base_type::digits(), base_type::capacity());
    base_type::digits_   = 0;
    base_type::size_     = 0;
    base_type::capacity_ = 0;
    swap(rhs);
  }
  return *this;
}
#endif

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T>&>::type
unbounded_int<A,T>::operator = (IntegralT rhs)
{
  integral_ops<IntegralT>::assign(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename charT>
unbounded_int<A,T>& unbounded_int<A,T>::operator = (const charT* s)
{
  base_type::set_size(0);
  init_from_string(s, s + std::char_traits<charT>::length(s));
  if (!*this)    // This may happen on the input "0"
    base_type::set_sign(1);
  return *this;
}

template<class A, class T>
template<typename charT, class traits, class Alloc>
unbounded_int<A,T>&
unbounded_int<A,T>::operator = (const std::basic_string<charT,traits,Alloc>& s)
{
  base_type::set_size(0);
  init_from_string(s.begin(), s.end());
  if (!*this)
    base_type::set_sign(1);
  return *this;
}

template<class A, class T>
template<typename charT>
inline void
unbounded_int<A,T>::assign(const charT* s, std::ios_base::fmtflags f)
{
  assign(s, s + std::char_traits<charT>::length(s), f);
}

template<class A, class T>
template<typename charT, class traits, class Alloc>
inline void
unbounded_int<A,T>::assign(const std::basic_string<charT,traits,Alloc>& s,
                    std::ios_base::fmtflags f)
{
  assign(s.begin(), s.end(), f);
}

template<class A, class T>
template<typename RandomAccessIterator>
inline void
unbounded_int<A,T>::assign(RandomAccessIterator first,
                           RandomAccessIterator last,
                           std::ios_base::fmtflags f)
{
  base_type::set_size(0);
  init_from_string(first, last, f);
  if (!*this)
    base_type::set_sign(1);
}

template<class A, class T>
void unbounded_int<A,T>::reserve(size_type n)
{
  if (base_type::capacity() < n)
  {
    const size_type new_cap = base_type::capacity() + base_type::capacity();
    if (new_cap > n)
      n = new_cap;
    digit_type* d = this->allocate(n, base_type::digits_);
    std::memcpy(d, base_type::digits(), sizeof(digit_type) * base_type::size());
    this->deallocate(base_type::digits_, base_type::capacity());
    base_type::digits_ = d;
    base_type::set_capacity(n);
  }
}

// prefix ops
template<class A, class T>
inline unbounded_int<A,T>& unbounded_int<A,T>::operator ++()
{
  reserve(base_type::size() + 1);
  base_type::operator++();
  return *this;
}

template<class A, class T>
inline unbounded_int<A,T>& unbounded_int<A,T>::operator --()
{
  base_type::operator--();
  return *this;
}

// postfix ops
template<class A, class T>
inline unbounded_int<A,T> unbounded_int<A,T>::operator ++(int)
{
  unbounded_int<A,T> tmp(*this);
  ++(*this);
  return tmp;
}

template<class A, class T>
inline unbounded_int<A,T> unbounded_int<A,T>::operator --(int)
{
  unbounded_int<A,T> tmp(*this);
  --(*this);
  return tmp;
}

template<class A, class T>
inline unbounded_int<A,T>& unbounded_int<A,T>::operator <<= (size_type n)
{
  if (*this != digit_type(0))
    detail::shifter<unbounded_int>::shift_bits_left(*this, n);
  return *this;
}

template<class A, class T>
inline unbounded_int<A,T>& unbounded_int<A,T>::operator >>= (size_type n)
{
  detail::shifter<unbounded_int>::shift_bits_right(*this, n);
  if (!*this)
    base_type::set_sign_bit(0);
  return *this;
}

template<class A, class T>
inline unbounded_int<A,T>
operator << (const unbounded_int<A,T>& x,
             typename unbounded_int<A,T>::size_type n)
{
  unbounded_int<A,T> nrv(x);
  nrv <<= n;
  return nrv;
}

template<class A, class T>
inline unbounded_int<A,T>
operator >> (const unbounded_int<A,T>& x,
             typename unbounded_int<A,T>::size_type n)
{
  unbounded_int<A,T> nrv(x);
  nrv >>= n;
  return nrv;
}

template<class A, class T>
inline unbounded_int<A,T>&
unbounded_int<A,T>::operator += (const unbounded_int<A,T>& rhs)
{
  detail::adder<unbounded_int, true>::add(*this, rhs);
  return *this;
}

template<class A, class T>
inline unbounded_int<A,T>&
unbounded_int<A,T>::operator -= (const unbounded_int<A,T>& rhs)
{
  detail::adder<unbounded_int, true>::subtract(*this, rhs);
  return *this;
}

template<class A, class T>
inline unbounded_int<A,T>&
unbounded_int<A,T>::operator *= (const unbounded_int<A,T>& rhs)
{
  detail::multiplier<unbounded_int>::multiply_or_square(*this, rhs);
  return *this;
}

template<class A, class T>
unbounded_int<A,T>&
unbounded_int<A,T>::operator /= (const unbounded_int<A,T>& rhs)
{
  const unbounded_int tmp(*this);
  detail::divider<unbounded_int>::classic_divide(tmp, rhs, *this);
  base_type::set_sign_bit(tmp.sign_bit() ^ rhs.sign_bit());
  return *this;
}

template<class A, class T>
unbounded_int<A,T>&
unbounded_int<A,T>::operator %= (const unbounded_int<A,T>& rhs)
{
  const bool sign = base_type::sign_bit();
  unbounded_int quotient;
  detail::divider<unbounded_int>::classic_divide(*this, rhs, quotient, this);
  if (*this)
    base_type::set_sign_bit(sign);
  return *this;
}

template<class A, class T>
inline unbounded_int<A,T>&
unbounded_int<A,T>::operator |= (const unbounded_int<A,T>& rhs)
{
  detail::bitwise_ops<unbounded_int>::or_bits(*this, *this, rhs);
  return *this;
}

template<class A, class T>
inline unbounded_int<A,T>&
unbounded_int<A,T>::operator &= (const unbounded_int<A,T>& rhs)
{
  detail::bitwise_ops<unbounded_int>::and_bits(*this, *this, rhs);
  return *this;
}

template<class A, class T>
inline unbounded_int<A,T>&
unbounded_int<A,T>::operator ^= (const unbounded_int<A,T>& rhs)
{
  detail::bitwise_ops<unbounded_int>::xor_bits(*this, *this, rhs);
  return *this;
}

template<class A, class T>
inline unbounded_int<A,T> operator + (const unbounded_int<A,T>& lhs,
                                      const unbounded_int<A,T>& rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv += rhs;
  return nrv;
}

template<class A, class T>
inline unbounded_int<A,T> operator - (const unbounded_int<A,T>& lhs,
                                      const unbounded_int<A,T>& rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv -= rhs;
  return nrv;
}

template<class A, class T>
inline unbounded_int<A,T> operator * (const unbounded_int<A,T>& lhs,
                                      const unbounded_int<A,T>& rhs)
{
  unbounded_int<A,T> nrv;
  detail::multiplier<unbounded_int<A,T> >::multiply_or_square(nrv, lhs, rhs);
  return nrv;
}

template<class A, class T>
inline unbounded_int<A,T> operator / (const unbounded_int<A,T>& lhs,
                                      const unbounded_int<A,T>& rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv /= rhs;
  return nrv;
}

template<class A, class T>
inline unbounded_int<A,T> operator % (const unbounded_int<A,T>& lhs,
                                      const unbounded_int<A,T>& rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv %= rhs;
  return nrv;
}

template<class A, class T>
inline unbounded_int<A,T> operator | (const unbounded_int<A,T>& lhs,
                                      const unbounded_int<A,T>& rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv |= rhs;
  return nrv;
}

template<class A, class T>
inline unbounded_int<A,T> operator & (const unbounded_int<A,T>& lhs,
                                      const unbounded_int<A,T>& rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv &= rhs;
  return nrv;
}

template<class A, class T>
inline unbounded_int<A,T> operator ^ (const unbounded_int<A,T>& lhs,
                                      const unbounded_int<A,T>& rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv ^= rhs;
  return nrv;
}

template<class A, class T>
unbounded_int<A,T> operator ~ (const unbounded_int<A,T>& x)
{
  unbounded_int<A,T> nrv;
  nrv.reserve(x.size());
  detail::base::bitwise_ops<unbounded_int<A,T> >::compl_bits(nrv, x);
  return nrv;
}

// Arithmetic and bitwise operators involving integral types
template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T>&>::type
unbounded_int<A,T>::operator += (IntegralT rhs)
{
  integral_ops<IntegralT>::add(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T>&>::type
unbounded_int<A,T>::operator -= (IntegralT rhs)
{
  integral_ops<IntegralT>::subtract(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T>&>::type
unbounded_int<A,T>::operator *= (IntegralT rhs)
{
  integral_ops<IntegralT>::multiply(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T>&>::type
unbounded_int<A,T>::operator /= (IntegralT rhs)
{
  integral_ops<IntegralT>::divide(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T>&>::type
unbounded_int<A,T>::operator %= (IntegralT rhs)
{
  integral_ops<IntegralT>::modulo(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T>&>::type
unbounded_int<A,T>::operator |= (IntegralT rhs)
{
  integral_ops<IntegralT>::bitwise_or(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T>&>::type
unbounded_int<A,T>::operator &= (IntegralT rhs)
{
  integral_ops<IntegralT>::bitwise_and(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T>&>::type
unbounded_int<A,T>::operator ^= (IntegralT rhs)
{
  integral_ops<IntegralT>::bitwise_xor(*this, rhs);
  return *this;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T> >::type
operator + (const unbounded_int<A,T>& lhs, IntegralT rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv += rhs;
  return nrv;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T> >::type
operator - (const unbounded_int<A,T>& lhs, IntegralT rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv -= rhs;
  return nrv;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T> >::type
operator * (const unbounded_int<A,T>& lhs, IntegralT rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv *= rhs;
  return nrv;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T> >::type
operator / (const unbounded_int<A,T>& lhs, IntegralT rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv /= rhs;
  return nrv;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T> >::type
operator % (const unbounded_int<A,T>& lhs, IntegralT rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv %= rhs;
  return nrv;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T> >::type
operator | (const unbounded_int<A,T>& lhs, IntegralT rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv |= rhs;
  return nrv;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T> >::type
operator & (const unbounded_int<A,T>& lhs, IntegralT rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv &= rhs;
  return nrv;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_int<A,T> >::type
operator ^ (const unbounded_int<A,T>& lhs, IntegralT rhs)
{
  unbounded_int<A,T> nrv(lhs);
  nrv ^= rhs;
  return nrv;
}

// compare unbounded_int to unbounded_int
template<class A, class T>
inline bool
operator == (const unbounded_int<A,T>& lhs, const unbounded_int<A,T>& rhs)
{
  return (lhs.sign_bit() == rhs.sign_bit()) &&
         (lhs.size()     == rhs.size()    ) &&
         std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<class A, class T>
inline bool
operator != (const unbounded_int<A,T>& lhs, const unbounded_int<A,T>& rhs)
{
  return !(lhs == rhs);
}

template<class A, class T>
bool
operator < (const unbounded_int<A,T>& lhs, const unbounded_int<A,T>& rhs)
{
  if (lhs.sign_bit() != rhs.sign_bit())
  {
    if (lhs.is_negative())
      return true;
    else
      return false;
  }

  if (lhs.size() < rhs.size())
    return true;
  if (lhs.size() > rhs.size())
    return false;

  if (lhs.is_negative())
    return std::lexicographical_compare(
      rhs.rbegin(), rhs.rend(), lhs.rbegin(), lhs.rend());
  else
    return std::lexicographical_compare(
      lhs.rbegin(), lhs.rend(), rhs.rbegin(), rhs.rend());
}

template<class A, class T>
inline bool
operator > (const unbounded_int<A,T>& lhs, const unbounded_int<A,T>& rhs)
{
  return rhs < lhs;
}

template<class A, class T>
inline bool
operator <= (const unbounded_int<A,T>& lhs, const unbounded_int<A,T>& rhs)
{
  return !(rhs < lhs);
}

template<class A, class T>
inline bool
operator >= (const unbounded_int<A,T>& lhs, const unbounded_int<A,T>& rhs)
{
  return !(lhs < rhs);
}

// compare unbounded_int to integral
template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator == (const unbounded_int<A,T>& lhs, IntegralT rhs)
{
  return detail::unbounded_int_integral_ops<
    unbounded_int<A,T>, IntegralT>::equal(lhs, rhs);
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator != (const unbounded_int<A,T>& lhs, IntegralT rhs)
{
  return !(lhs == rhs);
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator < (const unbounded_int<A,T>& lhs, IntegralT rhs)
{
  return detail::unbounded_int_integral_ops<
    unbounded_int<A,T>, IntegralT>::less(lhs, rhs);
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator > (const unbounded_int<A,T>& lhs, IntegralT rhs)
{
  return rhs < lhs;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator <= (const unbounded_int<A,T>& lhs, IntegralT rhs)
{
  return (lhs < rhs) || (lhs == rhs);
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator >= (const unbounded_int<A,T>& lhs, IntegralT rhs)
{
  return !(lhs < rhs);
}

// compare integral to unbounded_int
template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator == (IntegralT lhs, const unbounded_int<A,T>& rhs)
{
  return rhs == lhs;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator != (IntegralT lhs, const unbounded_int<A,T>& rhs)
{
  return !(lhs == rhs);
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator < (IntegralT lhs, const unbounded_int<A,T>& rhs)
{
  return !(rhs <= lhs);
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator > (IntegralT lhs, const unbounded_int<A,T>& rhs)
{
  return rhs < lhs;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator <= (IntegralT lhs, const unbounded_int<A,T>& rhs)
{
  return !(rhs < lhs);
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator >= (IntegralT lhs, const unbounded_int<A,T>& rhs)
{
  return rhs <= lhs;
}

// compare unbounded_int to const charT*
template<class A, class T, typename charT>
inline bool
operator == (const unbounded_int<A,T>& lhs, const charT* rhs)
{
  return lhs == unbounded_int<A,T>(rhs);
}

template<class A, class T, typename charT>
inline bool
operator != (const unbounded_int<A,T>& lhs, const charT* rhs)
{
  return lhs != unbounded_int<A,T>(rhs);
}

template<class A, class T, typename charT>
inline bool
operator < (const unbounded_int<A,T>& lhs, const charT* rhs)
{
  return lhs < unbounded_int<A,T>(rhs);
}

template<class A, class T, typename charT>
inline bool
operator > (const unbounded_int<A,T>& lhs, const charT* rhs)
{
  return lhs > unbounded_int<A,T>(rhs);
}

template<class A, class T, typename charT>
inline bool
operator <= (const unbounded_int<A,T>& lhs, const charT* rhs)
{
  return lhs <= unbounded_int<A,T>(rhs);
}

template<class A, class T, typename charT>
inline bool
operator >= (const unbounded_int<A,T>& lhs, const charT* rhs)
{
  return lhs >= unbounded_int<A,T>(rhs);
}

// comparison const charT* to unbounded_int
template<class A, class T, typename charT>
inline bool
operator == (const charT* lhs, const unbounded_int<A,T>& rhs)
{
  return unbounded_int<A,T>(lhs) == rhs;
}

template<class A, class T, typename charT>
inline bool
operator != (const charT* lhs, const unbounded_int<A,T>& rhs)
{
  return unbounded_int<A,T>(lhs) != rhs;
}

template<class A, class T, typename charT>
inline bool
operator < (const charT* lhs, const unbounded_int<A,T>& rhs)
{
  return unbounded_int<A,T>(lhs) < rhs;
}

template<class A, class T, typename charT>
inline bool
operator > (const charT* lhs, const unbounded_int<A,T>& rhs)
{
  return unbounded_int<A,T>(lhs) > rhs;
}

template<class A, class T, typename charT>
inline bool
operator <= (const charT* lhs, const unbounded_int<A,T>& rhs)
{
  return unbounded_int<A,T>(lhs) <= rhs;
}

template<class A, class T, typename charT>
inline bool
operator >= (const charT* lhs, const unbounded_int<A,T>& rhs)
{
  return unbounded_int<A,T>(lhs) >= rhs;
}

// compare unbounded_int to basic_string
template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator == (const unbounded_int<A,T>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs == unbounded_int<A,T>(rhs);
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator != (const unbounded_int<A,T>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs != unbounded_int<A,T>(rhs);
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator < (const unbounded_int<A,T>& lhs,
            const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs < unbounded_int<A,T>(rhs);
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator > (const unbounded_int<A,T>& lhs,
            const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs > unbounded_int<A,T>(rhs);
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator <= (const unbounded_int<A,T>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs <= unbounded_int<A,T>(rhs);
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator >= (const unbounded_int<A,T>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs >= unbounded_int<A,T>(rhs);
}

// compare basic_string to unbounded_int
template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator == (const std::basic_string<charT,Traits,Alloc>& lhs,
             const unbounded_int<A,T>& rhs)
{
  return unbounded_int<A,T>(lhs) == rhs;
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator != (const std::basic_string<charT,Traits,Alloc>& lhs,
             const unbounded_int<A,T>& rhs)
{
  return unbounded_int<A,T>(lhs) != rhs;
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator < (const std::basic_string<charT,Traits,Alloc>& lhs,
            const unbounded_int<A,T>& rhs)
{
  return unbounded_int<A,T>(lhs) < rhs;
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator > (const std::basic_string<charT,Traits,Alloc>& lhs,
            const unbounded_int<A,T>& rhs)
{
  return unbounded_int<A,T>(lhs) > rhs;
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator <= (const std::basic_string<charT,Traits,Alloc>& lhs,
             const unbounded_int<A,T>& rhs)
{
  return unbounded_int<A,T>(lhs) <= rhs;
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator >= (const std::basic_string<charT,Traits,Alloc>& lhs,
             const unbounded_int<A,T>& rhs)
{
  return unbounded_int<A,T>(lhs) >= rhs;
}

// Input/Output
template<class A, class T, typename charT, class traits>
std::basic_istream<charT, traits>&
operator >> (std::basic_istream<charT, traits>& is, unbounded_int<A,T>& x)
{
  detail::stream_io<unbounded_int<A,T> >::read(x, is);
  return is;
}

template<class A, class T, typename charT, class traits>
std::basic_ostream<charT, traits>&
operator << (std::basic_ostream<charT, traits>& os, const unbounded_int<A,T>& x)
{
  // TODO same as above, we should output digits directly to the stream
  return os << x.template to_string<std::string>(os.flags());
}


template<class A, class T>
unbounded_int<A,T> abs(const unbounded_int<A,T>& x)
{
  unbounded_int<A,T> tmp(x);
  tmp.set_sign_bit(0);
  return tmp;
}

template<class A, class T>
inline
unbounded_int<A,T> gcd(const unbounded_int<A,T>& a,
                       const unbounded_int<A,T>& b)
{
  unbounded_int<A,T> z;
  detail::gcd_finder<unbounded_int<A,T> >::gcd(z, a, b);
  return z;
}

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template<class A, class T, class... Integer>
unbounded_int<A,T> gcd(const unbounded_int<A,T>& a,
                       const unbounded_int<A,T>& b,
                       const Integer&... args)
{
  return gcd(gcd(a, b), args...);
}
#endif

template<class A, class T>
inline
unbounded_int<A,T> lcm(const unbounded_int<A,T>& a,
                       const unbounded_int<A,T>& b)
{
  unbounded_int<A,T> z;
  detail::lcm_finder<unbounded_int<A,T> >::lcm(z, a, b);
  return z;
}

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template<class A, class T, class... Integer>
unbounded_int<A,T> lcm(const unbounded_int<A,T>& a,
                       const unbounded_int<A,T>& b,
                       const Integer&... args)
{
  return lcm(lcm(a, b), args...);
}
#endif

template<class A, class T>
inline
unbounded_int<A,T>
pow(const unbounded_int<A,T>& x, typename unbounded_int<A,T>::size_type y)
{
  unbounded_int<A,T> z;
  detail::power<unbounded_int<A,T> >::pow(z, x, y);
  return z;
}

template<class A, class T>
inline
unbounded_int<A,T>
pow(const unbounded_int<A,T>& x, const unbounded_int<A,T>& y)
{
  unbounded_int<A,T> z;
  detail::power<unbounded_int<A,T> >::pow(z, x, y);
  return z;
}

template<class A, class T>
inline
unbounded_int<A,T> sqrt(const unbounded_int<A,T>& x)
{
  unbounded_int<A,T> z;
  detail::root<unbounded_int<A,T> >::sqrt(z, x);
  return z;
}

template<class A, class T>
inline
unbounded_int<A,T> nth_root(typename unbounded_int<A,T>::size_type n,
                            const unbounded_int<A,T>& x)
{
  unbounded_int<A,T> z;
  detail::root<unbounded_int<A,T> >::nth_root(z, n, x);
  return z;
}

template<class A, class T>
inline
unbounded_int<A,T> nth_root(const unbounded_int<A,T>& n,
                            const unbounded_int<A,T>& x)
{
  unbounded_int<A,T> z;
  detail::root<unbounded_int<A,T> >::nth_root(z, n, x);
  return z;
}

template<class A, class T>
inline
unbounded_int<A,T> modinv(const unbounded_int<A,T>& x,
                          const unbounded_int<A,T>& m)
{
  unbounded_int<A,T> nrv;
  detail::modular_inverter<unbounded_int<A,T> >::modinv(nrv, x, m);
  return nrv;
}

template<class A, class T>
struct modpow_ctx<unbounded_int<A,T> >
:
  detail::modpow_ctx<unbounded_int<A,T> >
{};

// returns base^exp % mod
template<class A, class T>
inline
unbounded_int<A,T> modpow(const unbounded_int<A,T>& base,
                          const unbounded_int<A,T>& exp,
                          const unbounded_int<A,T>& mod,
                          modpow_ctx<unbounded_int<A,T> >* ctx = 0)
{
  unbounded_int<A,T> z;
  detail::modular_power<unbounded_int<A,T> >::modpow(z, base, exp, mod, ctx);
  return z;
}

template<class A, class T>
inline
int jacobi(const unbounded_int<A,T>& x, const unbounded_int<A,T>& y)
{
  return detail::jacobi(x, y);
}



} // namespace mp_math
} // namespace boost

#endif

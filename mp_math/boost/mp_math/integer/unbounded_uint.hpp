// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_UNBOUNDED_UINT_HPP
#define BOOST_MP_MATH_INTEGER_UNBOUNDED_UINT_HPP

#include <boost/config.hpp>
#include <boost/mp_math/integer/contexts.hpp>
#include <boost/mp_math/integer/multiprecision_integer_tag.hpp>
#include <boost/mp_math/integer/unbounded_traits.hpp>
#include <boost/mp_math/integer/detail/adder.hpp>
#include <boost/mp_math/integer/detail/bitwise_ops.hpp>
#include <boost/mp_math/integer/detail/divider.hpp>
#include <boost/mp_math/integer/detail/gcd.hpp>
#include <boost/mp_math/integer/detail/jacobi.hpp>
#include <boost/mp_math/integer/detail/lcm.hpp>
#include <boost/mp_math/integer/detail/multiplier.hpp>
#include <boost/mp_math/integer/detail/power.hpp>
#include <boost/mp_math/integer/detail/root.hpp>
#include <boost/mp_math/integer/detail/stream_io.hpp>
#include <boost/mp_math/integer/detail/string_conversion.hpp>
#include <boost/mp_math/integer/detail/unbounded_uint_integral.hpp>
#include <boost/mp_math/integer/detail/base/unbounded_uint.hpp>

namespace boost {
namespace mp_math {

template<
  class Alloc = std::allocator<void>,
  class Traits = unbounded_traits<>
>
struct unbounded_uint
:
  Alloc::template rebind<typename Traits::digit_type>::other,
  detail::base::unbounded_uint<Traits>
{
protected:

  typedef detail::base::unbounded_uint<Traits> base_type;

  typedef typename Alloc::template
    rebind<typename Traits::digit_type>::other base_allocator_type;

public:

  typedef multiprecision_integer_tag tag;

  template<typename IntegralT>
  struct integral_ops
  :
    detail::unbounded_uint_integral_ops<unbounded_uint, IntegralT>
  {};

  typedef Alloc                              allocator_type;
  typedef Traits                             traits_type;
  typedef typename traits_type::digit_type   digit_type;
  typedef typename allocator_type::size_type size_type;

  typedef typename base_type::iterator               iterator;
  typedef typename base_type::const_iterator         const_iterator;
  typedef typename base_type::reverse_iterator       reverse_iterator;
  typedef typename base_type::const_reverse_iterator const_reverse_iterator;

  unbounded_uint();

  explicit unbounded_uint(const allocator_type& a);

  template<typename IntegralT>
  unbounded_uint(IntegralT,
         const allocator_type& a = allocator_type(),
         typename enable_if<is_integral<IntegralT> >::type* dummy = 0);

  template<typename charT>
  unbounded_uint(const charT*, const allocator_type& a = allocator_type());

  template<typename charT>
  unbounded_uint(const charT*,
                 std::ios_base::fmtflags,
                 const allocator_type& a = allocator_type());

  template<typename charT, class traits, class alloc>
  unbounded_uint(const std::basic_string<charT,traits,alloc>&,
                 const allocator_type& a = allocator_type());

  template<typename charT, class traits, class alloc>
  unbounded_uint(const std::basic_string<charT,traits,alloc>&,
                 std::ios_base::fmtflags,
                 const allocator_type& a = allocator_type());

  template<typename RandomAccessIterator>
  unbounded_uint(RandomAccessIterator first,
                 RandomAccessIterator last,
                 const allocator_type& a = allocator_type());

  template<typename RandomAccessIterator>
  unbounded_uint(RandomAccessIterator first,
                 RandomAccessIterator last,
                 std::ios_base::fmtflags f,
                 const allocator_type& a = allocator_type());

  unbounded_uint(const unbounded_uint& copy);

  #ifndef BOOST_NO_RVALUE_REFERENCES
  unbounded_uint(unbounded_uint&& copy);
  #endif

  ~unbounded_uint();

  unbounded_uint& operator = (const unbounded_uint& rhs);

  #ifndef BOOST_NO_RVALUE_REFERENCES
  unbounded_uint& operator = (unbounded_uint&& rhs);
  #endif

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint&>::type
  operator = (IntegralT rhs);

  template<typename charT>
  unbounded_uint& operator = (const charT*);

  template<typename charT, class traits, class alloc>
  unbounded_uint& operator = (const std::basic_string<charT,traits,alloc>&);

  template<typename charT>
  void assign(const charT*, std::ios_base::fmtflags);

  template<typename charT, class traits, class alloc>
  void assign(const std::basic_string<charT,traits,alloc>&,
              std::ios_base::fmtflags);

  template<typename RandomAccessIterator>
  void assign(RandomAccessIterator first, RandomAccessIterator last,
              std::ios_base::fmtflags);

  void swap(unbounded_uint& other)
  {
    base_type::swap(other);
  }

  allocator_type get_allocator() const { return allocator_type(); }

  void reserve(size_type n);

  unbounded_uint& operator ++();
  unbounded_uint& operator --();
  unbounded_uint  operator ++(int);
  unbounded_uint  operator --(int);
  unbounded_uint& operator <<= (size_type);
  unbounded_uint& operator >>= (size_type);

  unbounded_uint& operator += (const unbounded_uint&);
  unbounded_uint& operator -= (const unbounded_uint&);
  unbounded_uint& operator *= (const unbounded_uint&);
  unbounded_uint& operator /= (const unbounded_uint&);
  unbounded_uint& operator %= (const unbounded_uint&);
  unbounded_uint& operator |= (const unbounded_uint&);
  unbounded_uint& operator &= (const unbounded_uint&);
  unbounded_uint& operator ^= (const unbounded_uint&);

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint&>::type
  operator += (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint&>::type
  operator -= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint&>::type
  operator *= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint&>::type
  operator /= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint&>::type
  operator %= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint&>::type
  operator |= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint&>::type
  operator &= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, unbounded_uint&>::type
  operator ^= (IntegralT);

  template<typename charT> unbounded_uint& operator += (const charT*);
  template<typename charT> unbounded_uint& operator -= (const charT*);
  template<typename charT> unbounded_uint& operator *= (const charT*);
  template<typename charT> unbounded_uint& operator /= (const charT*);
  template<typename charT> unbounded_uint& operator %= (const charT*);
  template<typename charT> unbounded_uint& operator |= (const charT*);
  template<typename charT> unbounded_uint& operator &= (const charT*);
  template<typename charT> unbounded_uint& operator ^= (const charT*);

  template<typename charT, class traits, class alloc>
  unbounded_uint& operator += (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_uint& operator -= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_uint& operator *= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_uint& operator /= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_uint& operator %= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_uint& operator |= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_uint& operator &= (const std::basic_string<charT,traits,alloc>&);
  template<typename charT, class traits, class alloc>
  unbounded_uint& operator ^= (const std::basic_string<charT,traits,alloc>&);

  template<class StringT>
  StringT to_string(std::ios_base::fmtflags f = std::ios_base::dec) const
  {
    StringT tmp;
    detail::to_string_converter<unbounded_uint> conv;
    conv.convert(tmp, *this, f);
    return tmp;
  }

protected:

  template<typename Iter>
  void init_from_string(Iter first, Iter last)
  {
    if (first < last)
    {
      detail::from_string_converter<unbounded_uint> conv;
      conv.detect_properties(first, last);
      if (!conv.is_positive)
        throw std::invalid_argument("unbounded_uint::init_from_string: "
            "cannot convert negative number to unsigned integer");
      conv.convert(*this, first, last);
    }
  }

  template<typename Iter>
  void init_from_string(Iter first, Iter last, std::ios_base::fmtflags f)
  {
    if (first < last)
    {
      detail::from_string_converter<unbounded_uint> conv;
      conv.detect_properties(first, last, f);
      if (!conv.is_positive)
        throw std::invalid_argument("unbounded_uint::init_from_string: "
            "cannot convert negative number to unsigned integer");
      conv.convert(*this, first, last);
    }
  }
};


template<class A, class T>
unbounded_uint<A,T>::unbounded_uint()
:
  base_type(0, 0, 0)
{
}

template<class A, class T>
unbounded_uint<A,T>::unbounded_uint(const allocator_type& a)
:
  base_allocator_type(a),
  base_type(0, 0, 0)
{
}

template<class A, class T>
template<typename IntegralT>
unbounded_uint<A,T>::unbounded_uint(
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
unbounded_uint<A,T>::unbounded_uint(RandomAccessIterator first,
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
unbounded_uint<A,T>::unbounded_uint(const charT* s, const allocator_type& a)
:
  base_allocator_type(a),
  base_type(0, 0, 0)
{
  init_from_string(s, s + std::char_traits<charT>::length(s));
}

template<class A, class T>
template<typename charT>
unbounded_uint<A,T>::unbounded_uint(const charT* s,
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
unbounded_uint<A,T>::unbounded_uint(
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
unbounded_uint<A,T>::unbounded_uint(
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
unbounded_uint<A,T>::unbounded_uint(const unbounded_uint& copy)
:
  base_allocator_type(copy.get_allocator())
{
  base_type::digits_ = this->allocate(copy.size());
  std::memcpy(base_type::digits_,
              copy.digits(), copy.size() * sizeof(digit_type));
  base_type::set_size(copy.size());
  base_type::set_capacity(copy.size());
}

#ifndef BOOST_NO_RVALUE_REFERENCES
template<class A, class T>
unbounded_uint<A,T>::unbounded_uint(unbounded_uint&& copy)
:
  base_type(copy.digits(), copy.size(), copy.capacity())
{
  copy.digits_   = 0;
  copy.size_     = 0;
  copy.capacity_ = 0;
}
#endif


template<class A, class T>
unbounded_uint<A,T>::~unbounded_uint()
{
  base_type::assert_invariants();
  if (base_type::digits())
    this->deallocate(base_type::digits(), base_type::capacity());
}

template<class A, class T>
unbounded_uint<A,T>&
unbounded_uint<A,T>::operator = (const unbounded_uint<A,T>& rhs)
{
  if (this != &rhs)
  {
    if ((base_type::capacity() == 0) || (base_type::capacity() < rhs.size()))
      unbounded_uint(rhs).swap(*this);
    else
    {
      std::memcpy(base_type::digits_,
                  rhs.digits(), rhs.size() * sizeof(digit_type));
      base_type::set_size(rhs.size());
    }
  }
  return *this;
}

#ifndef BOOST_NO_RVALUE_REFERENCES
template<class A, class T>
unbounded_uint<A,T>& unbounded_uint<A,T>::operator = (unbounded_uint<A,T>&& rhs)
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
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T>&>::type
unbounded_uint<A,T>::operator = (IntegralT rhs)
{
  integral_ops<IntegralT>::assign(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename charT>
unbounded_uint<A,T>& unbounded_uint<A,T>::operator = (const charT* s)
{
  base_type::set_size(0);
  init_from_string(s, s + std::char_traits<charT>::length(s));
  return *this;
}

template<class A, class T>
template<typename charT, class traits, class Alloc>
unbounded_uint<A,T>&
unbounded_uint<A,T>::operator = (const std::basic_string<charT,traits,Alloc>& s)
{
  base_type::set_size(0);
  init_from_string(s.begin(), s.end());
  return *this;
}

template<class A, class T>
template<typename charT>
inline void
unbounded_uint<A,T>::assign(const charT* s, std::ios_base::fmtflags f)
{
  assign(s, s + std::char_traits<charT>::length(s), f);
}

template<class A, class T>
template<typename charT, class traits, class Alloc>
inline void
unbounded_uint<A,T>::assign(const std::basic_string<charT,traits,Alloc>& s,
                            std::ios_base::fmtflags f)
{
  assign(s.begin(), s.end(), f);
}

template<class A, class T>
template<typename RandomAccessIterator>
inline void
unbounded_uint<A,T>::assign(RandomAccessIterator first,
                            RandomAccessIterator last,
                            std::ios_base::fmtflags f)
{
  base_type::set_size(0);
  init_from_string(first, last, f);
;
}

template<class A, class T>
void unbounded_uint<A,T>::reserve(size_type n)
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
inline unbounded_uint<A,T>& unbounded_uint<A,T>::operator ++()
{
  reserve(base_type::size() + 1);
  base_type::operator++();
  return *this;
}

template<class A, class T>
inline unbounded_uint<A,T>& unbounded_uint<A,T>::operator --()
{
  base_type::operator--();
  return *this;
}

// postfix ops
template<class A, class T>
inline unbounded_uint<A,T> unbounded_uint<A,T>::operator ++(int)
{
  unbounded_uint<A,T> tmp(*this);
  ++(*this);
  return tmp;
}

template<class A, class T>
inline unbounded_uint<A,T> unbounded_uint<A,T>::operator --(int)
{
  unbounded_uint<A,T> tmp(*this);
  --(*this);
  return tmp;
}

template<class A, class T>
inline unbounded_uint<A,T>& unbounded_uint<A,T>::operator <<= (size_type n)
{
  if (*this != digit_type(0))
    detail::shifter<unbounded_uint>::shift_bits_left(*this, n);
  return *this;
}

template<class A, class T>
inline unbounded_uint<A,T>& unbounded_uint<A,T>::operator >>= (size_type n)
{
  detail::shifter<unbounded_uint>::shift_bits_right(*this, n);
  return *this;
}

template<class A, class T>
inline unbounded_uint<A,T>
operator << (const unbounded_uint<A,T>& x,
             typename unbounded_uint<A,T>::size_type n)
{
  unbounded_uint<A,T> nrv(x);
  nrv <<= n;
  return nrv;
}

template<class A, class T>
inline unbounded_uint<A,T>
operator >> (const unbounded_uint<A,T>& x,
             typename unbounded_uint<A,T>::size_type n)
{
  unbounded_uint<A,T> nrv(x);
  nrv >>= n;
  return nrv;
}

template<class A, class T>
inline unbounded_uint<A,T>&
unbounded_uint<A,T>::operator += (const unbounded_uint<A,T>& rhs)
{
  detail::adder<unbounded_uint, false>::add(*this, rhs);
  return *this;
}

template<class A, class T>
inline unbounded_uint<A,T>&
unbounded_uint<A,T>::operator -= (const unbounded_uint<A,T>& rhs)
{
  detail::adder<unbounded_uint, false>::subtract(*this, rhs);
  return *this;
}

template<class A, class T>
inline unbounded_uint<A,T>&
unbounded_uint<A,T>::operator *= (const unbounded_uint<A,T>& rhs)
{
  detail::multiplier<unbounded_uint>::multiply_or_square(*this, rhs);
  return *this;
}

template<class A, class T>
unbounded_uint<A,T>&
unbounded_uint<A,T>::operator /= (const unbounded_uint<A,T>& rhs)
{
  const unbounded_uint tmp(*this);
  detail::divider<unbounded_uint>::classic_divide(tmp, rhs, *this);
  return *this;
}

template<class A, class T>
unbounded_uint<A,T>&
unbounded_uint<A,T>::operator %= (const unbounded_uint<A,T>& rhs)
{
  unbounded_uint quotient;
  detail::divider<unbounded_uint>::classic_divide(*this, rhs, quotient, this);
  return *this;
}

template<class A, class T>
inline unbounded_uint<A,T>&
unbounded_uint<A,T>::operator |= (const unbounded_uint<A,T>& rhs)
{
  detail::bitwise_ops<unbounded_uint>::or_bits(*this, *this, rhs);
  return *this;
}

template<class A, class T>
inline unbounded_uint<A,T>&
unbounded_uint<A,T>::operator &= (const unbounded_uint<A,T>& rhs)
{
  detail::bitwise_ops<unbounded_uint>::and_bits(*this, *this, rhs);
  return *this;
}

template<class A, class T>
inline unbounded_uint<A,T>&
unbounded_uint<A,T>::operator ^= (const unbounded_uint<A,T>& rhs)
{
  detail::bitwise_ops<unbounded_uint>::xor_bits(*this, *this, rhs);
  return *this;
}

template<class A, class T>
inline unbounded_uint<A,T> operator + (const unbounded_uint<A,T>& lhs,
                                       const unbounded_uint<A,T>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv += rhs;
  return nrv;
}

template<class A, class T>
inline unbounded_uint<A,T> operator - (const unbounded_uint<A,T>& lhs,
                                       const unbounded_uint<A,T>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv -= rhs;
  return nrv;
}

template<class A, class T>
inline unbounded_uint<A,T> operator * (const unbounded_uint<A,T>& lhs,
                                       const unbounded_uint<A,T>& rhs)
{
  unbounded_uint<A,T> nrv;
  detail::multiplier<unbounded_uint<A,T> >::multiply_or_square(nrv, lhs, rhs);
  return nrv;
}

template<class A, class T>
inline unbounded_uint<A,T> operator / (const unbounded_uint<A,T>& lhs,
                                       const unbounded_uint<A,T>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv /= rhs;
  return nrv;
}

template<class A, class T>
inline unbounded_uint<A,T> operator % (const unbounded_uint<A,T>& lhs,
                                       const unbounded_uint<A,T>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv %= rhs;
  return nrv;
}

// TODO can do optimization here depending on which arg is larger, then
// construct nrv from the larger to avoid a reallocation in op <<=. Same for
// other args. The specific optimization that is possible depends on the
// kind of operator we are looking at. That also means that we would need to
// forward to these ops directly in integer.hpp. The same can be done for shift
// operators << and >>.
template<class A, class T>
inline unbounded_uint<A,T> operator | (const unbounded_uint<A,T>& lhs,
                                       const unbounded_uint<A,T>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv |= rhs;
  return nrv;
}

template<class A, class T>
inline unbounded_uint<A,T> operator & (const unbounded_uint<A,T>& lhs,
                                       const unbounded_uint<A,T>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv &= rhs;
  return nrv;
}

template<class A, class T>
inline unbounded_uint<A,T> operator ^ (const unbounded_uint<A,T>& lhs,
                                       const unbounded_uint<A,T>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv ^= rhs;
  return nrv;
}

template<class A, class T>
unbounded_uint<A,T> operator ~ (const unbounded_uint<A,T>& x)
{
  unbounded_uint<A,T> nrv;
  nrv.reserve(x.size());
  detail::base::bitwise_ops<unbounded_uint<A,T> >::compl_bits(nrv, x);
  return nrv;
}

// Arithmetic and bitwise operators involving integral types
template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T>&>::type
unbounded_uint<A,T>::operator += (IntegralT rhs)
{
  integral_ops<IntegralT>::add(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T>&>::type
unbounded_uint<A,T>::operator -= (IntegralT rhs)
{
  integral_ops<IntegralT>::subtract(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T>&>::type
unbounded_uint<A,T>::operator *= (IntegralT rhs)
{
  integral_ops<IntegralT>::multiply(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T>&>::type
unbounded_uint<A,T>::operator /= (IntegralT rhs)
{
  integral_ops<IntegralT>::divide(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T>&>::type
unbounded_uint<A,T>::operator %= (IntegralT rhs)
{
  integral_ops<IntegralT>::modulo(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T>&>::type
unbounded_uint<A,T>::operator |= (IntegralT rhs)
{
  integral_ops<IntegralT>::bitwise_or(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T>&>::type
unbounded_uint<A,T>::operator &= (IntegralT rhs)
{
  integral_ops<IntegralT>::bitwise_and(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T>&>::type
unbounded_uint<A,T>::operator ^= (IntegralT rhs)
{
  integral_ops<IntegralT>::bitwise_xor(*this, rhs);
  return *this;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T> >::type
operator + (const unbounded_uint<A,T>& lhs, IntegralT rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv += rhs;
  return nrv;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T> >::type
operator - (const unbounded_uint<A,T>& lhs, IntegralT rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv -= rhs;
  return nrv;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T> >::type
operator * (const unbounded_uint<A,T>& lhs, IntegralT rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv *= rhs;
  return nrv;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T> >::type
operator / (const unbounded_uint<A,T>& lhs, IntegralT rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv /= rhs;
  return nrv;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T> >::type
operator % (const unbounded_uint<A,T>& lhs, IntegralT rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv %= rhs;
  return nrv;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T> >::type
operator | (const unbounded_uint<A,T>& lhs, IntegralT rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv |= rhs;
  return nrv;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T> >::type
operator & (const unbounded_uint<A,T>& lhs, IntegralT rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv &= rhs;
  return nrv;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, unbounded_uint<A,T> >::type
operator ^ (const unbounded_uint<A,T>& lhs, IntegralT rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv ^= rhs;
  return nrv;
}

// Arithmetic and bitwise operators involving character strings
template<class A, class T>
template<typename charT>
inline unbounded_uint<A,T>& unbounded_uint<A,T>::operator += (const charT* s)
{
  return *this += unbounded_uint<A,T>(s);
}

template<class A, class T>
template<typename charT>
inline unbounded_uint<A,T>& unbounded_uint<A,T>::operator -= (const charT* s)
{
  return *this -= unbounded_uint<A,T>(s);
}

template<class A, class T>
template<typename charT>
inline unbounded_uint<A,T>& unbounded_uint<A,T>::operator *= (const charT* s)
{
  return *this *= unbounded_uint<A,T>(s);
}

template<class A, class T>
template<typename charT>
inline unbounded_uint<A,T>& unbounded_uint<A,T>::operator /= (const charT* s)
{
  return *this /= unbounded_uint<A,T>(s);
}

template<class A, class T>
template<typename charT>
inline unbounded_uint<A,T>& unbounded_uint<A,T>::operator %= (const charT* s)
{
  return *this %= unbounded_uint<A,T>(s);
}

template<class A, class T>
template<typename charT>
inline unbounded_uint<A,T>& unbounded_uint<A,T>::operator |= (const charT* s)
{
  return *this |= unbounded_uint<A,T>(s);
}

template<class A, class T>
template<typename charT>
inline unbounded_uint<A,T>& unbounded_uint<A,T>::operator &= (const charT* s)
{
  return *this &= unbounded_uint<A,T>(s);
}

template<class A, class T>
template<typename charT>
inline unbounded_uint<A,T>& unbounded_uint<A,T>::operator ^= (const charT* s)
{
  return *this ^= unbounded_uint<A,T>(s);
}

template<class A, class T, typename charT>
inline
unbounded_uint<A,T> operator + (const unbounded_uint<A,T>& lhs, const charT* rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv += unbounded_uint<A,T>(rhs);
  return nrv;
}

template<class A, class T, typename charT>
inline
unbounded_uint<A,T> operator - (const unbounded_uint<A,T>& lhs, const charT* rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv -= unbounded_uint<A,T>(rhs);
  return nrv;
}

template<class A, class T, typename charT>
inline
unbounded_uint<A,T> operator * (const unbounded_uint<A,T>& lhs, const charT* rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv *= unbounded_uint<A,T>(rhs);
  return nrv;
}

template<class A, class T, typename charT>
inline
unbounded_uint<A,T> operator / (const unbounded_uint<A,T>& lhs, const charT* rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv /= unbounded_uint<A,T>(rhs);
  return nrv;
}

template<class A, class T, typename charT>
inline
unbounded_uint<A,T> operator % (const unbounded_uint<A,T>& lhs, const charT* rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv %= unbounded_uint<A,T>(rhs);
  return nrv;
}

template<class A, class T, typename charT>
inline
unbounded_uint<A,T> operator | (const unbounded_uint<A,T>& lhs, const charT* rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv |= unbounded_uint<A,T>(rhs);
  return nrv;
}

template<class A, class T, typename charT>
inline
unbounded_uint<A,T> operator & (const unbounded_uint<A,T>& lhs, const charT* rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv &= unbounded_uint<A,T>(rhs);
  return nrv;
}

template<class A, class T, typename charT>
inline
unbounded_uint<A,T> operator ^ (const unbounded_uint<A,T>& lhs, const charT* rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv ^= unbounded_uint<A,T>(rhs);
  return nrv;
}


// Arithmetic and bitwise operators involving basic_string
template<class A, class T>
template<typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>&
unbounded_uint<A,T>::operator += (const std::basic_string<charT,traits,Alloc>& s)
{
  return *this += unbounded_uint<A,T>(s);
}

template<class A, class T>
template<typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>&
unbounded_uint<A,T>::operator -= (const std::basic_string<charT,traits,Alloc>& s)
{
  return *this -= unbounded_uint<A,T>(s);
}

template<class A, class T>
template<typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>&
unbounded_uint<A,T>::operator *= (const std::basic_string<charT,traits,Alloc>& s)
{
  return *this *= unbounded_uint<A,T>(s);
}

template<class A, class T>
template<typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>&
unbounded_uint<A,T>::operator /= (const std::basic_string<charT,traits,Alloc>& s)
{
  return *this /= unbounded_uint<A,T>(s);
}

template<class A, class T>
template<typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>&
unbounded_uint<A,T>::operator %= (const std::basic_string<charT,traits,Alloc>& s)
{
  return *this %= unbounded_uint<A,T>(s);
}

template<class A, class T>
template<typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>&
unbounded_uint<A,T>::operator |= (const std::basic_string<charT,traits,Alloc>& s)
{
  return *this |= unbounded_uint<A,T>(s);
}

template<class A, class T>
template<typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>&
unbounded_uint<A,T>::operator &= (const std::basic_string<charT,traits,Alloc>& s)
{
  return *this &= unbounded_uint<A,T>(s);
}

template<class A, class T>
template<typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>&
unbounded_uint<A,T>::operator ^= (const std::basic_string<charT,traits,Alloc>& s)
{
  return *this ^= unbounded_uint<A,T>(s);
}

template<class A, class T, typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>
operator + (const unbounded_uint<A,T>& lhs,
            const std::basic_string<charT,traits,Alloc>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv += unbounded_uint<A,T>(rhs);
  return nrv;
}

template<class A, class T, typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>
operator - (const unbounded_uint<A,T>& lhs,
            const std::basic_string<charT,traits,Alloc>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv -= unbounded_uint<A,T>(rhs);
  return nrv;
}

template<class A, class T, typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>
operator * (const unbounded_uint<A,T>& lhs,
            const std::basic_string<charT,traits,Alloc>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv *= unbounded_uint<A,T>(rhs);
  return nrv;
}

template<class A, class T, typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>
operator / (const unbounded_uint<A,T>& lhs,
            const std::basic_string<charT,traits,Alloc>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv /= unbounded_uint<A,T>(rhs);
  return nrv;
}

template<class A, class T, typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>
operator % (const unbounded_uint<A,T>& lhs,
            const std::basic_string<charT,traits,Alloc>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv %= unbounded_uint<A,T>(rhs);
  return nrv;
}

template<class A, class T, typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>
operator | (const unbounded_uint<A,T>& lhs,
            const std::basic_string<charT,traits,Alloc>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv |= unbounded_uint<A,T>(rhs);
  return nrv;
}

template<class A, class T, typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>
operator & (const unbounded_uint<A,T>& lhs,
            const std::basic_string<charT,traits,Alloc>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv &= unbounded_uint<A,T>(rhs);
  return nrv;
}

template<class A, class T, typename charT, class traits, class Alloc>
inline unbounded_uint<A,T>
operator ^ (const unbounded_uint<A,T>& lhs,
            const std::basic_string<charT,traits,Alloc>& rhs)
{
  unbounded_uint<A,T> nrv(lhs);
  nrv ^= unbounded_uint<A,T>(rhs);
  return nrv;
}


// compare unbounded_uint to unbounded_uint
template<class A, class T>
inline bool
operator == (const unbounded_uint<A,T>& lhs, const unbounded_uint<A,T>& rhs)
{
  return (lhs.size() == rhs.size()) &&
         std::equal(lhs.begin(), lhs.end(), rhs.begin());
}

template<class A, class T>
inline bool
operator != (const unbounded_uint<A,T>& lhs, const unbounded_uint<A,T>& rhs)
{
  return !(lhs == rhs);
}

template<class A, class T>
bool
operator < (const unbounded_uint<A,T>& lhs, const unbounded_uint<A,T>& rhs)
{
  if (lhs.size() < rhs.size())
    return true;
  if (lhs.size() > rhs.size())
    return false;

  // TODO ops_type::compare_magnitude vs lexicographical_compare
  return std::lexicographical_compare(lhs.rbegin(), lhs.rend(),
                                      rhs.rbegin(), rhs.rend());
}

template<class A, class T>
inline bool
operator > (const unbounded_uint<A,T>& lhs, const unbounded_uint<A,T>& rhs)
{
  return rhs < lhs;
}

template<class A, class T>
inline bool
operator <= (const unbounded_uint<A,T>& lhs, const unbounded_uint<A,T>& rhs)
{
  return !(rhs < lhs);
}

template<class A, class T>
inline bool
operator >= (const unbounded_uint<A,T>& lhs, const unbounded_uint<A,T>& rhs)
{
  return !(lhs < rhs);
}

// compare unbounded_uint to integral
template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator == (const unbounded_uint<A,T>& lhs, IntegralT rhs)
{
  return detail::unbounded_uint_integral_ops<
    unbounded_uint<A,T>, IntegralT>::equal(lhs, rhs);
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator != (const unbounded_uint<A,T>& lhs, IntegralT rhs)
{
  return !(lhs == rhs);
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator < (const unbounded_uint<A,T>& lhs, IntegralT rhs)
{
  return detail::unbounded_uint_integral_ops<
    unbounded_uint<A,T>, IntegralT>::less(lhs, rhs);
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator > (const unbounded_uint<A,T>& lhs, IntegralT rhs)
{
  return rhs < lhs;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator <= (const unbounded_uint<A,T>& lhs, IntegralT rhs)
{
  return (lhs < rhs) || (lhs == rhs);
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator >= (const unbounded_uint<A,T>& lhs, IntegralT rhs)
{
  return !(lhs < rhs);
}

// compare integral to unbounded_uint
template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator == (IntegralT lhs, const unbounded_uint<A,T>& rhs)
{
  return rhs == lhs;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator != (IntegralT lhs, const unbounded_uint<A,T>& rhs)
{
  return !(lhs == rhs);
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator < (IntegralT lhs, const unbounded_uint<A,T>& rhs)
{
  return !(rhs <= lhs);
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator > (IntegralT lhs, const unbounded_uint<A,T>& rhs)
{
  return rhs < lhs;
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator <= (IntegralT lhs, const unbounded_uint<A,T>& rhs)
{
  return !(rhs < lhs);
}

template<class A, class T, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator >= (IntegralT lhs, const unbounded_uint<A,T>& rhs)
{
  return rhs <= lhs;
}

// compare unbounded_uint to const charT*
template<class A, class T, typename charT>
bool
operator == (const unbounded_uint<A,T>& lhs, const charT* rhs)
{
  detail::from_string_converter<unbounded_uint<A,T> > conv;
  conv.detect_properties(rhs, rhs + std::char_traits<charT>::length(rhs));

  if (conv.total_length == 0)
  {
    if (lhs.is_uninitialized())
      return true;
    else
      return false;
  }

  // if we see a minus sign and it is not "-0" then return false
  if (!conv.is_positive && !(conv.length == 0 && conv.radix == 8))
    return false;
  // TODO that also means we skip scanning the string for invalid characters

  unbounded_uint<A,T> tmp;
  conv.convert(tmp, rhs, rhs + std::char_traits<charT>::length(rhs));

  return lhs == tmp;
}

template<class A, class T, typename charT>
inline bool
operator != (const unbounded_uint<A,T>& lhs, const charT* rhs)
{
  return lhs != unbounded_uint<A,T>(rhs);
}

template<class A, class T, typename charT>
inline bool
operator < (const unbounded_uint<A,T>& lhs, const charT* rhs)
{
  return lhs < unbounded_uint<A,T>(rhs);
}

template<class A, class T, typename charT>
inline bool
operator > (const unbounded_uint<A,T>& lhs, const charT* rhs)
{
  return lhs > unbounded_uint<A,T>(rhs);
}

template<class A, class T, typename charT>
inline bool
operator <= (const unbounded_uint<A,T>& lhs, const charT* rhs)
{
  return lhs <= unbounded_uint<A,T>(rhs);
}

template<class A, class T, typename charT>
inline bool
operator >= (const unbounded_uint<A,T>& lhs, const charT* rhs)
{
  return lhs >= unbounded_uint<A,T>(rhs);
}

// comparison const charT* to unbounded_uint
template<class A, class T, typename charT>
inline bool
operator == (const charT* lhs, const unbounded_uint<A,T>& rhs)
{
  return unbounded_uint<A,T>(lhs) == rhs;
}

template<class A, class T, typename charT>
inline bool
operator != (const charT* lhs, const unbounded_uint<A,T>& rhs)
{
  return unbounded_uint<A,T>(lhs) != rhs;
}

template<class A, class T, typename charT>
inline bool
operator < (const charT* lhs, const unbounded_uint<A,T>& rhs)
{
  return unbounded_uint<A,T>(lhs) < rhs;
}

template<class A, class T, typename charT>
inline bool
operator > (const charT* lhs, const unbounded_uint<A,T>& rhs)
{
  return unbounded_uint<A,T>(lhs) > rhs;
}

template<class A, class T, typename charT>
inline bool
operator <= (const charT* lhs, const unbounded_uint<A,T>& rhs)
{
  return unbounded_uint<A,T>(lhs) <= rhs;
}

template<class A, class T, typename charT>
inline bool
operator >= (const charT* lhs, const unbounded_uint<A,T>& rhs)
{
  return unbounded_uint<A,T>(lhs) >= rhs;
}

// compare unbounded_uint to basic_string
template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator == (const unbounded_uint<A,T>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs == unbounded_uint<A,T>(rhs);
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator != (const unbounded_uint<A,T>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs != unbounded_uint<A,T>(rhs);
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator < (const unbounded_uint<A,T>& lhs,
            const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs < unbounded_uint<A,T>(rhs);
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator > (const unbounded_uint<A,T>& lhs,
            const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs > unbounded_uint<A,T>(rhs);
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator <= (const unbounded_uint<A,T>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs <= unbounded_uint<A,T>(rhs);
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator >= (const unbounded_uint<A,T>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs >= unbounded_uint<A,T>(rhs);
}

// compare basic_string to unbounded_uint
template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator == (const std::basic_string<charT,Traits,Alloc>& lhs,
             const unbounded_uint<A,T>& rhs)
{
  return unbounded_uint<A,T>(lhs) == rhs;
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator != (const std::basic_string<charT,Traits,Alloc>& lhs,
             const unbounded_uint<A,T>& rhs)
{
  return unbounded_uint<A,T>(lhs) != rhs;
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator < (const std::basic_string<charT,Traits,Alloc>& lhs,
            const unbounded_uint<A,T>& rhs)
{
  return unbounded_uint<A,T>(lhs) < rhs;
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator > (const std::basic_string<charT,Traits,Alloc>& lhs,
            const unbounded_uint<A,T>& rhs)
{
  return unbounded_uint<A,T>(lhs) > rhs;
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator <= (const std::basic_string<charT,Traits,Alloc>& lhs,
             const unbounded_uint<A,T>& rhs)
{
  return unbounded_uint<A,T>(lhs) <= rhs;
}

template<class A, class T, class charT, class Traits, class Alloc>
inline bool
operator >= (const std::basic_string<charT,Traits,Alloc>& lhs,
             const unbounded_uint<A,T>& rhs)
{
  return unbounded_uint<A,T>(lhs) >= rhs;
}

// Input/Output
template<class A, class T, typename charT, class traits>
std::basic_istream<charT, traits>&
operator >> (std::basic_istream<charT, traits>& is, unbounded_uint<A,T>& x)
{
  detail::stream_io<unbounded_uint<A,T> >::read(x, is);
  return is;
}

template<class A, class T, typename charT, class traits>
std::basic_ostream<charT, traits>&
operator << (std::basic_ostream<charT, traits>& os, const unbounded_uint<A,T>& x)
{
  // TODO same as above, we should output digits directly to the stream
  return os << x.template to_string<std::string>(os.flags());
}


template<class A, class T>
inline
unbounded_uint<A,T> abs(const unbounded_uint<A,T>& x)
{
  return x;
}

template<class A, class T>
inline
unbounded_uint<A,T> gcd(const unbounded_uint<A,T>& a,
                        const unbounded_uint<A,T>& b)
{
  unbounded_uint<A,T> z;
  detail::gcd_finder<unbounded_uint<A,T> >::gcd(z, a, b);
  return z;
}

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template<class A, class T, class... Integer>
unbounded_uint<A,T> gcd(const unbounded_uint<A,T>& a,
                        const unbounded_uint<A,T>& b,
                        const Integer&... args)
{
  return gcd(gcd(a, b), args...);
}
#endif

template<class A, class T>
inline
unbounded_uint<A,T> lcm(const unbounded_uint<A,T>& a,
                        const unbounded_uint<A,T>& b)
{
  unbounded_uint<A,T> z;
  detail::lcm_finder<unbounded_uint<A,T> >::lcm(z, a, b);
  return z;
}

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template<class A, class T, class... Integer>
unbounded_uint<A,T> lcm(const unbounded_uint<A,T>& a,
                        const unbounded_uint<A,T>& b,
                        const Integer&... args)
{
  return lcm(lcm(a, b), args...);
}
#endif

template<class A, class T>
inline
unbounded_uint<A,T>
pow(const unbounded_uint<A,T>& x, typename unbounded_uint<A,T>::size_type y)
{
  unbounded_uint<A,T> z;
  detail::power<unbounded_uint<A,T> >::pow(z, x, y);
  return z;
}

template<class A, class T>
inline
unbounded_uint<A,T>
pow(const unbounded_uint<A,T>& x, const unbounded_uint<A,T>& y)
{
  unbounded_uint<A,T> z;
  detail::power<unbounded_uint<A,T> >::pow(z, x, y);
  return z;
}

template<class A, class T>
inline
unbounded_uint<A,T> sqrt(const unbounded_uint<A,T>& x)
{
  unbounded_uint<A,T> z;
  detail::root<unbounded_uint<A,T> >::sqrt(z, x);
  return z;
}

template<class A, class T>
inline
unbounded_uint<A,T> nth_root(typename unbounded_uint<A,T>::size_type n,
                             const unbounded_uint<A,T>& x)
{
  unbounded_uint<A,T> z;
  detail::root<unbounded_uint<A,T> >::nth_root(z, n, x);
  return z;
}

template<class A, class T>
inline
unbounded_uint<A,T> nth_root(const unbounded_uint<A,T>& n,
                             const unbounded_uint<A,T>& x)
{
  unbounded_uint<A,T> z;
  detail::root<unbounded_uint<A,T> >::nth_root(z, n, x);
  return z;
}

template<class A, class T>
inline
int jacobi(const unbounded_uint<A,T>& x, const unbounded_uint<A,T>& y)
{
  return detail::jacobi(x, y);
}


} // namespace mp_math
} // namespace boost

#endif


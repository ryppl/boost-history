// Copyright Kevin Sopp 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_LIBTOM_INTEGER_HPP
#define BOOST_MP_MATH_INTEGER_LIBTOM_INTEGER_HPP

#include <tommath.h>
#include <boost/config.hpp>
#include <boost/mp_math/integer/contexts.hpp>
#include <boost/mp_math/integer/libtom_integer_fwd.hpp>
#include <boost/mp_math/integer/multiprecision_integer_tag.hpp>
#include <boost/mp_math/integer/detail/stream_io.hpp>
#include <boost/mp_math/integer/detail/base/print_digits.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>
#include <iostream>
#include <limits>
#include <stdexcept>

namespace boost {
namespace mp_math {

struct boost_behavior
{
  static void bitwise_or(mp_int* z, const mp_int* x, const mp_int* y);
  static void bitwise_and(mp_int* z, const mp_int* x, const mp_int* y);
  static void bitwise_xor(mp_int* z, const mp_int* x, const mp_int* y);
  static void compl_bits(mp_int* z, const mp_int* x);
};


struct libtom_behavior
{
  static void bitwise_or(mp_int* z, const mp_int* x, const mp_int* y)
  {
    mp_or(x, y, z);
  }

  static void bitwise_and(mp_int* z, const mp_int* x, const mp_int* y)
  {
    mp_and(x, y, z);
  }

  static void bitwise_xor(mp_int* z, const mp_int* x, const mp_int* y)
  {
    mp_xor(x, y, z);
  }

  static void compl_bits(mp_int* z, const mp_int* x)
  {
    mp_compl(x, z);
  }
};


namespace detail {

template<
  class B,
  typename IntegralT,
  bool IsSigned = std::numeric_limits<IntegralT>::is_signed,
  bool FitsIntoMpDigit =
             (std::numeric_limits<IntegralT>::digits <=
              std::numeric_limits<mp_digit>::digits)
>
struct libtom_integer_integral_ops;


template<
  class B,
  typename IntegralT
>
struct libtom_integer_integral_ops<libtom_integer<B>, IntegralT, false, true>
{
  typedef libtom_integer<B> libtom_integer_type;
  typedef IntegralT         integral_type;

  static void init(libtom_integer_type& z, integral_type x)
  {
    mp_init_set(&z.get_mp_int(), x);
  }

  static void assign(libtom_integer_type& z, integral_type x)
  {
    mp_set(&z.get_mp_int(), x);
  }

  static bool equal(const libtom_integer_type& z, integral_type x)
  {
    return mp_cmp_d(&z.get_mp_int(), x) == MP_EQ;
  }

  static bool less(const libtom_integer_type& z, integral_type x)
  {
    return mp_cmp_d(&z.get_mp_int(), x) == MP_LT;
  }

  static void add(libtom_integer_type& z, integral_type x)
  {
    mp_add_d(&z.get_mp_int(), x, z.get_mp_int());
  }

  static void subtract(libtom_integer_type& z, integral_type x)
  {
    mp_sub_d(&z.get_mp_int(), x, z.get_mp_int());
  }

  static void multiply(libtom_integer_type& z, integral_type x)
  {
    mp_mul_d(&z.get_mp_int(), x, z.get_mp_int());
  }

  static void divide(libtom_integer_type& z, integral_type x)
  {
    mp_div_d(&z.get_mp_int(), x, z.get_mp_int(), 0);
  }

  static void modulo(libtom_integer_type& z, integral_type x)
  {
    mp_mod_d(&z.get_mp_int(), x, z.get_mp_int());
  }

  static void bitwise_or(libtom_integer_type& z, integral_type x);
  static void bitwise_and(libtom_integer_type& z, integral_type x);
  static void bitwise_xor(libtom_integer_type& z, integral_type x);
};


template<
  class B,
  typename IntegralT
>
struct libtom_integer_integral_ops<libtom_integer<B>, IntegralT, true, true>
{
  typedef libtom_integer<B> libtom_integer_type;
  typedef IntegralT         integral_type;

  static void init(libtom_integer_type& z, integral_type x)
  {
    mp_init_set(&z.get_mp_int(), x);
  }

  static void assign(libtom_integer_type& z, integral_type x)
  {
    mp_set(&z.get_mp_int(), x);
  }

  static bool equal(const libtom_integer_type& z, integral_type x)
  {
    return mp_cmp_d(&z.get_mp_int(), x) == MP_EQ;
  }

  static bool less(const libtom_integer_type& z, integral_type x)
  {
    return mp_cmp_d(&z.get_mp_int(), x) == MP_LT;
  }

  static void add(libtom_integer_type& z, integral_type x)
  {
    mp_add_d(&z.get_mp_int(), x, z.get_mp_int());
  }

  static void subtract(libtom_integer_type& z, integral_type x)
  {
    mp_sub_d(&z.get_mp_int(), x, z.get_mp_int());
  }

  static void multiply(libtom_integer_type& z, integral_type x)
  {
    mp_mul_d(&z.get_mp_int(), x, z.get_mp_int());
  }

  static void divide(libtom_integer_type& z, integral_type x)
  {
    mp_div_d(&z.get_mp_int(), x, z.get_mp_int(), 0);
  }

  static void modulo(libtom_integer_type& z, integral_type x)
  {
    mp_mod_d(&z.get_mp_int(), x, z.get_mp_int());
  }

  static void bitwise_or(libtom_integer_type& z, integral_type x);
  static void bitwise_and(libtom_integer_type& z, integral_type x);
  static void bitwise_xor(libtom_integer_type& z, integral_type x);
};


template<
  class B,
  typename IntegralT,
  bool IsSigned = std::numeric_limits<IntegralT>::is_signed,
  bool FitsIntoLongInt =
    IsSigned ? (std::numeric_limits<IntegralT>::digits <=
                std::numeric_limits<signed long int>::digits)
             : (std::numeric_limits<IntegralT>::digits <=
                std::numeric_limits<unsigned long int>::digits)
>
struct libtom_integer_to_integral;


template<class B, typename IntegralT>
struct libtom_integer_to_integral<B, IntegralT, false, true>
{
  static IntegralT convert(const libtom_integer<B>& x)
  {
    const unsigned long y = mp_get_int(&x.get_mp_int());
    return static_cast<IntegralT>(y);
  }
};


template<class B, typename IntegralT>
struct libtom_integer_to_integral<B, IntegralT, true, true>
{
  static IntegralT convert(const libtom_integer<B>& x)
  {
    unsigned long y = mp_get_int(&x.get_mp_int());
    std::cout << "y = " << y << std::endl;
    if (x.is_negative())
      y = -y;
    return static_cast<IntegralT>(y);
  }
};



struct libtom_integer_traits
{
  typedef mp_digit    digit_type;
  typedef std::size_t size_type;

  static const size_type  digit_bits      = CHAR_BIT * sizeof(mp_digit);
  static const size_type  radix_bits      = DIGIT_BIT;
  static const digit_type max_digit_value = MP_DIGIT_MAX;
};


struct libtom_allocated_string
{
  char* str;
  libtom_allocated_string(const mp_int* a, int radix, int length)
  :
    str(static_cast<char*>(XMALLOC(length)))
  {
    mp_toradix(a, str, radix);
  }

  ~libtom_allocated_string()
  {
    XFREE(str);
  }
};



} // namespace detail


// libtom_original or libtom_boost
// differences: TODO
template<class Behavior>
class libtom_integer
{
  mp_int val_;

public:

  typedef multiprecision_integer_tag tag;

  static const bool is_signed = true;
  static const bool is_bounded = false;

  typedef Behavior behavior_type;

  template<typename IntegralT>
  struct integral_ops
  :
    detail::libtom_integer_integral_ops<libtom_integer<Behavior>, IntegralT>
  {};

  typedef detail::libtom_integer_traits traits_type;

  typedef traits_type::digit_type digit_type;
  typedef traits_type::size_type  size_type;

  typedef digit_type*                           iterator;
  typedef const digit_type*                     const_iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  libtom_integer()
  {
    mp_init(&val_);
  }

  template<typename IntegralT>
  libtom_integer(IntegralT x,
              typename enable_if<is_integral<IntegralT> >::type* dummy = 0)
  {
    integral_ops<IntegralT>::init(*this, x);
  }

  libtom_integer(const mp_int& x)
  {
    mp_init_copy(&val_, &x);
  }

  #ifndef BOOST_NO_RVALUE_REFERENCES
  libtom_integer(mp_int&& x)
  :
    val_(x)
  {}
  #endif

  explicit libtom_integer(const char* s);
  explicit libtom_integer(const std::string& s);

  libtom_integer(const char* s,        std::ios_base::fmtflags);
  libtom_integer(const std::string& s, std::ios_base::fmtflags);

  libtom_integer(const libtom_integer& copy)
  {
    mp_init_copy(&val_, &copy.val_);
  }

  #ifndef BOOST_NO_RVALUE_REFERENCES
  libtom_integer(libtom_integer&& copy)
  {
    val_.used  = copy.val_.used;
    val_.alloc = copy.val_.alloc;
    val_.sign  = copy.val_.sign;
    val_.dp    = copy.val_.dp;
    copy.val_.used  = 0;
    copy.val_.alloc = 0;
    copy.val_.dp    = 0;
  }
  #endif

  ~libtom_integer()
  {
    mp_clear(&val_);
  }

  libtom_integer& operator = (const libtom_integer& rhs)
  {
    mp_copy(&rhs.val_, &val_);
    return *this;
  }

  #ifndef BOOST_NO_RVALUE_REFERENCES
  libtom_integer& operator = (libtom_integer&& rhs)
  {
    mp_clear(&val_);
    swap(rhs);
    return *this;
  }
  #endif

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, libtom_integer&>::type
  operator = (IntegralT x)
  {
    integral_ops<IntegralT>::assign(*this, x);
    return *this;
  }

  template<typename charT>
  libtom_integer& operator = (const charT* s)
  {
    if (mp_read_radix(&val_, s, 0) == MP_OKAY)
      return *this;
    else
      throw std::invalid_argument(
          "boost::mp_math::libtom_integer: operator = (const charT*)");
  }

  template<typename charT, class traits, class alloc>
  libtom_integer& operator = (const std::basic_string<charT,traits,alloc>& s)
  {
    if (mp_read_radix(&val_, s, 0) == MP_OKAY)
      return *this;
    else
      throw std::invalid_argument(
          "boost::mp_math::libtom_integer: "
          "operator = (const std::basic_string<charT,traits,alloc>&)");
  }

  template<typename charT>
  void assign(const charT* s, std::ios_base::fmtflags f)
  {
    construct_from_string(s, f);
  }

  // TODO dispatch std::string to mp_read_radix, but dispatch other string
  // types to detail/string_converter? Could do that but would need support for
  // nail bits.
  template<typename charT, class traits, class alloc>
  void assign(const std::basic_string<charT,traits,alloc>& s,
              std::ios_base::fmtflags f)
  {
    construct_from_string(s.c_str(), f);
  }


  template<typename RandomAccessIterator>
  void assign(RandomAccessIterator first, RandomAccessIterator last,
              std::ios_base::fmtflags);

  void swap(libtom_integer& other)
  {
    mp_exch(&val_, &other.val_);
  }

#ifdef BOOST_NO_EXPLICIT_CONVERSION_OPERATORS
private:

  typedef mp_int libtom_integer::*unspecified_bool_type;

public:

  operator unspecified_bool_type() const
  {
	  return mp_iszero(&val_) ? 0 : &libtom_integer::val_;
  }
#else
  explicit operator bool() const { return static_cast<bool>(mp_iszero(&val_)); }
#endif

  bool is_even() const { return mp_iseven(&val_); }
  bool is_odd () const { return mp_isodd(&val_); }

  bool is_positive() const { return val_.sign == MP_ZPOS; }
  bool is_negative() const { return val_.sign == MP_NEG; }

  // These two functions use the same signature as GMP's mpz_class
  mp_int&       get_mp_int()       { return val_; }
  const mp_int& get_mp_int() const { return val_; }

  size_type size() const { return static_cast<size_type>(val_.used); }

  void set_size(size_type s) { val_.used = static_cast<int>(s); }

  size_type capacity() const { return static_cast<size_type>(val_.alloc); }

  void reserve(size_type n) { mp_grow(&val_, static_cast<int>(n)); }

  void clamp() { mp_clamp(&val_); }

  digit_type*       digits()       { return val_.dp; }
  const digit_type* digits() const { return val_.dp; }

  iterator begin() { return digits(); }
  iterator end  () { return digits(); }
  const_iterator begin() const { return digits(); }
  const_iterator end  () const { return digits() + size(); }
  const_iterator cbegin() const { return digits(); }
  const_iterator cend  () const { return digits() + size(); }
  reverse_iterator rbegin() { return reverse_iterator(end  ()); }
  reverse_iterator rend  () { return reverse_iterator(begin()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end  ()); }
  const_reverse_iterator rend  () const { return const_reverse_iterator(begin()); }
  const_reverse_iterator crbegin() const { return const_reverse_iterator(end  ()); }
  const_reverse_iterator crend  () const { return const_reverse_iterator(begin()); }

  digit_type&       operator [] (size_type i)       { return val_.dp[i]; }
  const digit_type& operator [] (size_type i) const { return val_.dp[i]; }

  libtom_integer& operator ++() { mp_add_d(&val_, 1, &val_); return *this; }
  libtom_integer& operator --() { mp_sub_d(&val_, 1, &val_); return *this; }

  libtom_integer operator ++(int)
  {
    libtom_integer tmp;
    mp_add_d(&val_, 1, &tmp.val_);
    return tmp;
  }

  libtom_integer operator --(int)
  {
    libtom_integer tmp;
    mp_sub_d(&val_, 1, &tmp.val_);
    return tmp;
  }

  libtom_integer& operator <<= (size_type n)
  {
    mp_mul_2d(&val_, n, &val_);
    return *this;
  }

  libtom_integer& operator >>= (size_type n)
  {
    mp_div_2d(&val_, n, &val_, 0);
    return *this;
  }

  libtom_integer& operator - () { mp_neg(&val_, &val_); return *this; }

  libtom_integer& operator += (const libtom_integer& rhs)
  {
    mp_add(&val_, &rhs.val_, &val_);
    return *this;
  }

  libtom_integer& operator -= (const libtom_integer& rhs)
  {
    mp_sub(&val_, &rhs.val_, &val_);
    return *this;
  }

  libtom_integer& operator *= (const libtom_integer& rhs)
  {
    mp_mul(&val_, &rhs.val_, &val_);
    return *this;
  }

  libtom_integer& operator /= (const libtom_integer& rhs)
  {
    mp_div(&val_, &rhs.val_, &val_, 0);
    return *this;
  }

  libtom_integer& operator %= (const libtom_integer& rhs)
  {
    mp_div(&val_, &rhs.val_, 0, &val_);
    return *this;
  }

  libtom_integer& operator |= (const libtom_integer& rhs)
  {
    behavior_type::bitwise_or(val_, val_, rhs.val_);
    return *this;
  }

  libtom_integer& operator &= (const libtom_integer& rhs)
  {
    behavior_type::bitwise_and(val_, val_, rhs.val_);
    return *this;
  }

  libtom_integer& operator ^= (const libtom_integer& rhs)
  {
    behavior_type::bitwise_xor(val_, val_, rhs.val_);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, libtom_integer&>::type
  operator += (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, libtom_integer&>::type
  operator -= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, libtom_integer&>::type
  operator *= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, libtom_integer&>::type
  operator /= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, libtom_integer&>::type
  operator %= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, libtom_integer&>::type
  operator |= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, libtom_integer&>::type
  operator &= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, libtom_integer&>::type
  operator ^= (IntegralT);

  template<typename IntegralT>
  IntegralT to_integral() const
  {
    return detail::libtom_integer_to_integral<behavior_type, IntegralT>::
      convert(*this);
  }

  template<class StringT>
  StringT to_string(std::ios_base::fmtflags f =  std::ios_base::fmtflags()) const
  {
    int radix;
    if (f & std::ios_base::hex)
      radix = 16;
    else if (f & std::ios_base::oct)
      radix = 8;
    else
      radix = 10;

    int length;
    mp_radix_size(&val_, radix, &length);
    const detail::libtom_allocated_string tmp(&val_, radix, length);
    return StringT(tmp.str, tmp.str + length - 1);
  }

  void print(bool all = false) const
  {
    detail::base::print_digits(*this, all);
  }

private:

  void construct_from_string(const char*);
  void construct_from_string(const char*, std::ios_base::fmtflags);
};



template<class B>
void libtom_integer<B>::construct_from_string(const char* s)
{
  if (mp_read_radix(&val_, s, 0) != MP_OKAY)
  {
    if (val_.dp)
      mp_clear(&val_);
    throw std::invalid_argument(
        "boost::mp_math::libtom_integer::construct_from_string(const char*)");
  }
}

template<class B>
void libtom_integer<B>::construct_from_string(const char* s, std::ios_base::fmtflags f)
{
  int radix;
  if (f & std::ios_base::hex)
    radix = 16;
  else if (f & std::ios_base::oct)
    radix = 8;
  else
    radix = 10;

  static const char* msg =
    "boost::mp_math::libtom_integer::"
    "construct_from_string(const char*, std::ios_base::fmtflags)";

  int sign;
  if (*s != '\0')
  {
    if (*s == '-')
    {
      sign = MP_NEG;
      ++s;
    }
    else
    {
      if (f & std::ios_base::showpos)
      {
        if (*s == '+')
          ++s;
        else
          throw std::invalid_argument(msg);
      }
      sign = MP_ZPOS;
    }
  }

  if (f & std::ios_base::showbase)
  {
    bool err = false;
    if (*s != '\0')
    {
      if (*s == '0')
      {
        if (radix == 10)
          err = true;
        else if (radix == 16)
        {
          ++s;
          if (*s != '\0' && (*s == 'x' || *s == 'X'))
            ++s;
          else
            err = true;
        }
      }
      else if (radix == 8 || radix == 16)
        err = true;
    }

    if (err)
      throw std::invalid_argument(msg);
  }

  if (mp_read_radix(&val_, s, radix) != MP_OKAY)
  {
    if (val_.dp)
      mp_clear(&val_);
    throw std::invalid_argument(msg);
  }

  val_.sign = sign;
}

template<class B>
libtom_integer<B>::libtom_integer(const char* s)
{
  mp_init(&val_);
  construct_from_string(s);
}

template<class B>
libtom_integer<B>::libtom_integer(const char* s, std::ios_base::fmtflags f)
{
  mp_init(&val_);
  construct_from_string(s, f);
}

template<class B>
libtom_integer<B>::libtom_integer(const std::string& s)
{
  mp_init(&val_);
  if (!s.empty())
    construct_from_string(s.c_str());
}

template<class B>
libtom_integer<B>::libtom_integer(const std::string& s, std::ios_base::fmtflags f)
{
  mp_init(&val_);
  if (!s.empty())
    construct_from_string(s.c_str());
}

template<class B>
inline void swap(libtom_integer<B>& lhs, libtom_integer<B>& rhs)
{
  lhs.swap(rhs);
}


template<class B>
inline libtom_integer<B>
operator << (const libtom_integer<B>& x,
             typename libtom_integer<B>::size_type n)
{
  libtom_integer<B> nrv;
  mp_mul_2d(&x.get_mp_int(), n, &nrv.get_mp_int());
  return nrv;
}

template<class B>
inline libtom_integer<B>
operator >> (const libtom_integer<B>& x,
             typename libtom_integer<B>::size_type n)
{
  libtom_integer<B> nrv;
  mp_div_2d(&x.get_mp_int(), n, &nrv.get_mp_int(), 0);
  return nrv;
}

template<class B>
inline
libtom_integer<B>
operator + (const libtom_integer<B>& lhs, const libtom_integer<B>& rhs)
{
  libtom_integer<B> nrv;
  mp_add(&lhs.get_mp_int(), &rhs.get_mp_int(), &nrv.get_mp_int());
  return nrv;
}

template<class B>
inline
libtom_integer<B>
operator - (const libtom_integer<B>& lhs, const libtom_integer<B>& rhs)
{
  libtom_integer<B> nrv;
  mp_sub(&lhs.get_mp_int(), &rhs.get_mp_int(), &nrv.get_mp_int());
  return nrv;
}

template<class B>
inline
libtom_integer<B>
operator * (const libtom_integer<B>& lhs, const libtom_integer<B>& rhs)
{
  libtom_integer<B> nrv;
  mp_mul(&lhs.get_mp_int(), &rhs.get_mp_int(), &nrv.get_mp_int());
  return nrv;
}

template<class B>
inline
libtom_integer<B>
operator / (const libtom_integer<B>& lhs, const libtom_integer<B>& rhs)
{
  libtom_integer<B> nrv;
  mp_div(&lhs.get_mp_int(), &rhs.get_mp_int(), &nrv.get_mp_int(), 0);
  return nrv;
}

template<class B>
inline
libtom_integer<B>
operator % (const libtom_integer<B>& lhs, const libtom_integer<B>& rhs)
{
  libtom_integer<B> nrv;
  mp_mod(&lhs.get_mp_int(), &rhs.get_mp_int(), &nrv.get_mp_int());
  return nrv;
}

template<class B>
inline
libtom_integer<B>
operator | (const libtom_integer<B>& lhs, const libtom_integer<B>& rhs)
{
  libtom_integer<B> nrv;
  B::bitwise_or(&nrv.get_mp_int(), &lhs.get_mp_int(), &rhs.get_mp_int());
  return nrv;
}

template<class B>
inline
libtom_integer<B>
operator & (const libtom_integer<B>& lhs, const libtom_integer<B>& rhs)
{
  libtom_integer<B> nrv;
  B::bitwise_and(&nrv.get_mp_int(), &lhs.get_mp_int(), &rhs.get_mp_int());
  return nrv;
}

template<class B>
inline
libtom_integer<B>
operator ^ (const libtom_integer<B>& lhs, const libtom_integer<B>& rhs)
{
  libtom_integer<B> nrv;
  B::bitwise_xor(&nrv.get_mp_int(), &lhs.get_mp_int(), &rhs.get_mp_int());
  return nrv;
}

template<class B>
inline
libtom_integer<B> operator ~ (const libtom_integer<B>& x)
{
  libtom_integer<B> nrv;
  B::compl_bits(&nrv.get_mp_int(), &x.get_mp_int());
  return nrv;
}


// Arithmetic and bitwise operators involving integral types
template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B>&>::type
libtom_integer<B>::operator += (IntegralT rhs)
{
  integral_ops<IntegralT>::add(*this, rhs);
  return *this;
}

template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B>&>::type
libtom_integer<B>::operator -= (IntegralT rhs)
{
  integral_ops<IntegralT>::subtract(*this, rhs);
  return *this;
}

template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B>&>::type
libtom_integer<B>::operator *= (IntegralT rhs)
{
  integral_ops<IntegralT>::multiply(*this, rhs);
  return *this;
}

template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B>&>::type
libtom_integer<B>::operator /= (IntegralT rhs)
{
  integral_ops<IntegralT>::divide(*this, rhs);
  return *this;
}

template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B>&>::type
libtom_integer<B>::operator %= (IntegralT rhs)
{
  integral_ops<IntegralT>::modulo(*this, rhs);
  return *this;
}

template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B>&>::type
libtom_integer<B>::operator |= (IntegralT rhs)
{
  integral_ops<IntegralT>::bitwise_or(*this, rhs);
  return *this;
}

template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B>&>::type
libtom_integer<B>::operator &= (IntegralT rhs)
{
  integral_ops<IntegralT>::bitwise_and(*this, rhs);
  return *this;
}

template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B>&>::type
libtom_integer<B>::operator ^= (IntegralT rhs)
{
  integral_ops<IntegralT>::bitwise_xor(*this, rhs);
  return *this;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B> >::type
operator + (const libtom_integer<B>& lhs, IntegralT rhs)
{
  libtom_integer<B> nrv(lhs);
  nrv += rhs;
  return nrv;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B> >::type
operator - (const libtom_integer<B>& lhs, IntegralT rhs)
{
  libtom_integer<B> nrv(lhs);
  nrv -= rhs;
  return nrv;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B> >::type
operator * (const libtom_integer<B>& lhs, IntegralT rhs)
{
  libtom_integer<B> nrv(lhs);
  nrv *= rhs;
  return nrv;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B> >::type
operator / (const libtom_integer<B>& lhs, IntegralT rhs)
{
  libtom_integer<B> nrv(lhs);
  nrv /= rhs;
  return nrv;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B> >::type
operator % (const libtom_integer<B>& lhs, IntegralT rhs)
{
  libtom_integer<B> nrv(lhs);
  nrv %= rhs;
  return nrv;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B> >::type
operator | (const libtom_integer<B>& lhs, IntegralT rhs)
{
  libtom_integer<B> nrv(lhs);
  nrv |= rhs;
  return nrv;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B> >::type
operator & (const libtom_integer<B>& lhs, IntegralT rhs)
{
  libtom_integer<B> nrv(lhs);
  nrv &= rhs;
  return nrv;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, libtom_integer<B> >::type
operator ^ (const libtom_integer<B>& lhs, IntegralT rhs)
{
  libtom_integer<B> nrv(lhs);
  nrv ^= rhs;
  return nrv;
}


template<class B>
inline bool operator == (const libtom_integer<B>& lhs, const libtom_integer<B>& rhs)
{
  return mp_cmp(&lhs.get_mp_int(), &rhs.get_mp_int()) == MP_EQ;
}

template<class B>
inline bool operator != (const libtom_integer<B>& lhs, const libtom_integer<B>& rhs)
{
  return mp_cmp(&lhs.get_mp_int(), &rhs.get_mp_int()) != MP_EQ;
}

template<class B>
inline bool operator < (const libtom_integer<B>& lhs, const libtom_integer<B>& rhs)
{
  return mp_cmp(&lhs.get_mp_int(), &rhs.get_mp_int()) == MP_LT;
}

template<class B>
inline bool operator > (const libtom_integer<B>& lhs, const libtom_integer<B>& rhs)
{
  return mp_cmp(&lhs.get_mp_int(), &rhs.get_mp_int()) == MP_GT;
}

template<class B>
inline bool operator <= (const libtom_integer<B>& lhs, const libtom_integer<B>& rhs)
{
  return mp_cmp(&lhs.get_mp_int(), &rhs.get_mp_int()) <= 0;
}

template<class B>
inline bool operator >= (const libtom_integer<B>& lhs, const libtom_integer<B>& rhs)
{
  return mp_cmp(&lhs.get_mp_int(), &rhs.get_mp_int()) >= 0;
}

// compare unbounded_int to integral
template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator == (const libtom_integer<B>& lhs, IntegralT rhs)
{
  return detail::libtom_integer_integral_ops<
    libtom_integer<B>, IntegralT>::equal(lhs, rhs);
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator != (const libtom_integer<B>& lhs, IntegralT rhs)
{
  return !(lhs == rhs);
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator < (const libtom_integer<B>& lhs, IntegralT rhs)
{
  return detail::libtom_integer_integral_ops<
    libtom_integer<B>, IntegralT>::less(lhs, rhs);
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator > (const libtom_integer<B>& lhs, IntegralT rhs)
{
  return rhs < lhs;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator <= (const libtom_integer<B>& lhs, IntegralT rhs)
{
  return (lhs < rhs) || (lhs == rhs);
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator >= (const libtom_integer<B>& lhs, IntegralT rhs)
{
  return !(lhs < rhs);
}

// compare integral to unbounded_int
template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator == (IntegralT lhs, const libtom_integer<B>& rhs)
{
  return rhs == lhs;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator != (IntegralT lhs, const libtom_integer<B>& rhs)
{
  return !(lhs == rhs);
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator < (IntegralT lhs, const libtom_integer<B>& rhs)
{
  return !(rhs <= lhs);
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator > (IntegralT lhs, const libtom_integer<B>& rhs)
{
  return rhs < lhs;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator <= (IntegralT lhs, const libtom_integer<B>& rhs)
{
  return !(rhs < lhs);
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator >= (IntegralT lhs, const libtom_integer<B>& rhs)
{
  return rhs <= lhs;
}

// compare unbounded_int to const charT*
template<class B, typename charT>
inline bool
operator == (const libtom_integer<B>& lhs, const charT* rhs)
{
  return lhs == libtom_integer<B>(rhs);
}

template<class B, typename charT>
inline bool
operator != (const libtom_integer<B>& lhs, const charT* rhs)
{
  return lhs != libtom_integer<B>(rhs);
}

template<class B, typename charT>
inline bool
operator < (const libtom_integer<B>& lhs, const charT* rhs)
{
  return lhs < libtom_integer<B>(rhs);
}

template<class B, typename charT>
inline bool
operator > (const libtom_integer<B>& lhs, const charT* rhs)
{
  return lhs > libtom_integer<B>(rhs);
}

template<class B, typename charT>
inline bool
operator <= (const libtom_integer<B>& lhs, const charT* rhs)
{
  return lhs <= libtom_integer<B>(rhs);
}

template<class B, typename charT>
inline bool
operator >= (const libtom_integer<B>& lhs, const charT* rhs)
{
  return lhs >= libtom_integer<B>(rhs);
}

// comparison const charT* to unbounded_int
template<class B, typename charT>
inline bool
operator == (const charT* lhs, const libtom_integer<B>& rhs)
{
  return libtom_integer<B>(lhs) == rhs;
}

template<class B, typename charT>
inline bool
operator != (const charT* lhs, const libtom_integer<B>& rhs)
{
  return libtom_integer<B>(lhs) != rhs;
}

template<class B, typename charT>
inline bool
operator < (const charT* lhs, const libtom_integer<B>& rhs)
{
  return libtom_integer<B>(lhs) < rhs;
}

template<class B, typename charT>
inline bool
operator > (const charT* lhs, const libtom_integer<B>& rhs)
{
  return libtom_integer<B>(lhs) > rhs;
}

template<class B, typename charT>
inline bool
operator <= (const charT* lhs, const libtom_integer<B>& rhs)
{
  return libtom_integer<B>(lhs) <= rhs;
}

template<class B, typename charT>
inline bool
operator >= (const charT* lhs, const libtom_integer<B>& rhs)
{
  return libtom_integer<B>(lhs) >= rhs;
}

// compare unbounded_int to basic_string
template<class B, class charT, class Traits, class Alloc>
inline bool
operator == (const libtom_integer<B>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs == libtom_integer<B>(rhs);
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator != (const libtom_integer<B>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs != libtom_integer<B>(rhs);
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator < (const libtom_integer<B>& lhs,
            const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs < libtom_integer<B>(rhs);
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator > (const libtom_integer<B>& lhs,
            const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs > libtom_integer<B>(rhs);
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator <= (const libtom_integer<B>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs <= libtom_integer<B>(rhs);
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator >= (const libtom_integer<B>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs >= libtom_integer<B>(rhs);
}

// compare basic_string to unbounded_int
template<class B, class charT, class Traits, class Alloc>
inline bool
operator == (const std::basic_string<charT,Traits,Alloc>& lhs,
             const libtom_integer<B>& rhs)
{
  return libtom_integer<B>(lhs) == rhs;
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator != (const std::basic_string<charT,Traits,Alloc>& lhs,
             const libtom_integer<B>& rhs)
{
  return libtom_integer<B>(lhs) != rhs;
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator < (const std::basic_string<charT,Traits,Alloc>& lhs,
            const libtom_integer<B>& rhs)
{
  return libtom_integer<B>(lhs) < rhs;
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator > (const std::basic_string<charT,Traits,Alloc>& lhs,
            const libtom_integer<B>& rhs)
{
  return libtom_integer<B>(lhs) > rhs;
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator <= (const std::basic_string<charT,Traits,Alloc>& lhs,
             const libtom_integer<B>& rhs)
{
  return libtom_integer<B>(lhs) <= rhs;
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator >= (const std::basic_string<charT,Traits,Alloc>& lhs,
             const libtom_integer<B>& rhs)
{
  return libtom_integer<B>(lhs) >= rhs;
}

// Input/Output
template<class B, typename charT, class traits>
std::basic_istream<charT, traits>&
operator >> (std::basic_istream<charT, traits>& is, libtom_integer<B>& x)
{
  detail::stream_io<libtom_integer<B> >::read(x, is);
  return is;
}

template<class B, typename charT, class traits>
std::basic_ostream<charT, traits>&
operator << (std::basic_ostream<charT, traits>& os, const libtom_integer<B>& x)
{
  return os << x.template to_string<std::string>(os.flags());
}



template<class B>
inline libtom_integer<B> abs(const libtom_integer<B>& x)
{
  libtom_integer<B> nrv;
  mp_abs(&x.get_mp_int(), &nrv.get_mp_int());
  return nrv;
}

template<class B>
inline libtom_integer<B> gcd(const libtom_integer<B>& x, const libtom_integer<B>& y)
{
  libtom_integer<B> nrv;
  mp_gcd(&x.get_mp_int(), &y.get_mp_int(), &nrv.get_mp_int());
  return nrv;
}

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template<class B, class... LTMInteger>
libtom_integer<B> gcd(const libtom_integer<B>& a,
                   const libtom_integer<B>& b,
                   const LTMInteger&... args)
{
  return gcd(gcd(a, b), args...);
}
#endif

template<class B>
inline libtom_integer<B> lcm(const libtom_integer<B>& x, const libtom_integer<B>& y)
{
  libtom_integer<B> nrv;
  mp_lcm(&x.get_mp_int(), &y.get_mp_int(), &nrv.get_mp_int());
  return nrv;
}

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template<class B, class... LTMInteger>
libtom_integer<B> lcm(const libtom_integer<B>& a,
                   const libtom_integer<B>& b,
                   const LTMInteger&... args)
{
  return lcm(lcm(a, b), args...);
}
#endif

template<class B>
inline
libtom_integer<B>
pow(const libtom_integer<B>& x, typename libtom_integer<B>::digit_type y)
{
  libtom_integer<B> nrv;
  mp_expt_d(&x.get_mp_int(), y, &nrv.get_mp_int());
  return nrv;
}

template<class B>
inline
libtom_integer<B> pow(const libtom_integer<B>& x, const libtom_integer<B>& y)
{
  libtom_integer<B> nrv;
  mp_expt_d(&x.get_mp_int(),
            y.template to_integral<unsigned long>(),
            &nrv.get_mp_int());
  return nrv;
}

template<class B>
inline
libtom_integer<B>
sqrt(const libtom_integer<B>& x)
{
  libtom_integer<B> nrv;
  mp_sqrt(&x.get_mp_int(), &nrv.get_mp_int());
  return nrv;
}

template<class B>
inline
libtom_integer<B>
nth_root(typename libtom_integer<B>::digit_type n, const libtom_integer<B>& x)
{
  libtom_integer<B> nrv;
  mp_n_root(&x.get_mp_int(), n, &nrv.get_mp_int());
  return nrv;
}

template<class B>
inline
libtom_integer<B>
nth_root(const libtom_integer<B>& n, const libtom_integer<B>& x)
{
  libtom_integer<B> nrv;
  mp_n_root(&x.get_mp_int(), n.template to_integral<mp_digit>(), &nrv.get_mp_int());
  return nrv;
}

template<class B>
inline
libtom_integer<B>
modinv(const libtom_integer<B>& x, const libtom_integer<B>& m)
{
  libtom_integer<B> nrv;
  mp_invmod(&x.get_mp_int(), &m.get_mp_int(), &nrv.get_mp_int());
  return nrv;
}

template<class B>
inline
libtom_integer<B> modpow(const libtom_integer<B>& base,
                         const libtom_integer<B>& exp,
                         const libtom_integer<B>& mod,
                         modpow_ctx<libtom_integer<B> >* ctx = 0)
{
  libtom_integer<B> nrv;
  mp_exptmod(&base.get_mp_int(), &exp.get_mp_int(), &mod.get_mp_int(),
             &nrv.get_mp_int());
  return nrv;
}

template<class B>
inline
int jacobi(const libtom_integer<B>& x, const libtom_integer<B>& y)
{
  int j;
  mp_jacobi(&x.get_mp_int(), &y.get_mp_int(), &j);
  return j;
}



} // namespace mp_math
} // namespace boost

#endif

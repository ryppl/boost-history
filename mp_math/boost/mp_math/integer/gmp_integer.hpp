// Copyright Kevin Sopp 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_GMP_INTEGER_HPP
#define BOOST_MP_MATH_INTEGER_GMP_INTEGER_HPP

#include <gmp.h>
#include <boost/config.hpp>
#include <boost/mp_math/integer/contexts.hpp>
#include <boost/mp_math/integer/gmp_integer_fwd.hpp>
#include <boost/mp_math/integer/multiprecision_integer_tag.hpp>
#include <boost/mp_math/integer/detail/stream_io.hpp>
#include <boost/mp_math/integer/detail/base/print_digits.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>
#include <cstring> // strlen
#include <iostream>
#include <limits>
#include <stdexcept>

namespace boost {
namespace mp_math {

struct boost_behavior
{
  static void bitwise_or(mpz_ptr z, mpz_srcptr x, mpz_srcptr y);
  static void bitwise_and(mpz_ptr z, mpz_srcptr x, mpz_srcptr y);
  static void bitwise_xor(mpz_ptr z, mpz_srcptr x, mpz_srcptr y);
};


struct gmp_behavior
{
  static void bitwise_or(mpz_ptr z, mpz_srcptr x, mpz_srcptr y)
  {
    mpz_ior(z, x, y);
  }

  static void bitwise_and(mpz_ptr z, mpz_srcptr x, mpz_srcptr y)
  {
    mpz_and(z, x, y);
  }

  static void bitwise_xor(mpz_ptr z, mpz_srcptr x, mpz_srcptr y)
  {
    mpz_xor(z, x, y);
  }
};



namespace detail {

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
struct gmp_integer_integral_ops;


template<
  class B,
  typename IntegralT
>
struct gmp_integer_integral_ops<gmp_integer<B>, IntegralT, false, true>
{
  typedef gmp_integer<B> gmp_integer_type;
  typedef IntegralT      integral_type;

  static void init(gmp_integer_type& z, integral_type x)
  {
    mpz_init_set_ui(z.get_mpz_t(), x);
  }

  static void assign(gmp_integer_type& z, integral_type x)
  {
    mpz_set_ui(z.get_mpz_t(), x);
  }

  static bool equal(const gmp_integer_type& z, integral_type x)
  {
    return mpz_cmp_ui(z.get_mpz_t(), x) == 0;
  }

  static bool less(const gmp_integer_type& z, integral_type x)
  {
    return mpz_cmp_ui(z.get_mpz_t(), x) < 0;
  }

  static void add(gmp_integer_type& z, integral_type x)
  {
    mpz_add_ui(z.get_mpz_t(), z.get_mpz_t(), x);
  }

  static void subtract(gmp_integer_type& z, integral_type x)
  {
    mpz_sub_ui(z.get_mpz_t(), z.get_mpz_t(), x);
  }

  static void multiply(gmp_integer_type& z, integral_type x)
  {
    mpz_mul_ui(z.get_mpz_t(), z.get_mpz_t(), x);
  }

  static void divide(gmp_integer_type& z, integral_type x)
  {
    mpz_tdiv_q_ui(z.get_mpz_t(), z.get_mpz_t(), x);
  }

  static void modulo(gmp_integer_type& z, integral_type x)
  {
    mpz_mod_ui(z.get_mpz_t(), z.get_mpz_t(), x);
  }

  static void bitwise_or(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    gmp_integer_type::behavior_type::bitwise_or(z.get_mpz_t(),
                                                z.get_mpz_t(),
                                                tmp.get_mpz_t());
  }

  static void bitwise_and(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    gmp_integer_type::behavior_type::bitwise_and(z.get_mpz_t(),
                                                 z.get_mpz_t(),
                                                 tmp.get_mpz_t());
  }

  static void bitwise_xor(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    gmp_integer_type::behavior_type::bitwise_xor(z.get_mpz_t(),
                                                 z.get_mpz_t(),
                                                 tmp.get_mpz_t());
  }
};


template<
  class B,
  typename IntegralT
>
struct gmp_integer_integral_ops<gmp_integer<B>, IntegralT, true, true>
{
  typedef gmp_integer<B> gmp_integer_type;
  typedef IntegralT      integral_type;

  static void init(gmp_integer_type& z, integral_type x)
  {
    mpz_init_set_si(z.get_mpz_t(), x);
  }

  static void assign(gmp_integer_type& z, integral_type x)
  {
    mpz_set_si(z.get_mpz_t(), x);
  }

  static bool equal(const gmp_integer_type& z, integral_type x)
  {
    return mpz_cmp_si(z.get_mpz_t(), x) == 0;
  }

  static bool less(const gmp_integer_type& z, integral_type x)
  {
    return mpz_cmp_si(z.get_mpz_t(), x) < 0;
  }

  static void add(gmp_integer_type& z, integral_type x)
  {
    if (x >= 0)
      mpz_add_ui(z.get_mpz_t(), z.get_mpz_t(), x);
    else
      mpz_sub_ui(z.get_mpz_t(), z.get_mpz_t(), -x);
  }

  static void subtract(gmp_integer_type& z, integral_type x)
  {
    if (x >= 0)
      mpz_sub_ui(z.get_mpz_t(), z.get_mpz_t(), x);
    else
      mpz_add_ui(z.get_mpz_t(), z.get_mpz_t(), -x);
  }

  static void multiply(gmp_integer_type& z, integral_type x)
  {
    mpz_mul_si(z.get_mpz_t(), z.get_mpz_t(), x);
  }

  static void divide(gmp_integer_type& z, integral_type x)
  {
    mpz_tdiv_q_ui(z.get_mpz_t(), z.get_mpz_t(), x);
  }

  static void modulo(gmp_integer_type& z, integral_type x)
  {
    mpz_mod_ui(z.get_mpz_t(), z.get_mpz_t(), x);
  }

  static void bitwise_or(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    gmp_integer_type::behavior_type::bitwise_or(z.get_mpz_t(),
                                                z.get_mpz_t(),
                                                tmp.get_mpz_t());
  }

  static void bitwise_and(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    gmp_integer_type::behavior_type::bitwise_and(z.get_mpz_t(),
                                                 z.get_mpz_t(),
                                                 tmp.get_mpz_t());
  }

  static void bitwise_xor(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    gmp_integer_type::behavior_type::bitwise_xor(z.get_mpz_t(),
                                                 z.get_mpz_t(),
                                                 tmp.get_mpz_t());
  }
};


template<
  class B,
  typename IntegralT
>
struct gmp_integer_integral_ops<gmp_integer<B>, IntegralT, false, false>
{
  typedef gmp_integer<B> gmp_integer_type;
  typedef IntegralT      integral_type;

  static void init(gmp_integer_type& z, integral_type x)
  {
    mpz_init(z.get_mpz_t());
    mpz_import(z.get_mpz_t(),
               1,  // count
               -1, // -1 = least significant first
               sizeof(integral_type),
               0,  // 0 = host endianness
               0,  // no nail bits
               &x);
  }

  static void assign(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    z = tmp;
  }

  static bool equal(const gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    return z == tmp;
  }

  static bool less(const gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    return z < tmp;
  }

  static void add(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    z += tmp;
  }

  static void subtract(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    z -= tmp;
  }

  static void multiply(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    z *= tmp;
  }

  static void divide(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    z /= tmp;
  }

  static void modulo(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    z %= tmp;
  }

  static void bitwise_or(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    gmp_integer_type::behavior_type::bitwise_or(z.get_mpz_t(),
                                                z.get_mpz_t(),
                                                tmp.get_mpz_t());
  }

  static void bitwise_and(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    gmp_integer_type::behavior_type::bitwise_and(z.get_mpz_t(),
                                                 z.get_mpz_t(),
                                                 tmp.get_mpz_t());
  }

  static void bitwise_xor(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    gmp_integer_type::behavior_type::bitwise_xor(z.get_mpz_t(),
                                                 z.get_mpz_t(),
                                                 tmp.get_mpz_t());
  }
};


template<
  class B,
  typename IntegralT
>
struct gmp_integer_integral_ops<gmp_integer<B>, IntegralT, true, false>
{
  typedef gmp_integer<B> gmp_integer_type;
  typedef IntegralT      integral_type;

  static void init(gmp_integer_type& z, integral_type x)
  {
    mpz_init(z.get_mpz_t());
    mpz_import(z.get_mpz_t(),
               1,  // count
               -1, // -1 = least significant first
               sizeof(integral_type),
               0,  // 0 = host endianness
               0,  // no nail bits
               &x);
  }

  static void assign(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    z = tmp;
  }

  static bool equal(const gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    return z == tmp;
  }

  static bool less(const gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    return z < tmp;
  }

  static void add(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    z += tmp;
  }

  static void subtract(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    z -= tmp;
  }

  static void multiply(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    z *= tmp;
  }

  static void divide(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    z /= tmp;
  }

  static void modulo(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    z %= tmp;
  }

  static void bitwise_or(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    gmp_integer_type::behavior_type::bitwise_or(z.get_mpz_t(),
                                                z.get_mpz_t(),
                                                tmp.get_mpz_t());
  }

  static void bitwise_and(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    gmp_integer_type::behavior_type::bitwise_and(z.get_mpz_t(),
                                                 z.get_mpz_t(),
                                                 tmp.get_mpz_t());
  }

  static void bitwise_xor(gmp_integer_type& z, integral_type x)
  {
    const gmp_integer_type tmp(x);
    gmp_integer_type::behavior_type::bitwise_xor(z.get_mpz_t(),
                                                 z.get_mpz_t(),
                                                 tmp.get_mpz_t());
  }
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
struct gmp_integer_to_integral;


template<class B, typename IntegralT>
struct gmp_integer_to_integral<B, IntegralT, false, true>
{
  static IntegralT convert(const gmp_integer<B>& x)
  {
    return static_cast<IntegralT>(mpz_get_ui(x.get_mpz_t()));
  }
};


template<class B, typename IntegralT>
struct gmp_integer_to_integral<B, IntegralT, true, true>
{
  static IntegralT convert(const gmp_integer<B>& x)
  {
    return static_cast<IntegralT>(mpz_get_si(x.get_mpz_t()));
  }
};


template<class B, typename IntegralT>
struct gmp_integer_to_integral<B, IntegralT, false, false>
{
  static IntegralT convert(const gmp_integer<B>& x)
  {
    // need to init to zero because GMP will early out in mpz_export and not
    // write to value if the value of x is zero.
    IntegralT value = 0;
    mpz_export(&value,
               0, // count
               -1, // -1 = least significant first
               sizeof(IntegralT),
               0, // 0 = host endianness
               0, // 0 nail bits
               x.get_mpz_t());
    return value;
  }
};


template<class B, typename IntegralT>
struct gmp_integer_to_integral<B, IntegralT, true, false>
{
  static IntegralT convert(const gmp_integer<B>& x)
  {
    IntegralT value;
    mpz_export(&value,
               0, // count
               -1, // -1 = least significant first
               sizeof(IntegralT),
               0, // 0 = host endianness
               0, // 0 nail bits
               x.get_mpz_t());
    if (x.sign() == 1)
      return value;
    else
      return -value;
  }
};



struct gmp_integer_traits
{
  typedef mp_limb_t   digit_type;
  typedef std::size_t size_type;

  static const size_type  digit_bits      = GMP_LIMB_BITS;
  static const size_type  radix_bits      = GMP_NUMB_BITS;
  static const digit_type max_digit_value = GMP_NUMB_MAX;
};


// Same as gmp_allocated_string in gmp-impl.h, but we can't use it because
// gmp-impl.h is an internal GMP header and is probably not installed.
extern "C"
{
  typedef void (*gmp_free_func)(void *, size_t);
}

struct gmp_allocated_string
{
  char*             str;
  const std::size_t len;

  gmp_allocated_string(char *s)
  :
    str(s), len(std::strlen(s) + 1)
  {}

  ~gmp_allocated_string()
  {
    gmp_free_func f;
    mp_get_memory_functions (0, 0, &f);
    (*f)(str, len);
  }
};



} // namespace detail


// gmp_original or gmp_boost
// differences: bitwise ops
template<class Behavior>
class gmp_integer
{
  mpz_t val_;

public:

  typedef multiprecision_integer_tag tag;

  static const bool is_signed = true;
  static const bool is_bounded = false;

  typedef Behavior behavior_type;

  template<typename IntegralT>
  struct integral_ops
  :
    detail::gmp_integer_integral_ops<gmp_integer<Behavior>, IntegralT>
  {};

  typedef detail::gmp_integer_traits traits_type;

  typedef traits_type::digit_type digit_type;
  typedef traits_type::size_type  size_type;

  typedef digit_type*                           iterator;
  typedef const digit_type*                     const_iterator;
  typedef std::reverse_iterator<iterator>       reverse_iterator;
  typedef std::reverse_iterator<const_iterator> const_reverse_iterator;

  gmp_integer()
  {
    mpz_init(val_);
  }

  template<typename IntegralT>
  gmp_integer(IntegralT x,
              typename enable_if<is_integral<IntegralT> >::type* dummy = 0)
  {
    integral_ops<IntegralT>::init(*this, x);
  }

  gmp_integer(const mpz_t& x)
  {
    mpz_init_set(val_, x);
  }

  #ifndef BOOST_NO_RVALUE_REFERENCES
  gmp_integer(mpz_t&& x)
  :
    val_(x)
  {}
  #endif

  explicit gmp_integer(const char* s);
  explicit gmp_integer(const std::string& s);

  gmp_integer(const char* s,        std::ios_base::fmtflags);
  gmp_integer(const std::string& s, std::ios_base::fmtflags);


  template<class ApInt>
  gmp_integer(const ApInt& x,
              typename enable_if<
                mpl::and_<
                  mpl::not_<is_same<traits_type, typename ApInt::traits_type> >,
                  is_same<typename ApInt::tag, multiprecision_integer_tag>
                >
                >::type* dummy = 0);

  gmp_integer(const gmp_integer& copy)
  {
    mpz_init_set(val_, copy.val_);
  }

  #ifndef BOOST_NO_RVALUE_REFERENCES
  gmp_integer(gmp_integer&& copy)
  {
    val_->_mp_alloc = copy.val_->_mp_alloc;
    val_->_mp_size  = copy.val_->_mp_size;
    val_->_mp_d     = copy.val_->_mp_d;
    copy.val_->_mp_alloc = 0;
    copy.val_->_mp_size  = 0;
    copy.val_->_mp_d     = 0;
  }
  #endif

  ~gmp_integer()
  {
    mpz_clear(val_);
  }

  gmp_integer& operator = (const gmp_integer& rhs)
  {
    mpz_set(val_, rhs.val_);
    return *this;
  }

  #ifndef BOOST_NO_RVALUE_REFERENCES
  gmp_integer& operator = (gmp_integer&& rhs)
  {
    mpz_clear(val_);
    swap(rhs);
    return *this;
  }
  #endif

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, gmp_integer&>::type
  operator = (IntegralT x)
  {
    integral_ops<IntegralT>::assign(*this, x);
    return *this;
  }

  template<typename charT>
  gmp_integer& operator = (const charT* s)
  {
    if (mpz_set_str(val_, s, 0) == 0)
      return *this;
    else
      throw std::invalid_argument(
          "boost::mp_math::gmp_integer: operator = (const charT*)");
  }

  template<typename charT, class traits, class alloc>
  gmp_integer& operator = (const std::basic_string<charT,traits,alloc>& s)
  {
    if (mpz_set_str(val_, s.c_str(), 0) == 0)
      return *this;
    else
      throw std::invalid_argument(
          "boost::mp_math::gmp_integer: operator = (const charT*)");
  }

  template<typename charT>
  void assign(const charT* s, std::ios_base::fmtflags f)
  {
    unsigned radix;
    if (f & std::ios_base::hex)
      radix = 16;
    else if (f & std::ios_base::oct)
      radix = 8;
    else
      radix = 10;
    if (mpz_set_str(val_, s, radix) == -1)
      throw std::invalid_argument(
          "boost::mp_math::gmp_integer::assign: illformatted string)");
  }

  // TODO dispatch std::string to mpz_set_string, but dispatch other string
  // types to detail/string_converter? Could do that but would need support for
  // NAIL bits.
  template<typename charT, class traits, class alloc>
  void assign(const std::basic_string<charT,traits,alloc>& s,
              std::ios_base::fmtflags f)
  {
    unsigned radix;
    if (f & std::ios_base::hex)
      radix = 16;
    else if (f & std::ios_base::oct)
      radix = 8;
    else
      radix = 10;
    if (mpz_set_str(val_, s.c_str(), radix) == -1)
      throw std::invalid_argument(
          "boost::mp_math::gmp_integer::assign: illformatted string)");
  }


  template<typename RandomAccessIterator>
  void assign(RandomAccessIterator first, RandomAccessIterator last,
              std::ios_base::fmtflags);

  void swap(gmp_integer& other)
  {
    mpz_swap(val_, other.val_);
  }

#ifdef BOOST_NO_EXPLICIT_CONVERSION_OPERATORS
private:

  typedef mpz_t gmp_integer::*unspecified_bool_type;

public:

  operator unspecified_bool_type() const
  {
	  return mpz_sgn(val_) ? &gmp_integer::val_ : 0;
  }
#else
  explicit operator bool() const { return static_cast<bool>(mpz_sgn(val_)); }
#endif

  bool is_even() const { return mpz_even_p(val_); }
  bool is_odd () const { return mpz_odd_p(val_); }

  bool is_positive() const { return mpz_sgn(val_) >= 0; }
  bool is_negative() const { return mpz_sgn(val_) < 0; }

  int sign() const { return is_positive() ? 1 : -1; }
  bool sign_bit() const { return is_positive() ? 0 : 1; }

  void set_sign_bit(bool s)
  {
    if (sign_bit() != s)
      mpz_neg(val_, val_);
  }

  // These two functions use the same signature as GMP's mpz_class
  mpz_ptr    get_mpz_t()       { return val_; }
  mpz_srcptr get_mpz_t() const { return val_; }

  size_type size() const { return mpz_size(val_); }

  void set_size(size_type s) { val_->_mp_size = static_cast<int>(s); }

  size_type capacity() const { return static_cast<size_type>(val_->_mp_alloc); }

  void reserve(size_type n) { mpz_realloc(val_, n); }

  void push(digit_type d)
  {
    val_->_mp_d[__GMP_ABS(val_->_mp_size)] = d;
    if (val_->_mp_size > 0)
      ++(val_->_mp_size);
    else
      --(val_->_mp_size);
  }

  digit_type*       digits()       { return val_->_mp_d; }
  const digit_type* digits() const { return val_->_mp_d; }

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

  digit_type&       operator [] (size_type i)       { return val_->_mp_d[i]; }
  const digit_type& operator [] (size_type i) const { return val_->_mp_d[i]; }

  gmp_integer& operator ++() { mpz_add_ui(val_, val_, 1); return *this; }
  gmp_integer& operator --() { mpz_sub_ui(val_, val_, 1); return *this; }

  gmp_integer operator ++(int)
  {
    gmp_integer tmp;
    mpz_add_ui(tmp.val_, val_, 1);
    return tmp;
  }

  gmp_integer operator --(int)
  {
    gmp_integer tmp;
    mpz_sub_ui(tmp.val_, val_, 1);
    return tmp;
  }

  gmp_integer& operator <<= (size_type n)
  {
    mpz_mul_2exp(val_, val_, n);
    return *this;
  }

  gmp_integer& operator >>= (size_type n)
  {
    mpz_tdiv_q_2exp(val_, val_, n);
    return *this;
  }

  gmp_integer& operator - () { mpz_neg(val_, val_); return *this; }

  gmp_integer& operator ~ () { mpz_cmp(val_, val_); return *this; }

  gmp_integer& operator += (const gmp_integer& rhs)
  {
    mpz_add(val_, val_, rhs.val_);
    return *this;
  }

  gmp_integer& operator -= (const gmp_integer& rhs)
  {
    mpz_sub(val_, val_, rhs.val_);
    return *this;
  }

  gmp_integer& operator *= (const gmp_integer& rhs)
  {
    mpz_mul(val_, val_, rhs.val_);
    return *this;
  }

  gmp_integer& operator /= (const gmp_integer& rhs)
  {
    mpz_tdiv_q(val_, val_, rhs.val_);
    return *this;
  }

  gmp_integer& operator %= (const gmp_integer& rhs)
  {
    mpz_tdiv_r(val_, val_, rhs.val_);
    return *this;
  }

  gmp_integer& operator |= (const gmp_integer& rhs)
  {
    behavior_type::bitwise_or(val_, val_, rhs.val_);
    return *this;
  }

  gmp_integer& operator &= (const gmp_integer& rhs)
  {
    behavior_type::bitwise_and(val_, val_, rhs.val_);
    return *this;
  }

  gmp_integer& operator ^= (const gmp_integer& rhs)
  {
    behavior_type::bitwise_xor(val_, val_, rhs.val_);
    return *this;
  }

  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, gmp_integer&>::type
  operator += (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, gmp_integer&>::type
  operator -= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, gmp_integer&>::type
  operator *= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, gmp_integer&>::type
  operator /= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, gmp_integer&>::type
  operator %= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, gmp_integer&>::type
  operator |= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, gmp_integer&>::type
  operator &= (IntegralT);
  template<typename IntegralT>
  typename enable_if<is_integral<IntegralT>, gmp_integer&>::type
  operator ^= (IntegralT);

  template<typename IntegralT>
  IntegralT to_integral() const
  {
    return detail::gmp_integer_to_integral<behavior_type, IntegralT>::
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

    const detail::gmp_allocated_string tmp(mpz_get_str(0, radix, val_));
    return StringT(tmp.str, tmp.str + tmp.len - 1);
  }

  void print(bool all = false) const
  {
    detail::base::print_digits(*this, all);
  }
};


template<class B>
gmp_integer<B>::gmp_integer(const char* s)
{
  if (*s != '\0')
  {
    if (mpz_init_set_str(val_, s, 0))
    {
      if (val_->_mp_d)
        mpz_clear(val_);
      throw std::invalid_argument(
          "boost::mp_math::gmp_integer::gmp_integer(const char*)");
    }
  }
  else
    mpz_init(val_);
}

template<class B>
gmp_integer<B>::gmp_integer(const char* s, std::ios_base::fmtflags f)
{
  unsigned radix;
  if (f & std::ios_base::hex)
    radix = 16;
  else if (f & std::ios_base::oct)
    radix = 8;
  else
    radix = 10;

  if (*s != '\0')
  {
    if (mpz_init_set_str(val_, s, radix))
    {
      if (val_->_mp_d)
        mpz_clear(val_);
      throw std::invalid_argument(
          "boost::mp_math::gmp_integer::"
          "gmp_integer(const char*, std::ios_base::fmtflags)");
    }
  }
  else
    mpz_init(val_);
}

template<class B>
gmp_integer<B>::gmp_integer(const std::string& s)
{
  if (!s.empty())
  {
    if (mpz_init_set_str(val_, s.c_str(), 0))
    {
      if (val_->_mp_d)
        mpz_clear(val_);
      throw std::invalid_argument(
          "boost::mp_math::gmp_integer::gmp_integer(const std::string&)");
    }
  }
  else
    mpz_init(val_);
}

template<class B>
gmp_integer<B>::gmp_integer(const std::string& s, std::ios_base::fmtflags f)
{
  unsigned radix;
  if (f & std::ios_base::hex)
    radix = 16;
  else if (f & std::ios_base::oct)
    radix = 8;
  else
    radix = 10;

  if (!s.empty())
  {
    if (mpz_init_set_str(val_, s.c_str(), radix))
    {
      if (val_->_mp_d)
        mpz_clear(val_);
      throw std::invalid_argument(
          "boost::mp_math::gmp_integer::"
          "gmp_integer(const std::string&, std::ios_base::fmtflags)");
    }
  }
  else
    mpz_init(val_);
}

template<class B>
template<class ApInt>
gmp_integer<B>::gmp_integer(
    const ApInt& x,
    typename enable_if<
      mpl::and_<
        mpl::not_<is_same<traits_type, typename ApInt::traits_type> >,
        is_same<typename ApInt::tag, multiprecision_integer_tag>
      >
    >::type*)
{
  detail::digit_converter<gmp_integer<B>, ApInt>::convert(*this, x);
  set_sign_bit(x.sign_bit()); // FIXME works for signed types only
}

template<class B>
inline void swap(gmp_integer<B>& lhs, gmp_integer<B>& rhs)
{
  lhs.swap(rhs);
}


template<class B>
inline gmp_integer<B>
operator << (const gmp_integer<B>& x,
             typename gmp_integer<B>::size_type n)
{
  gmp_integer<B> nrv;
  mpz_mul_2exp(nrv.get_mpz_t(), x.get_mpz_t(), n);
  return nrv;
}

template<class B>
inline gmp_integer<B>
operator >> (const gmp_integer<B>& x,
             typename gmp_integer<B>::size_type n)
{
  gmp_integer<B> nrv;
  mpz_tdiv_q_2exp(nrv.get_mpz_t(), x.get_mpz_t(), n);
  return nrv;
}

template<class B>
inline
gmp_integer<B>
operator + (const gmp_integer<B>& lhs, const gmp_integer<B>& rhs)
{
  gmp_integer<B> nrv;
  mpz_add(nrv.get_mpz_t(), lhs.get_mpz_t(), rhs.get_mpz_t());
  return nrv;
}

template<class B>
inline
gmp_integer<B>
operator - (const gmp_integer<B>& lhs, const gmp_integer<B>& rhs)
{
  gmp_integer<B> nrv;
  mpz_sub(nrv.get_mpz_t(), lhs.get_mpz_t(), rhs.get_mpz_t());
  return nrv;
}

template<class B>
inline
gmp_integer<B>
operator * (const gmp_integer<B>& lhs, const gmp_integer<B>& rhs)
{
  gmp_integer<B> nrv;
  mpz_mul(nrv.get_mpz_t(), lhs.get_mpz_t(), rhs.get_mpz_t());
  return nrv;
}

template<class B>
inline
gmp_integer<B>
operator / (const gmp_integer<B>& lhs, const gmp_integer<B>& rhs)
{
  gmp_integer<B> nrv;
  mpz_tdiv_q(nrv.get_mpz_t(), lhs.get_mpz_t(), rhs.get_mpz_t());
  return nrv;
}

template<class B>
inline
gmp_integer<B>
operator % (const gmp_integer<B>& lhs, const gmp_integer<B>& rhs)
{
  gmp_integer<B> nrv;
  mpz_tdiv_r(nrv.get_mpz_t(), lhs.get_mpz_t(), rhs.get_mpz_t());
  return nrv;
}

template<class B>
inline
gmp_integer<B>
operator | (const gmp_integer<B>& lhs, const gmp_integer<B>& rhs)
{
  gmp_integer<B> nrv;
  B::bitwise_or(nrv.get_mpz_t(), lhs.get_mpz_t(), rhs.get_mpz_t());
  return nrv;
}

template<class B>
inline
gmp_integer<B>
operator & (const gmp_integer<B>& lhs, const gmp_integer<B>& rhs)
{
  gmp_integer<B> nrv;
  B::bitwise_and(nrv.get_mpz_t(), lhs.get_mpz_t(), rhs.get_mpz_t());
  return nrv;
}

template<class B>
inline
gmp_integer<B>
operator ^ (const gmp_integer<B>& lhs, const gmp_integer<B>& rhs)
{
  gmp_integer<B> nrv;
  B::bitwise_xor(nrv.get_mpz_t(), lhs.get_mpz_t(), rhs.get_mpz_t());
  return nrv;
}

// Arithmetic and bitwise operators involving integral types
template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B>&>::type
gmp_integer<B>::operator += (IntegralT rhs)
{
  integral_ops<IntegralT>::add(*this, rhs);
  return *this;
}

template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B>&>::type
gmp_integer<B>::operator -= (IntegralT rhs)
{
  integral_ops<IntegralT>::subtract(*this, rhs);
  return *this;
}

template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B>&>::type
gmp_integer<B>::operator *= (IntegralT rhs)
{
  integral_ops<IntegralT>::multiply(*this, rhs);
  return *this;
}

template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B>&>::type
gmp_integer<B>::operator /= (IntegralT rhs)
{
  integral_ops<IntegralT>::divide(*this, rhs);
  return *this;
}

template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B>&>::type
gmp_integer<B>::operator %= (IntegralT rhs)
{
  integral_ops<IntegralT>::modulo(*this, rhs);
  return *this;
}

template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B>&>::type
gmp_integer<B>::operator |= (IntegralT rhs)
{
  integral_ops<IntegralT>::bitwise_or(*this, rhs);
  return *this;
}

template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B>&>::type
gmp_integer<B>::operator &= (IntegralT rhs)
{
  integral_ops<IntegralT>::bitwise_and(*this, rhs);
  return *this;
}

template<class B>
template<typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B>&>::type
gmp_integer<B>::operator ^= (IntegralT rhs)
{
  integral_ops<IntegralT>::bitwise_xor(*this, rhs);
  return *this;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B> >::type
operator + (const gmp_integer<B>& lhs, IntegralT rhs)
{
  gmp_integer<B> nrv(lhs);
  nrv += rhs;
  return nrv;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B> >::type
operator - (const gmp_integer<B>& lhs, IntegralT rhs)
{
  gmp_integer<B> nrv(lhs);
  nrv -= rhs;
  return nrv;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B> >::type
operator * (const gmp_integer<B>& lhs, IntegralT rhs)
{
  gmp_integer<B> nrv(lhs);
  nrv *= rhs;
  return nrv;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B> >::type
operator / (const gmp_integer<B>& lhs, IntegralT rhs)
{
  gmp_integer<B> nrv(lhs);
  nrv /= rhs;
  return nrv;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B> >::type
operator % (const gmp_integer<B>& lhs, IntegralT rhs)
{
  gmp_integer<B> nrv(lhs);
  nrv %= rhs;
  return nrv;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B> >::type
operator | (const gmp_integer<B>& lhs, IntegralT rhs)
{
  gmp_integer<B> nrv(lhs);
  nrv |= rhs;
  return nrv;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B> >::type
operator & (const gmp_integer<B>& lhs, IntegralT rhs)
{
  gmp_integer<B> nrv(lhs);
  nrv &= rhs;
  return nrv;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, gmp_integer<B> >::type
operator ^ (const gmp_integer<B>& lhs, IntegralT rhs)
{
  gmp_integer<B> nrv(lhs);
  nrv ^= rhs;
  return nrv;
}


template<class B>
inline bool operator == (const gmp_integer<B>& lhs, const gmp_integer<B>& rhs)
{
  return mpz_cmp(lhs.get_mpz_t(), rhs.get_mpz_t()) == 0;
}

template<class B>
inline bool operator != (const gmp_integer<B>& lhs, const gmp_integer<B>& rhs)
{
  return mpz_cmp(lhs.get_mpz_t(), rhs.get_mpz_t()) != 0;
}

template<class B>
inline bool operator < (const gmp_integer<B>& lhs, const gmp_integer<B>& rhs)
{
  return mpz_cmp(lhs.get_mpz_t(), rhs.get_mpz_t()) < 0;
}

template<class B>
inline bool operator > (const gmp_integer<B>& lhs, const gmp_integer<B>& rhs)
{
  return mpz_cmp(lhs.get_mpz_t(), rhs.get_mpz_t()) > 0;
}

template<class B>
inline bool operator <= (const gmp_integer<B>& lhs, const gmp_integer<B>& rhs)
{
  return mpz_cmp(lhs.get_mpz_t(), rhs.get_mpz_t()) <= 0;
}

template<class B>
inline bool operator >= (const gmp_integer<B>& lhs, const gmp_integer<B>& rhs)
{
  return mpz_cmp(lhs.get_mpz_t(), rhs.get_mpz_t()) >= 0;
}

// compare unbounded_int to integral
template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator == (const gmp_integer<B>& lhs, IntegralT rhs)
{
  return detail::gmp_integer_integral_ops<
    gmp_integer<B>, IntegralT>::equal(lhs, rhs);
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator != (const gmp_integer<B>& lhs, IntegralT rhs)
{
  return !(lhs == rhs);
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator < (const gmp_integer<B>& lhs, IntegralT rhs)
{
  return detail::gmp_integer_integral_ops<
    gmp_integer<B>, IntegralT>::less(lhs, rhs);
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator > (const gmp_integer<B>& lhs, IntegralT rhs)
{
  return rhs < lhs;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator <= (const gmp_integer<B>& lhs, IntegralT rhs)
{
  return (lhs < rhs) || (lhs == rhs);
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator >= (const gmp_integer<B>& lhs, IntegralT rhs)
{
  return !(lhs < rhs);
}

// compare integral to unbounded_int
template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator == (IntegralT lhs, const gmp_integer<B>& rhs)
{
  return rhs == lhs;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator != (IntegralT lhs, const gmp_integer<B>& rhs)
{
  return !(lhs == rhs);
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator < (IntegralT lhs, const gmp_integer<B>& rhs)
{
  return !(rhs <= lhs);
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator > (IntegralT lhs, const gmp_integer<B>& rhs)
{
  return rhs < lhs;
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator <= (IntegralT lhs, const gmp_integer<B>& rhs)
{
  return !(rhs < lhs);
}

template<class B, typename IntegralT>
inline
typename enable_if<is_integral<IntegralT>, bool>::type
operator >= (IntegralT lhs, const gmp_integer<B>& rhs)
{
  return rhs <= lhs;
}

// compare unbounded_int to const charT*
template<class B, typename charT>
inline bool
operator == (const gmp_integer<B>& lhs, const charT* rhs)
{
  return lhs == gmp_integer<B>(rhs);
}

template<class B, typename charT>
inline bool
operator != (const gmp_integer<B>& lhs, const charT* rhs)
{
  return lhs != gmp_integer<B>(rhs);
}

template<class B, typename charT>
inline bool
operator < (const gmp_integer<B>& lhs, const charT* rhs)
{
  return lhs < gmp_integer<B>(rhs);
}

template<class B, typename charT>
inline bool
operator > (const gmp_integer<B>& lhs, const charT* rhs)
{
  return lhs > gmp_integer<B>(rhs);
}

template<class B, typename charT>
inline bool
operator <= (const gmp_integer<B>& lhs, const charT* rhs)
{
  return lhs <= gmp_integer<B>(rhs);
}

template<class B, typename charT>
inline bool
operator >= (const gmp_integer<B>& lhs, const charT* rhs)
{
  return lhs >= gmp_integer<B>(rhs);
}

// comparison const charT* to unbounded_int
template<class B, typename charT>
inline bool
operator == (const charT* lhs, const gmp_integer<B>& rhs)
{
  return gmp_integer<B>(lhs) == rhs;
}

template<class B, typename charT>
inline bool
operator != (const charT* lhs, const gmp_integer<B>& rhs)
{
  return gmp_integer<B>(lhs) != rhs;
}

template<class B, typename charT>
inline bool
operator < (const charT* lhs, const gmp_integer<B>& rhs)
{
  return gmp_integer<B>(lhs) < rhs;
}

template<class B, typename charT>
inline bool
operator > (const charT* lhs, const gmp_integer<B>& rhs)
{
  return gmp_integer<B>(lhs) > rhs;
}

template<class B, typename charT>
inline bool
operator <= (const charT* lhs, const gmp_integer<B>& rhs)
{
  return gmp_integer<B>(lhs) <= rhs;
}

template<class B, typename charT>
inline bool
operator >= (const charT* lhs, const gmp_integer<B>& rhs)
{
  return gmp_integer<B>(lhs) >= rhs;
}

// compare unbounded_int to basic_string
template<class B, class charT, class Traits, class Alloc>
inline bool
operator == (const gmp_integer<B>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs == gmp_integer<B>(rhs);
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator != (const gmp_integer<B>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs != gmp_integer<B>(rhs);
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator < (const gmp_integer<B>& lhs,
            const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs < gmp_integer<B>(rhs);
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator > (const gmp_integer<B>& lhs,
            const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs > gmp_integer<B>(rhs);
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator <= (const gmp_integer<B>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs <= gmp_integer<B>(rhs);
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator >= (const gmp_integer<B>& lhs,
             const std::basic_string<charT,Traits,Alloc>& rhs)
{
  return lhs >= gmp_integer<B>(rhs);
}

// compare basic_string to unbounded_int
template<class B, class charT, class Traits, class Alloc>
inline bool
operator == (const std::basic_string<charT,Traits,Alloc>& lhs,
             const gmp_integer<B>& rhs)
{
  return gmp_integer<B>(lhs) == rhs;
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator != (const std::basic_string<charT,Traits,Alloc>& lhs,
             const gmp_integer<B>& rhs)
{
  return gmp_integer<B>(lhs) != rhs;
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator < (const std::basic_string<charT,Traits,Alloc>& lhs,
            const gmp_integer<B>& rhs)
{
  return gmp_integer<B>(lhs) < rhs;
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator > (const std::basic_string<charT,Traits,Alloc>& lhs,
            const gmp_integer<B>& rhs)
{
  return gmp_integer<B>(lhs) > rhs;
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator <= (const std::basic_string<charT,Traits,Alloc>& lhs,
             const gmp_integer<B>& rhs)
{
  return gmp_integer<B>(lhs) <= rhs;
}

template<class B, class charT, class Traits, class Alloc>
inline bool
operator >= (const std::basic_string<charT,Traits,Alloc>& lhs,
             const gmp_integer<B>& rhs)
{
  return gmp_integer<B>(lhs) >= rhs;
}

// Input/Output
template<class B, typename charT, class traits>
std::basic_istream<charT, traits>&
operator >> (std::basic_istream<charT, traits>& is, gmp_integer<B>& x)
{
  detail::stream_io<gmp_integer<B> >::read(x, is);
  return is;
}

template<class B, typename charT, class traits>
std::basic_ostream<charT, traits>&
operator << (std::basic_ostream<charT, traits>& os, const gmp_integer<B>& x)
{
  return os << x.template to_string<std::string>(os.flags());
}



template<class B>
inline gmp_integer<B> abs(const gmp_integer<B>& x)
{
  gmp_integer<B> nrv;
  mpz_abs(nrv.get_mpz_t(), x.get_mpz_t());
  return nrv;
}

template<class B>
inline gmp_integer<B> gcd(const gmp_integer<B>& x, const gmp_integer<B>& y)
{
  gmp_integer<B> nrv;
  mpz_gcd(nrv.get_mpz_t(), x.get_mpz_t(), y.get_mpz_t());
  return nrv;
}

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template<class B, class... GMPInteger>
gmp_integer<B> gcd(const gmp_integer<B>& a,
                   const gmp_integer<B>& b,
                   const GMPInteger&... args)
{
  return gcd(gcd(a, b), args...);
}
#endif

template<class B>
inline gmp_integer<B> lcm(const gmp_integer<B>& x, const gmp_integer<B>& y)
{
  gmp_integer<B> nrv;
  mpz_lcm(nrv.get_mpz_t(), x.get_mpz_t(), y.get_mpz_t());
  return nrv;
}

#ifndef BOOST_NO_VARIADIC_TEMPLATES
template<class B, class... GMPInteger>
gmp_integer<B> lcm(const gmp_integer<B>& a,
                   const gmp_integer<B>& b,
                   const GMPInteger&... args)
{
  return lcm(lcm(a, b), args...);
}
#endif

template<class B>
inline
gmp_integer<B>
pow(const gmp_integer<B>& x, typename gmp_integer<B>::size_type y)
{
  gmp_integer<B> nrv;
  mpz_pow_ui(nrv.get_mpz_t(), x.get_mpz_t(), y);
  return nrv;
}

template<class B>
inline
gmp_integer<B> pow(const gmp_integer<B>& x, const gmp_integer<B>& y)
{
  gmp_integer<B> nrv;
  mpz_pow_ui(nrv.get_mpz_t(), x.get_mpz_t(),
             y.template to_integral<unsigned long>());
  return nrv;
}

template<class B>
inline
gmp_integer<B>
sqrt(const gmp_integer<B>& x)
{
  gmp_integer<B> nrv;
  mpz_sqrt(nrv.get_mpz_t(), x.get_mpz_t());
  return nrv;
}

template<class B>
inline
gmp_integer<B>
nth_root(typename gmp_integer<B>::size_type n, const gmp_integer<B>& x)
{
  gmp_integer<B> nrv;
  mpz_root(nrv.get_mpz_t(), x.get_mpz_t(), n);
  return nrv;
}

template<class B>
inline
gmp_integer<B>
nth_root(const gmp_integer<B>& n, const gmp_integer<B>& x)
{
  gmp_integer<B> nrv;
  mpz_root(nrv.get_mpz_t(), x.get_mpz_t(), n);
  return nrv;
}

template<class B>
inline
gmp_integer<B>
modinv(const gmp_integer<B>& x, const gmp_integer<B>& m)
{
  gmp_integer<B> nrv;
  if (mpz_invert(nrv.get_mpz_t(), x.get_mpz_t(), m.get_mpz_t()) == 0)
    throw std::domain_error(
        "boost::mp_math::modinv: modular inverse does not exist");
  return nrv;
}

template<class B>
inline
gmp_integer<B> modpow(const gmp_integer<B>& base,
                     const gmp_integer<B>& exp,
                     const gmp_integer<B>& mod,
                     modpow_ctx<gmp_integer<B> >* ctx = 0)
{
  gmp_integer<B> nrv;
  mpz_powm(nrv.get_mpz_t(), base.get_mpz_t(), exp.get_mpz_t(), mod.get_mpz_t());
  return nrv;
}

template<class B>
inline
int jacobi(const gmp_integer<B>& x, const gmp_integer<B>& y)
{
  return mpz_jacobi(x.get_mpz_t(), y.get_mpz_t());
}



} // namespace mp_math
} // namespace boost

#endif

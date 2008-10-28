// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_MP_INT_MP_INT_HPP
#define BOOST_MP_MATH_MP_INT_MP_INT_HPP

#include <algorithm>
#include <cassert>
#include <cstring>
#include <iosfwd>
#include <iterator> // reverse_iterator
#include <limits>
#include <stdexcept>
#include <sstream>
#include <string>

#include <boost/config.hpp>
#include <boost/random.hpp>
#include <boost/serialization/split_member.hpp>
#include <boost/serialization/string.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/mp_math/mp_int/traits.hpp>
#include <boost/mp_math/mp_int/detail/string_conversion_constants.hpp>
#include <boost/mp_math/mp_int/detail/integral_ops.hpp>
#include <boost/mp_math/mp_int/detail/meta_math.hpp>
#include <boost/mp_math/mp_int/detail/prime_tab.hpp>
#include <boost/mp_math/mp_int/detail/primitive_ops.hpp>


namespace boost {
namespace mp_math {

template<class A, class T> struct modpow_ctx;

// digits are stored in least significant order

template<
  class Allocator = std::allocator<void>,
  class Traits = mp_int_traits<>
>
struct mp_int
:
  Allocator::template rebind<typename Traits::digit_type>::other
{
  typedef Allocator                       allocator_type;
  typedef Traits                          traits_type;
  typedef typename traits_type::size_type size_type;

  mp_int();

  template<typename IntegralT>
  mp_int(IntegralT, typename enable_if<is_integral<IntegralT> >::type* dummy = 0);

  mp_int(const char*);
  mp_int(const char*, std::ios_base::fmtflags);

  #ifndef BOOST_NO_CWCHAR
  mp_int(const wchar_t*);
  mp_int(const wchar_t*, std::ios_base::fmtflags);
  #endif

  template<typename charT, class traits, class Alloc>
  mp_int(const std::basic_string<charT,traits,Alloc>&);
  
  template<typename charT, class traits, class Alloc>
  mp_int(const std::basic_string<charT,traits,Alloc>&, std::ios_base::fmtflags);
  
  template<typename RandomAccessIterator>
  mp_int(RandomAccessIterator first, RandomAccessIterator last);
  
  template<typename RandomAccessIterator>
  mp_int(RandomAccessIterator first, RandomAccessIterator last,
         std::ios_base::fmtflags f);

  mp_int(const mp_int& copy);

  #ifdef BOOST_HAS_RVALUE_REFS
  mp_int(mp_int&& copy);
  #endif
  
  ~mp_int();

  mp_int& operator = (const mp_int& rhs);

  #ifdef BOOST_HAS_RVALUE_REFS
  mp_int& operator = (mp_int&& rhs);
  #endif
  
  template<typename IntegralT>
  mp_int& operator = (IntegralT rhs);

  mp_int& operator = (const char*);
  
  #ifndef BOOST_NO_CWCHAR
  mp_int& operator = (const wchar_t*);
  #endif

  template<typename charT, class traits, class Alloc>
  mp_int& operator = (const std::basic_string<charT,traits,Alloc>&);

  void assign(const char*, std::ios_base::fmtflags);

  #ifndef BOOST_NO_CWCHAR
  void assign(const wchar_t*, std::ios_base::fmtflags);
  #endif

  template<typename charT, class traits, class Alloc>
  void assign(const std::basic_string<charT,traits,Alloc>&,
              std::ios_base::fmtflags);  
  
  template<typename RandomAccessIterator>
  void assign(RandomAccessIterator first, RandomAccessIterator last,
              std::ios_base::fmtflags);

  #ifdef BOOST_HAS_RVALUE_REFS
  void swap(mp_int&& other);
  #else
  void swap(mp_int& other);
  #endif

  mp_int& operator ++();
  mp_int& operator --();
  mp_int  operator ++(int);
  mp_int  operator --(int);
  mp_int& operator <<= (size_type);
  mp_int& operator >>= (size_type);
  mp_int& operator - ();

  mp_int& operator += (const mp_int&);
  mp_int& operator -= (const mp_int&);
  mp_int& operator *= (const mp_int&);
  mp_int& operator /= (const mp_int&);
  mp_int& operator %= (const mp_int&);
  mp_int& operator |= (const mp_int&);
  mp_int& operator &= (const mp_int&);
  mp_int& operator ^= (const mp_int&);

  template<typename IntegralT> mp_int& operator += (IntegralT);
  template<typename IntegralT> mp_int& operator -= (IntegralT);
  template<typename IntegralT> mp_int& operator *= (IntegralT);
  template<typename IntegralT> mp_int& operator /= (IntegralT);
  template<typename IntegralT> mp_int& operator %= (IntegralT);
  template<typename IntegralT> mp_int& operator |= (IntegralT);
  template<typename IntegralT> mp_int& operator &= (IntegralT);
  template<typename IntegralT> mp_int& operator ^= (IntegralT);

  mp_int& operator += (const char*);
  mp_int& operator -= (const char*);
  mp_int& operator *= (const char*);
  mp_int& operator /= (const char*);
  mp_int& operator %= (const char*);
  mp_int& operator |= (const char*);
  mp_int& operator &= (const char*);
  mp_int& operator ^= (const char*);

  #ifndef BOOST_NO_CWCHAR
  mp_int& operator += (const wchar_t*);
  mp_int& operator -= (const wchar_t*);
  mp_int& operator *= (const wchar_t*);
  mp_int& operator /= (const wchar_t*);
  mp_int& operator %= (const wchar_t*);
  mp_int& operator |= (const wchar_t*);
  mp_int& operator &= (const wchar_t*);
  mp_int& operator ^= (const wchar_t*);
  #endif

  template<typename charT, class traits, class Alloc>
  mp_int& operator += (const std::basic_string<charT,traits,Alloc>&);
  template<typename charT, class traits, class Alloc>
  mp_int& operator -= (const std::basic_string<charT,traits,Alloc>&);
  template<typename charT, class traits, class Alloc>
  mp_int& operator *= (const std::basic_string<charT,traits,Alloc>&);
  template<typename charT, class traits, class Alloc>
  mp_int& operator /= (const std::basic_string<charT,traits,Alloc>&);
  template<typename charT, class traits, class Alloc>
  mp_int& operator %= (const std::basic_string<charT,traits,Alloc>&);
  template<typename charT, class traits, class Alloc>
  mp_int& operator |= (const std::basic_string<charT,traits,Alloc>&);
  template<typename charT, class traits, class Alloc>
  mp_int& operator &= (const std::basic_string<charT,traits,Alloc>&);
  template<typename charT, class traits, class Alloc>
  mp_int& operator ^= (const std::basic_string<charT,traits,Alloc>&);

  allocator_type get_allocator() const { return allocator_type(); }

private:

  typedef int mp_int::*unspecified_bool_type;

public:

  operator unspecified_bool_type() const
  {
	  return is_zero() ? 0 : &mp_int::sign_;
  }

  bool is_even() const { return (digits_[0] & digit_type(1)) == 0; }
  bool is_odd () const { return (digits_[0] & digit_type(1)) == 1; }

  bool is_positive() const { return sign_ ==  1; }
  bool is_negative() const { return sign_ == -1; }

  template<class StringT>
  StringT to_string(std::ios_base::fmtflags f = std::ios_base::dec) const;

  template<typename IntegralT>
  IntegralT to_integral() const;

public: // low level interface

  typedef typename traits_type::digit_type        digit_type;
  typedef typename traits_type::word_type         word_type;
  typedef typename traits_type::digit_type*       iterator;
  typedef const typename traits_type::digit_type* const_iterator;
  typedef std::reverse_iterator<iterator>         reverse_iterator;
  typedef std::reverse_iterator<const_iterator>   const_reverse_iterator;
  typedef detail::primitive_ops<digit_type, word_type, size_type> ops_type;

  // bits per digit, we subtract one because we count from 0
  static const int valid_bits = std::numeric_limits<digit_type>::digits;
  static const int digit_bits = std::numeric_limits<digit_type>::digits;
  // used to mask off the most significant bit(s)
  static const digit_type mp_mask = (word_type(1) << valid_bits) - 1;
  static const size_type mp_warray = 512;
    //1 << (std::numeric_limits<word_type>::digits - 2 * valid_bits + 1);
  static const digit_type digit_max = static_cast<digit_type>(-1);

  template<typename RandomAccessIterator>
  void init(RandomAccessIterator first, RandomAccessIterator last);

  template<typename RandomAccessIterator>
  void init(RandomAccessIterator first, RandomAccessIterator last,
            std::ios_base::fmtflags f);

  iterator       begin()       { return digits_;         }
  iterator       end  ()       { return digits_ + used_; }
  const_iterator begin() const { return digits_;         }
  const_iterator end  () const { return digits_ + used_; }
  reverse_iterator       rbegin()       { return reverse_iterator(end());   }
  reverse_iterator       rend  ()       { return reverse_iterator(begin()); }
  const_reverse_iterator rbegin() const { return const_reverse_iterator(end());   }
  const_reverse_iterator rend  () const { return const_reverse_iterator(begin()); }

  digit_type&       operator[](size_type i)       { return digits_[i]; }
  const digit_type& operator[](size_type i) const { return digits_[i]; }
  
  digit_type&       at(size_type i)
  {
    if (i >= used_)
      throw std::out_of_range("mp_int::at: array subscript out of range");
    return digits_[i];
  }
  const digit_type& at(size_type i) const
  {
    if (i >= used_)
      throw std::out_of_range("mp_int::at: array subscript out of range");
    return digits_[i];
  }

  void push(digit_type x) { digits_[used_++] = x; }
  void pop() { --used_; }

  void zero();

  // debug functionality
  void print(bool all=false) const;
  bool test_invariants() const;

  bool is_uninitialized() const { return digits_ == 0; }

  size_type size() const { return used_; }
  size_type capacity() const { return capacity_; }

  void set_size(size_type s) { used_ = s; }

  int sign() const { return sign_; }
  void set_sign(int s) { sign_ = s; }

  digit_type* digits() { return digits_; }
  const digit_type* digits() const { return digits_; }

  void grow_capacity(size_type n);
  void clamp();

  int compare_magnitude(const mp_int& rhs) const;
  int compare_to_digit(digit_type) const;
  int compare(const mp_int& rhs) const;

  void add_magnitude(const mp_int& rhs);
  void sub_smaller_magnitude(const mp_int& rhs);

  bool is_zero() const;
  bool is_power_of_two() const;
  void add_digit(digit_type);
  void sub_digit(digit_type);

  void shift_digits_left(size_type);
  void shift_digits_right(size_type);

  void multiply_by_digit(digit_type);
  void karatsuba_mul(const mp_int&);
  void toom_cook_mul(const mp_int&);
  void multiply_by_2();
  void mul_digits(const mp_int&, size_type num_digits);
  void mul_high_digits(const mp_int&, size_type num_digits);
  void fast_mul_digits(const mp_int&, size_type num_digits);
  void fast_mul_high_digits(const mp_int&, size_type num_digits);

  void sqr();
  void toom_sqr();
  void karatsuba_sqr();
  void comba_sqr();

  digit_type divide_by_digit(digit_type); // returns remainder
  void divide(const mp_int& divisor, mp_int* remainder);
  void divide_by_2();
  digit_type divide_by_3();
  void modulo_2_to_the_power_of(size_type);
  size_type precision() const;
  size_type count_lsb() const;
  void shift_right(size_type b, mp_int* remainder);

  void pow2(size_type b);

  void modpow(const mp_int& exp, const mp_int& m, modpow_ctx<Allocator,Traits>* c = 0);

  void modinv(const mp_int& modulus);
  void even_modinv(const mp_int& modulus);
  void odd_modinv(const mp_int& modulus);

  void set_least_significant_bit()
  {
    digits_[0] |= digit_type(1);
  }
  
  void set_bit(size_type bit)
  {
    digits_[bit / valid_bits] |=
      digit_type(1) << digit_type(bit % valid_bits);
  }

  template<class A, class T>
  friend bool operator == (const mp_int<A,T>&, const mp_int<A,T>&);

  template<class A, class T>
  friend bool operator < (const mp_int<A,T>&, const mp_int<A,T>&);

  template<class A, class T>
  friend mp_int<A,T> abs(const mp_int<A,T>& x);
  template<class A, class T>
  friend mp_int<A,T> gcd(const mp_int<A,T>& a, const mp_int<A,T>& b);
  template<class A, class T>
  friend int jacobi(const mp_int<A,T>& a, const mp_int<A,T>& b);

  template<typename Iter>
  void from_string(Iter first, Iter last, unsigned radix);

private:
  
  friend class boost::serialization::access;
  template<class Archive>
  void save(Archive & ar, const unsigned int /*version*/) const
  {
    const std::string s = to_string<std::string>(std::ios_base::hex);
    ar & s;
  }
  template<class Archive>
  void load(Archive & ar, const unsigned int /*version*/)
  {
    std::string s;
    ar & s;
    assign(s, std::ios_base::hex);
  }
  BOOST_SERIALIZATION_SPLIT_MEMBER()

  digit_type* digits_;
  size_type used_, capacity_;
  int sign_;
};



template<class A, class T>
void mp_int<A,T>::print(bool all) const
{
  using std::cout;
  if (is_negative())
  cout << '-';
  cout << used_ << "{";
  for (size_type i = 0; i < used_; ++i)
  {
    cout << static_cast<word_type>(digits_[i]);
    if (i < used_  - 1)
      cout << ",";
  }
  cout << "}";
  
  if (all)
  {
    cout << capacity_ - used_ << "{";
    for (size_type i = used_; i < capacity_; ++i)
    {
      cout << static_cast<word_type>(digits_[i]);
      if (i < capacity_  - 1)
        cout << ",";
    }
    cout << "}";
  }
  cout << "\n";
}

template<class A, class T>
bool mp_int<A,T>::test_invariants() const
{
  if (used_) // don't test uninitialized mp_ints
  {
    if (used_ > capacity_)
      return false;
    if (digits_[used_-1] == 0)
      return false;
    if (sign_ != 1 && sign_ != -1)
      return false;
    if (is_zero() && sign_ != 1)
      return false;
  }
  return true;
}

template<class A, class T>
mp_int<A,T>& mp_int<A,T>::operator = (const mp_int<A,T>& rhs)
{
  if (this != &rhs)
  {
    if ((capacity_ == 0) || (capacity_ < rhs.capacity_))
      mp_int(rhs).swap(*this);
    else
    {
      std::memcpy(digits_, rhs.digits_, rhs.used_ * sizeof(digit_type));
      used_ = rhs.used_;
      sign_ = rhs.sign_;
    }
  }
  return *this;
}

#ifdef BOOST_HAS_RVALUE_REFS
template<class A, class T>
mp_int<A,T>& mp_int<A,T>::operator = (mp_int<A,T>&& rhs)
{
  if (this != &rhs)
  {
    this->deallocate(digits_, capacity_);
    digits_ = 0;
    used_ = 0;
    capacity_ = 0;
    sign_ = 1;
    swap(rhs);
  }
  return *this;
}
#endif

template<class A, class T>
template<typename IntegralT>
inline mp_int<A,T>& mp_int<A,T>::operator = (IntegralT rhs)
{
  detail::integral_ops<IntegralT>::assign(*this, rhs);
  return *this;
}

template<class A, class T>
mp_int<A,T>& mp_int<A,T>::operator = (const char* s)
{
  used_ = 0;
  init(s, s + std::strlen(s));
  return *this;
}

#ifndef BOOST_NO_CWCHAR
template<class A, class T>
mp_int<A,T>& mp_int<A,T>::operator = (const wchar_t* s)
{
  used_ = 0;
  init(s, s + std::wcslen(s));
  return *this;
}
#endif

template<class A, class T>
template<typename charT, class traits, class Alloc>
mp_int<A,T>& mp_int<A,T>::operator = (const std::basic_string<charT,traits,Alloc>& s)
{
  used_ = 0;
  init(s.begin(), s.end());
  return *this;
}

template<class A, class T>
inline void
mp_int<A,T>::assign(const char* s, std::ios_base::fmtflags f)
{
  assign(s, s + std::strlen(s), f);
}

#ifndef BOOST_NO_CWCHAR
template<class A, class T>
inline void
mp_int<A,T>::assign(const wchar_t* s, std::ios_base::fmtflags f)
{
  assign(s, s + std::wcslen(s), f);
}
#endif

template<class A, class T>
template<typename charT, class traits, class Alloc>
inline void
mp_int<A,T>::assign(const std::basic_string<charT,traits,Alloc>& s,
                    std::ios_base::fmtflags f)
{
  assign(s.begin(), s.end(), f);
}

template<class A, class T>
template<typename RandomAccessIterator>
inline void
mp_int<A,T>::assign(RandomAccessIterator first, RandomAccessIterator last,
                    std::ios_base::fmtflags f)
{
  used_ = 0;
  init(first, last, f);
}


template<class A, class T>
#ifdef BOOST_HAS_RVALUE_REFS
void mp_int<A,T>::swap(mp_int&& other)
#else
void mp_int<A,T>::swap(mp_int& other)
#endif
{
  std::swap(digits_, other.digits_);
  std::swap(used_, other.used_);
  std::swap(capacity_, other.capacity_);
  std::swap(sign_, other.sign_);
}

template<class A, class T>
template<typename IntegralT>
inline mp_int<A,T>& mp_int<A,T>::operator += (IntegralT rhs)
{
  detail::integral_ops<IntegralT>::add(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline mp_int<A,T>& mp_int<A,T>::operator -= (IntegralT rhs)
{
  detail::integral_ops<IntegralT>::subtract(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline mp_int<A,T>& mp_int<A,T>::operator *= (IntegralT rhs)
{
  detail::integral_ops<IntegralT>::multiply(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline mp_int<A,T>& mp_int<A,T>::operator /= (IntegralT rhs)
{
  detail::integral_ops<IntegralT>::divide(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline mp_int<A,T>& mp_int<A,T>::operator %= (IntegralT rhs)
{
  detail::integral_ops<IntegralT>::modulo(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline mp_int<A,T>& mp_int<A,T>::operator |= (IntegralT rhs)
{
  detail::integral_ops<IntegralT>::bitwise_or(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline mp_int<A,T>& mp_int<A,T>::operator &= (IntegralT rhs)
{
  detail::integral_ops<IntegralT>::bitwise_and(*this, rhs);
  return *this;
}

template<class A, class T>
template<typename IntegralT>
inline mp_int<A,T>& mp_int<A,T>::operator ^= (IntegralT rhs)
{
  detail::integral_ops<IntegralT>::bitwise_xor(*this, rhs);
  return *this;
}


template<class A, class T>
void mp_int<A,T>::zero()
{
  grow_capacity(1);
  digits_[0] = 0;
  used_ = 1;
  sign_ = 1;
}

template<class A, class T>
void mp_int<A,T>::grow_capacity(size_type n)
{
  if (capacity_ < n)
  {
    const size_type new_cap = capacity_ + capacity_;
    if (new_cap > n)
      n = new_cap;
    digit_type* d = this->allocate(n, digits_);
    std::memcpy(d, digits_, sizeof(digit_type) * used_);
    this->deallocate(digits_, capacity_);
    digits_ = d;
    capacity_ = n;
  }
}

/* trim unused digits 
 *
 * This is used to ensure that leading zero digits are trimmed.
 * Typically very fast.
 */
template<class A, class T>
void mp_int<A,T>::clamp()
{
  /* decrease used while the most significant digit is zero. */
  while (used_ > 1 && digits_[used_-1] == 0)
    --used_;
}

template<class A, class T>
inline bool mp_int<A,T>::is_zero() const
{
  return used_ == 1 && digits_[0] == 0;
}

// disregards the sign of the numbers
// return 1 if *this is greater
// returns 0 if both are equal
// return -1 if *this is smaller
template<class A, class T>
int mp_int<A,T>::compare_magnitude(const mp_int& rhs) const
{
  /* compare based on # of non-zero digits */
  if (used_ > rhs.used_)
    return 1;
  
  if (used_ < rhs.used_)
    return -1;

  /* compare based on digits  */
  const_reverse_iterator d = rbegin();
  const_reverse_iterator d2 = rhs.rbegin();
  for (; d != rend(); ++d, ++d2)
  {
    if (*d > *d2)
      return 1;
    if (*d < *d2)
      return -1;
  }
  return 0;
}

template<class A, class T>
int mp_int<A,T>::compare_to_digit(digit_type d) const
{
  /* compare based on sign */
  if (is_negative())
    return -1;

  /* compare based on magnitude */
  if (used_ > 1)
    return 1;

  /* compare the only digit of *this to d */
  if (digits_[0] > d)
    return 1;
  else if (digits_[0] < d)
    return -1;
  else
    return 0;
}

template<class A, class T>
int mp_int<A,T>::compare(const mp_int& rhs) const
{
  /* compare based on sign */
  if (sign_ != rhs.sign_)
  {
    if (is_negative())
      return -1;
    else
      return 1;
  }
  
  /* compare digits */
  if (is_negative())
    /* if negative compare opposite direction */
    return rhs.compare_magnitude(*this);
  else
    return compare_magnitude(rhs);
}

// {A,B,C,D,E} shifted left by 2 digits becomes
// {0,0,A,B,C,D,E}
template<class A, class T>
void mp_int<A,T>::shift_digits_left(size_type b)
{
  if (b <= 0)
    return;

  grow_capacity(used_ + b);

  std::memmove(digits_ + b, digits_, used_ * sizeof(digit_type));

  // zero the lower digits
  std::memset(digits_, 0, b * sizeof(digit_type));

  used_ += b;
}

// {A,B,C,D,E} shifted right by 2 digits becomes
// {C,D,E}
template<class A, class T>
void mp_int<A,T>::shift_digits_right(size_type b)
{
  if (b <= 0)
    return;

  if (used_ <= b)
  {
    zero();
    return;
  }

  /* shift the digits down */
  std::memmove(digits_, digits_ + b, (used_ - b) * sizeof(digit_type));

  /* zero the top digits */
  std::memset(digits_ + used_ - b, 0, b * sizeof(digit_type));
  
  /* remove excess digits */
  used_ -= b;
}

template<class A, class T>
typename mp_int<A,T>::size_type
mp_int<A,T>::precision() const
{
  /* get number of digits and add that */
  size_type r = (used_ - 1) * valid_bits;
  
  /* take the last digit and count the bits in it */
  digit_type q = digits_[used_ - 1];
  while (q > 0U)
  {
    ++r;
    q >>= 1;
  }
  return r;
}

/* Counts the number of lsbs which are zero before the first one bit */
template<class A, class T>
typename mp_int<A,T>::size_type
mp_int<A,T>::count_lsb() const
{
  static const size_type lnz[16] = {
    4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
  };

  /* easy out */
  if (is_zero())
    return 0;

  /* scan lower digits until non-zero */
  size_type x = 0;
  while (x < used_ && digits_[x] == 0)
    ++x;
  digit_type q = digits_[x];
  x *= valid_bits;

  /* now scan this digit until a 1 is found */
  if ((q & 1) == 0)
  {
    digit_type qq;
    do
    {
      qq  = q & 15;
      x  += lnz[qq];
      q >>= 4;
    } while (qq == 0);
  }
  return x;
}

template<class A, class T>
template<typename IntegralT>
inline IntegralT mp_int<A,T>::to_integral() const
{
  return detail::integral_ops<IntegralT>::convert(*this);
}


template<class A, class T>
inline void swap(mp_int<A,T>& lhs, mp_int<A,T>& rhs)
{
  lhs.swap(rhs);
}

#ifdef BOOST_HAS_RVALUE_REFS
template<class A, class T>
inline void swap(mp_int<A,T>&& lhs, mp_int<A,T>& rhs)
{
  lhs.swap(rhs);
}
template<class A, class T>
inline void swap(mp_int<A,T>& lhs, mp_int<A,T>&& rhs)
{
  lhs.swap(rhs);
}
#endif






#include <boost/mp_math/mp_int/abs.hpp>
#include <boost/mp_math/mp_int/add.hpp>
#include <boost/mp_math/mp_int/ctors.hpp>
#include <boost/mp_math/mp_int/div.hpp>
#include <boost/mp_math/mp_int/gcd.hpp>
#include <boost/mp_math/mp_int/jacobi.hpp>
#include <boost/mp_math/mp_int/lcm.hpp>
#include <boost/mp_math/mp_int/mod.hpp>
#include <boost/mp_math/mp_int/modinv.hpp>
#include <boost/mp_math/mp_int/modular_reduction.hpp>
#include <boost/mp_math/mp_int/modpow.hpp>
#include <boost/mp_math/mp_int/mul.hpp>
#include <boost/mp_math/mp_int/operators.hpp>
#include <boost/mp_math/mp_int/pow.hpp>
#include <boost/mp_math/mp_int/random.hpp>
#include <boost/mp_math/mp_int/prime.hpp>
#include <boost/mp_math/mp_int/root.hpp>
#include <boost/mp_math/mp_int/sqr.hpp>
#include <boost/mp_math/mp_int/sub.hpp>
#include <boost/mp_math/mp_int/string_conversion.hpp>

} // namespace mp_math
} // namespace boost

#include <boost/mp_math/mp_int/numeric_limits.hpp>


#endif

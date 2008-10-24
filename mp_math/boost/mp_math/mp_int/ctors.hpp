// Copyright Kevin Sopp 2008.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

template<class A, class T>
mp_int<A,T>::mp_int()
:
  digits_(0),
  used_(0),
  capacity_(0),
  sign_(1)
{
}

template<class A, class T>
template<typename IntegralT>
mp_int<A,T>::mp_int(
    IntegralT b,
    typename enable_if<is_integral<IntegralT> >::type*)
:
  digits_(0),
  used_(0),
  capacity_(0)
{
  detail::integral_ops<IntegralT>::assign(*this, b);
}

template<class A, class T>
template<typename RandomAccessIterator>
void mp_int<A,T>::init(RandomAccessIterator c, RandomAccessIterator last)
{
  assert(used_ == 0);

  if (c == last)
  {
    sign_ = 1;
    return;
  }

  int sign;

  if (*c == '-')
  {
    ++c;
    sign = -1;
  }
  else
  {
    if (*c == '+')
      ++c;
    sign = 1;
  }
 
  // detect the radix
  unsigned int radix;
  
  if (c != last)
  {
    if (*c == '0') // octal
    {
      ++c;
      if (c != last && (*c == 'x' || *c == 'X')) // hex
      {
        radix = 16;
        ++c;
      }
      else
      {
        radix = 8;
        --c; // keep the zero, necessary for mp_int("0")
      }
    }
    else // decimal
      radix = 10;
  }
  else
    throw std::invalid_argument("mp_int ctor: malformed string");

  sign_ = sign;

  from_string(c, last, radix);
}

template<class A, class T>
template<typename RandomAccessIterator>
void mp_int<A,T>::init(RandomAccessIterator c,
                       RandomAccessIterator last,
                       std::ios_base::fmtflags f)
{
  assert(used_ == 0);

  if (c == last)
  {
    sign_ = 1;
    return;
  }

  if (*c == '-')
  {
    sign_ = -1;
    ++c;
  }
  else
  {
    if (f & std::ios_base::showpos)
    {
      if (*c == '+')
        ++c;
      else
        throw std::invalid_argument("mp_int<>::init: expected a '+' sign");
    }
    sign_ = 1;
  }
  
  const bool uppercase = f & std::ios_base::uppercase;
  const bool showbase  = f & std::ios_base::showbase;

  bool bad_prefix = false;
  unsigned radix;

  if (f & std::ios_base::hex)
  {
    if (showbase)
    {
      if (*c == '0')
        ++c;
      else
        bad_prefix = true;
      if (*c == 'x' || (*c == 'X' && uppercase))
        ++c;
      else
        bad_prefix = true;
    }
    radix = 16;
  }
  else if (f & std::ios_base::oct)
  {
    if (showbase)
    {
      if (*c == '0')
        ++c;
      else
        bad_prefix = true;
    }
    radix = 8;
  }
  else if (f & std::ios_base::dec)
    radix = 10;
  else
    throw std::invalid_argument("mp_int<>::init: unknown radix");

  if (bad_prefix)
    throw std::invalid_argument("mp_int<>::init: bad radix prefix");

  from_string(c, last, radix);
}


template<class A, class T>
template<typename RandomAccessIterator>
mp_int<A,T>::mp_int(RandomAccessIterator first, RandomAccessIterator last)
:
  digits_(0),
  used_(0),
  capacity_(0)
{
  init(first, last);
}

template<class A, class T>
mp_int<A,T>::mp_int(const char* s)
:
  digits_(0),
  used_(0),
  capacity_(0)
{
  init(s, s + std::strlen(s));
}

template<class A, class T>
mp_int<A,T>::mp_int(const char* s, std::ios_base::fmtflags f)
:
  digits_(0),
  used_(0),
  capacity_(0)
{
  init(s, s + std::strlen(s), f);
}


#ifndef BOOST_NO_CWCHAR
template<class A, class T>
mp_int<A,T>::mp_int(const wchar_t* s)
:
  digits_(0),
  used_(0),
  capacity_(0)
{
  init(s, s + std::wcslen(s));
}

  template<class A, class T>
mp_int<A,T>::mp_int(const wchar_t* s, std::ios_base::fmtflags f)
:
  digits_(0),
  used_(0),
  capacity_(0)
{
  init(s, s + std::wcslen(s), f);
}
#endif

template<class A, class T>
template<typename charT, class traits, class Alloc>
mp_int<A,T>::mp_int(const std::basic_string<charT,traits,Alloc>& s)
:
  digits_(0),
  used_(0),
  capacity_(0)
{
  init(s.begin(), s.end());
}

template<class A, class T>
template<typename charT, class traits, class Alloc>
mp_int<A,T>::mp_int(const std::basic_string<charT,traits,Alloc>& s,
                    std::ios_base::fmtflags f)
:
  digits_(0),
  used_(0),
  capacity_(0)
{
  init(s.begin(), s.end(), f);
}


template<class A, class T>
mp_int<A,T>::mp_int(const mp_int& copy)
{
  digits_ = this->allocate(copy.used_);
  std::memcpy(digits_, copy.digits_, copy.used_ * sizeof(digit_type));
  used_ = copy.used_;
  capacity_ = copy.used_;
  sign_ = copy.sign_;
}

#ifdef BOOST_HAS_RVALUE_REFS
template<class A, class T>
mp_int<A,T>::mp_int(mp_int&& copy)
:
  digits_(copy.digits_),
  used_(copy.used_),
  capacity_(copy.capacity_),
  sign_(copy.sign_)
{
  copy.digits_ = 0;
  copy.used_ = 0;
  copy.capacity_ = 0;
  copy.sign_ = 1;
}
#endif


template<class A, class T>
mp_int<A,T>::~mp_int()
{
  this->deallocate(digits_, capacity_);
}


// Copyright Kevin Sopp 2008 - 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_DETAIL_STREAM_IO_HPP
#define BOOST_MP_MATH_INTEGER_DETAIL_STREAM_IO_HPP

#include <iosfwd>

namespace boost {
namespace mp_math {
namespace detail {


template<class ApInt>
struct stream_io
{
  template<typename charT, class traits>
  static void read(ApInt& z, std::basic_istream<charT, traits>& is);
};


template<class ApInt>
template<typename charT, class traits>
void stream_io<ApInt>::read(ApInt& z, std::basic_istream<charT, traits>& is)
{
  typename std::basic_istream<charT, traits>::sentry sentry(is);
  if (!sentry)
    return;

  // TODO we read into a string first which costs memory and std::string
  // allocator is not under user control. We should convert incoming digits
  // directly. Actually we should check what is the fastest way.
  std::string s;

  const std::istreambuf_iterator<charT, traits> end;
  std::istreambuf_iterator<charT, traits> c(is);

  // TODO stop at minus sign if unsigned ApInt
  if (*c == '+' || *c == '-')
  {
    s.push_back(*c);
    ++c;
  }

  int base;
  if (*c == '0')
  {
    base = 8;
    s.push_back(*c);
    ++c;
    if (*c == 'x' || *c == 'X')
    {
      base = 16;
      s.push_back(*c);
      ++c;
    }
  }
  else if (*c >= '0' && *c <= '9')
    base = 10;
  else
  {
    is.setstate(std::ios_base::failbit);
    return;
  }

  switch (base)
  {
    case 8:
      while (c != end)
      {
        if (*c >= '0' && *c <= '7')
          s.push_back(*c);
        else
          break;
        ++c;
      }
      break;
    case 10:
      while (c != end)
      {
        if (*c >= '0' && *c <= '9')
          s.push_back(*c);
        else
          break;
        ++c;
      }
      break;
    case 16:
      while (c != end)
      {
        if ((*c >= '0' && *c <= '9') ||
            (*c >= 'A' && *c <= 'F') ||
            (*c >= 'a' && *c <= 'f'))
          s.push_back(*c);
        else
          break;
        ++c;
      }
      break;
  }

  // TODO we do another round of radix detection in the string ctor...
  const ApInt tmp(s.begin(), s.end());
  z = tmp; // TODO move tmp into z
}



} // namespace detail
} // namespace mp_math
} // namespace boost

#endif


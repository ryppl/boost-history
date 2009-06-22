// Copyright Kevin Sopp 2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MP_MATH_INTEGER_UNBOUNDED_HPP
#define BOOST_MP_MATH_INTEGER_UNBOUNDED_HPP

#include <boost/config.hpp>
#include <boost/mp_math/integer/unbounded_int.hpp>
#include <boost/mp_math/integer/unbounded_uint.hpp>
#include <boost/mp_math/integer/unbounded_traits.hpp>

namespace boost {
namespace mp_math {

template<
  bool IsSigned = true,
  class Alloc = std::allocator<void>,
  class Traits = unbounded_traits<>
>
struct unbounded
:
  mpl::if_c<
    IsSigned,
    unbounded_int<Alloc,Traits>,
    unbounded_uint<Alloc,Traits>
  >::type
{
  typedef typename mpl::if_c<
    IsSigned,
    unbounded_int<Alloc,Traits>,
    unbounded_uint<Alloc,Traits>
  >::type base_type;

  // these constants are used by the numeric_limits specialization
  static const bool is_signed = IsSigned;
  static const bool is_bounded = false;

  typedef typename base_type::traits_type traits_type;
  typedef typename base_type::digit_type  digit_type;
  typedef typename base_type::size_type   size_type;

  unbounded(){}

  #if !defined(BOOST_NO_VARIADIC_TEMPLATES) &&\
      !defined(BOOST_NO_RVALUE_REFERENCES)
  template<typename... Args>
  unbounded(Args&&... args)
  :
    base_type(args...)
  {}

  #else

  template<typename T1>
  unbounded(const T1& t1)
  : base_type(t1) {}

  template<typename T1, typename T2>
  unbounded(const T1& t1, const T2& t2)
  : base_type(t1, t2) {}

  template<typename T1, typename T2, typename T3>
  unbounded(const T1& t1, const T2& t2, const T3& t3)
  : base_type(t1, t2, t3) {}

  template<typename T1, typename T2, typename T3, typename T4>
  unbounded(const T1& t1, const T2& t2, const T3& t3, const T4& t4)
  : base_type(t1, t2, t3, t4) {}

  template<typename T1, typename T2, typename T3, typename T4, typename T5>
  unbounded(const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5)
  : base_type(t1, t2, t3, t4, t5) {}
  #endif

  template<typename T>
  #ifndef BOOST_NO_RVALUE_REFERENCES
  unbounded& operator = (T&& t)
  #else
  unbounded& operator = (const T& t)
  #endif
  {
    base_type::operator=(t);
    return *this;
  }

  operator base_type&       ()       { return *this; }
  operator const base_type& () const { return *this; }
};


template<bool S, class A, class T>
struct modpow_ctx<unbounded<S,A,T> >
:
  modpow_ctx<typename unbounded<S,A,T>::base_type>
{};

// returns base^exp % mod
template<bool S, class A, class T>
inline
unbounded<S,A,T> modpow(const unbounded<S,A,T>& base,
                        const unbounded<S,A,T>& exp,
                        const unbounded<S,A,T>& mod,
                        modpow_ctx<unbounded<S,A,T> >* ctx = 0)
{
  return modpow(static_cast<const typename unbounded<S,A,T>::base_type&>(base),
                static_cast<const typename unbounded<S,A,T>::base_type&>(exp),
                static_cast<const typename unbounded<S,A,T>::base_type&>(mod),
                static_cast<modpow_ctx<typename unbounded<S,A,T>::base_type>*>(ctx));
}


} // namespace mp_math
} // namespace boost

#endif


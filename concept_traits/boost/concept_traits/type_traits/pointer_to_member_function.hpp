///////////////////////////////////////////////////////////////////////////////
// Concept traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_TT_EXT_POINTER_TO_MEMBER_FUNCTION_HPP
#define BOOST_CT_TT_EXT_POINTER_TO_MEMBER_FUNCTION_HPP

#include <boost/mpl/void.hpp>

namespace boost {

///////////////////////////////////////////////////////////////////////////////
// pointer_to_member_function
///////////////////////////////////////////////////////////////////////////////

template<
  class T,
  class R,
  class P1 = mpl::void_,
  class P2 = mpl::void_,
  class P3 = mpl::void_,
  class P4 = mpl::void_,
  class P5 = mpl::void_
>
struct pointer_to_member_function
{
  typedef R (T::*type)(P1,P2,P3,P4,P5);
};

template<
  class T,
  class R
>
struct pointer_to_member_function<
  T,
  R,
  mpl::void_,
  mpl::void_,
  mpl::void_,
  mpl::void_,
  mpl::void_
>
{
  typedef R (T::*type)();
};

template<
  class T,
  class R,
  class P1
>
struct pointer_to_member_function<
  T,
  R,
  P1,
  mpl::void_,
  mpl::void_,
  mpl::void_,
  mpl::void_
>
{
  typedef R (T::*type)(P1);
};

template<
  class T,
  class R,
  class P1,
  class P2
>
struct pointer_to_member_function<
  T,
  R,
  P1,
  P2,
  mpl::void_,
  mpl::void_,
  mpl::void_
>
{
  typedef R (T::*type)(P1,P2);
};

template<
  class T,
  class R,
  class P1,
  class P2,
  class P3
>
struct pointer_to_member_function<
  T,
  R,
  P1,
  P2,
  P3,
  mpl::void_,
  mpl::void_
>
{
  typedef R (T::*type)(P1,P2,P3);
};

template<
  class T,
  class R,
  class P1,
  class P2,
  class P3,
  class P4
>
struct pointer_to_member_function<
  T,
  R,
  P1,
  P2,
  P3,
  P4,
  mpl::void_
>
{
  typedef R (T::*type)(P1,P2,P3,P4);
};

template<
  class T,
  class R
>
struct pointer_to_member_function<
  const T,
  R,
  mpl::void_,
  mpl::void_,
  mpl::void_,
  mpl::void_,
  mpl::void_
>
{
  typedef R (T::*type)() const;
};

template<
  class T,
  class R,
  class P1
>
struct pointer_to_member_function<
  const T,
  R,
  P1,
  mpl::void_,
  mpl::void_,
  mpl::void_,
  mpl::void_
>
{
  typedef R (T::*type)(P1) const;
};

template<
  class T,
  class R,
  class P1,
  class P2
>
struct pointer_to_member_function<
  const T,
  R,
  P1,
  P2,
  mpl::void_,
  mpl::void_,
  mpl::void_
>
{
  typedef R (T::*type)(P1,P2) const;
};

template<
  class T,
  class R,
  class P1,
  class P2,
  class P3
>
struct pointer_to_member_function<
  const T,
  R,
  P1,
  P2,
  P3,
  mpl::void_,
  mpl::void_
>
{
  typedef R (T::*type)(P1,P2,P3) const;
};

template<
  class T,
  class R,
  class P1,
  class P2,
  class P3,
  class P4
>
struct pointer_to_member_function<
  const T,
  R,
  P1,
  P2,
  P3,
  P4,
  mpl::void_
>
{
  typedef R (T::*type)(P1,P2,P3,P4) const;
};

template<
  class T,
  class R,
  class P1,
  class P2,
  class P3,
  class P4,
  class P5
>
struct pointer_to_member_function<
  const T,
  R,
  P1,
  P2,
  P3,
  P4,
  P5
>
{
  typedef R (T::*type)(P1,P2,P3,P4,P5) const;
};

} // namespace boost

#endif // BOOST_CT_TT_EXT_POINTER_TO_MEMBER_FUNCTION_HPP

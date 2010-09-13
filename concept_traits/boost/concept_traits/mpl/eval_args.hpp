///////////////////////////////////////////////////////////////////////////////
// Concept traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_MPL_EXT_EVAL_ARGS_HPP
#define BOOST_CT_MPL_EXT_EVAL_ARGS_HPP

namespace boost {
namespace mpl {

///////////////////////////////////////////////////////////////////////////////
// eval_args
///////////////////////////////////////////////////////////////////////////////

template<class T>
struct eval_args;

template<
  template<
    class
  > class F,
  class P1
>
struct eval_args<F<P1> >
{
  typedef F<
    typename P1::type
  > type;
};

template<
  template<
    class,
    class
  > class F,
  class P1,
  class P2
>
struct eval_args<F<P1,P2> >
{
  typedef F<
    typename P1::type,
    typename P2::type
  > type;
};

template<
  template<
    class,
    class,
    class
  > class F,
  class P1,
  class P2,
  class P3
>
struct eval_args<F<P1,P2,P3> >
{
  typedef F<
    typename P1::type,
    typename P2::type,
    typename P3::type
  > type;
};

template<
  template<
    class,
    class,
    class,
    class
  > class F,
  class P1,
  class P2,
  class P3,
  class P4
>
struct eval_args<F<P1,P2,P3,P4> >
{
  typedef F<
    typename P1::type,
    typename P2::type,
    typename P3::type,
    typename P4::type
  > type;
};

template<
  template<
    class,
    class,
    class,
    class,
    class
  > class F,
  class P1,
  class P2,
  class P3,
  class P4,
  class P5
>
struct eval_args<F<P1,P2,P3,P4,P5> >
{
  typedef F<
    typename P1::type,
    typename P2::type,
    typename P3::type,
    typename P4::type,
    typename P5::type
  > type;
};

} // namespace mpl
} // namespace boost

#endif // BOOST_CT_MPL_EXT_EVAL_ARGS_HPP

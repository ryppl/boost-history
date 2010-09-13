///////////////////////////////////////////////////////////////////////////////
// Concept traits
//
// (C) Copyright Terje Slettebø and Tobias Schwinger 2004.
// Use, modification and distribution are subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt).
///////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_CT_MPL_EXT_EVAL_HPP
#define BOOST_CT_MPL_EXT_EVAL_HPP

namespace boost {
namespace mpl {

///////////////////////////////////////////////////////////////////////////////
// eval
///////////////////////////////////////////////////////////////////////////////

template<class T>
struct eval;

template<
  template<
    class
  > class F,
  class P1
>
struct eval<F<P1> > : F<
  typename P1::type
> {};

template<
  template<
    class,
    class
  > class F,
  class P1,
  class P2
>
struct eval<F<P1,P2> > : F<
  typename P1::type,
  typename P2::type
> {};

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
struct eval<F<P1,P2,P3> > : F<
  typename P1::type,
  typename P2::type,
  typename P3::type
> {};

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
struct eval<F<P1,P2,P3,P4> > : F<
  typename P1::type,
  typename P2::type,
  typename P3::type,
  typename P4::type
> {};

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
struct eval<F<P1,P2,P3,P4,P5> > : F<
  typename P1::type,
  typename P2::type,
  typename P3::type,
  typename P4::type,
  typename P5::type
> {};

template<
  template<
    class,
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
  class P5,
  class P6
>
struct eval<F<P1,P2,P3,P4,P5,P6> > : F<
  typename P1::type,
  typename P2::type,
  typename P3::type,
  typename P4::type,
  typename P5::type,
  typename P6::type
> {};

template<
  template<
    class,
    class,
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
  class P5,
  class P6,
  class P7
>
struct eval<F<P1,P2,P3,P4,P5,P6,P7> > : F<
  typename P1::type,
  typename P2::type,
  typename P3::type,
  typename P4::type,
  typename P5::type,
  typename P6::type,
  typename P7::type
> {};

template<
  template<
    class,
    class,
    class,
    class,
    class,
    class,
    class,
    class> class F,
  class P1,
  class P2,
  class P3,
  class P4,
  class P5,
  class P6,
  class P7,
  class P8
>
struct eval<F<P1,P2,P3,P4,P5,P6,P7,P8> > : F<
  typename P1::type,
  typename P2::type,
  typename P3::type,
  typename P4::type,
  typename P5::type,
  typename P6::type,
  typename P7::type,
  typename P8::type
> {};

} // namespace mpl
} // namespace boost

#endif // BOOST_CT_MPL_EXT_EVAL_HPP

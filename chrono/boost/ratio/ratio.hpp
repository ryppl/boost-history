//  ratio.hpp  ---------------------------------------------------------------//

//  Copyright 2008 Howard Hinnant
//  Copyright 2008 Beman Dawes
//  Copyright 2009 Vicente J. Botet Escriba

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

/*

This code was derived by Beman Dawes from Howard Hinnant's time2_demo prototype.
Many thanks to Howard for making his code available under the Boost license.
The original code was modified to conform to Boost conventions and to section
20.4 Compile-time rational arithmetic [ratio], of the C++ committee working
paper N2798.
See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2008/n2798.pdf.

time2_demo contained this comment:

    Much thanks to Andrei Alexandrescu,
                   Walter Brown,
                   Peter Dimov,
                   Jeff Garland,
                   Terry Golubiewski,
                   Daniel Kr�gler,
                   Anthony Williams.
*/

#ifndef BOOST_RATIO_RATIO__HPP
#define BOOST_RATIO_RATIO_HPP

#include <cstdlib>
#include <climits>
#include <limits>
#include <boost/cstdint.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/integer_traits.hpp>

#if !defined(BOOST_NO_STATIC_ASSERT) || !defined(BOOST_RATIO_USES_MPL_ASSERT)
#define BOOST_RATIO_OVERFLOW_IN_ADD "overflow in ratio add"
#define BOOST_RATIO_OVERFLOW_IN_ADD "overflow in ratio add"
#define BOOST_RATIO_OVERFLOW_IN_SUB "overflow in ratio sub"
#define BOOST_RATIO_OVERFLOW_IN_MUL "overflow in ratio mul"
#define BOOST_RATIO_OVERFLOW_IN_DIV "overflow in ratio div"
#define BOOST_RATIO_RATIO_NUMERATOR_IS_OUT_OF_RANGE "ratio numerator is out of range"
#define BOOST_RATIO_RATIO_DIVIDE_BY_0 "ratio divide by 0"
#define BOOST_RATIO_RATIO_DENOMINATOR_IS_OUT_OF_RANGE "ratio denominator is out of range"
#endif

#ifndef BOOST_NO_STATIC_ASSERT
#define BOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES) static_assert(CND,MSG)
#elif defined(BOOST_RATIO_USES_STATIC_ASSERT)
#include <boost/static_assert.hpp>
#define BOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES) BOOST_STATIC_ASSERT(CND)
#elif defined(BOOST_RATIO_USES_MPL_ASSERT)
#include <boost/mpl/assert.hpp>
#include <boost/mpl/bool.hpp>
#define BOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES)                                 \
    BOOST_MPL_ASSERT_MSG(boost::mpl::bool_< (CND) >::type::value, MSG, TYPES)
#else
//~ #elif defined(BOOST_RATIO_USES_ARRAY_ASSERT)
#define BOOST_RATIO_CONCAT(A,B) A##B
#define BOOST_RATIO_NAME(A,B) BOOST_RATIO_CONCAT(A,B)
#define BOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES) static char BOOST_RATIO_NAME(__boost_ratio_test_,__LINE__)[CND]
//~ #define BOOST_RATIO_STATIC_ASSERT(CND, MSG, TYPES)
#endif

#ifdef INTMAX_C
#define BOOST_INTMAX_C(a) INTMAX_C(a)
#else
#define BOOST_INTMAX_C(a) a##LL
#endif

//
// We simply cannot include this header on gcc without getting copious warnings of the kind:
//
// boost/integer.hpp:77:30: warning: use of C99 long long integer constant
//
// And yet there is no other reasonable implementation, so we declare this a system header
// to suppress these warnings.
//
#if defined(__GNUC__) && (__GNUC__ >= 4)
#pragma GCC system_header
#endif

namespace boost
{

//----------------------------------------------------------------------------//
//                                                                            //
//              20.4 Compile-time rational arithmetic [ratio]                 //
//                                                                            //
//----------------------------------------------------------------------------//

template <boost::intmax_t N, boost::intmax_t D = 1> class ratio;

// ratio arithmetic
template <class R1, class R2> struct ratio_add;
template <class R1, class R2> struct ratio_subtract;
template <class R1, class R2> struct ratio_multiply;
template <class R1, class R2> struct ratio_divide;

// ratio comparison
template <class R1, class R2> struct ratio_equal;
template <class R1, class R2> struct ratio_not_equal;
template <class R1, class R2> struct ratio_less;
template <class R1, class R2> struct ratio_less_equal;
template <class R1, class R2> struct ratio_greater;
template <class R1, class R2> struct ratio_greater_equal;

// convenience SI typedefs
typedef ratio<BOOST_INTMAX_C(1), BOOST_INTMAX_C(1000000000000000000)> atto;
typedef ratio<BOOST_INTMAX_C(1),    BOOST_INTMAX_C(1000000000000000)> femto;
typedef ratio<BOOST_INTMAX_C(1),       BOOST_INTMAX_C(1000000000000)> pico;
typedef ratio<BOOST_INTMAX_C(1),          BOOST_INTMAX_C(1000000000)> nano;
typedef ratio<BOOST_INTMAX_C(1),             BOOST_INTMAX_C(1000000)> micro;
typedef ratio<BOOST_INTMAX_C(1),                BOOST_INTMAX_C(1000)> milli;
typedef ratio<BOOST_INTMAX_C(1),                 BOOST_INTMAX_C(100)> centi;
typedef ratio<BOOST_INTMAX_C(1),                  BOOST_INTMAX_C(10)> deci;
typedef ratio<                 BOOST_INTMAX_C(10), BOOST_INTMAX_C(1)> deca;
typedef ratio<                BOOST_INTMAX_C(100), BOOST_INTMAX_C(1)> hecto;
typedef ratio<               BOOST_INTMAX_C(1000), BOOST_INTMAX_C(1)> kilo;
typedef ratio<            BOOST_INTMAX_C(1000000), BOOST_INTMAX_C(1)> mega;
typedef ratio<         BOOST_INTMAX_C(1000000000), BOOST_INTMAX_C(1)> giga;
typedef ratio<      BOOST_INTMAX_C(1000000000000), BOOST_INTMAX_C(1)> tera;
typedef ratio<   BOOST_INTMAX_C(1000000000000000), BOOST_INTMAX_C(1)> peta;
typedef ratio<BOOST_INTMAX_C(1000000000000000000), BOOST_INTMAX_C(1)> exa;

//----------------------------------------------------------------------------//
//                                 helpers                                    //
//----------------------------------------------------------------------------//

namespace detail
{

  // static_gcd

  template <boost::intmax_t X, boost::intmax_t Y>
  struct static_gcd
  {
      static const boost::intmax_t value = static_gcd<Y, X % Y>::value;
  };

  template <boost::intmax_t X>
  struct static_gcd<X, 0>
  {
      static const boost::intmax_t value = X;
  };

  // static_lcm

  template <boost::intmax_t X, boost::intmax_t Y>
  struct static_lcm
  {
      static const boost::intmax_t value = X / static_gcd<X, Y>::value * Y;
  };

  template <boost::intmax_t X>
  struct static_abs
  {
      static const boost::intmax_t value = X < 0 ? -X : X;
  };

  template <boost::intmax_t X>
  struct static_sign
  {
      static const boost::intmax_t value = X == 0 ? 0 : (X < 0 ? -1 : 1);
  };

  template <boost::intmax_t X, boost::intmax_t Y, boost::intmax_t = static_sign<Y>::value>
  class ll_add;

  template <boost::intmax_t X, boost::intmax_t Y>
  class ll_add<X, Y, 1>
  {
      #if BOOST_RATIO_OLD_MIN_MAX
      static const boost::intmax_t min =
        (BOOST_INTMAX_C(1) << (sizeof(boost::intmax_t) * CHAR_BIT - 1)) + 1;
      static const boost::intmax_t max = -min;
      #else
      static const boost::intmax_t min = boost::integer_traits<boost::intmax_t>::const_min;
      static const boost::intmax_t max = boost::integer_traits<boost::intmax_t>::const_max;
      #endif

      BOOST_RATIO_STATIC_ASSERT(X <= max - Y , BOOST_RATIO_OVERFLOW_IN_ADD, ());
  public:
      static const boost::intmax_t value = X + Y;
  };

  template <boost::intmax_t X, boost::intmax_t Y>
  class ll_add<X, Y, 0>
  {
  public:
      static const boost::intmax_t value = X;
  };

  template <boost::intmax_t X, boost::intmax_t Y>
  class ll_add<X, Y, -1>
  {
      #if BOOST_RATIO_OLD_MIN_MAX
      static const boost::intmax_t min =
        (BOOST_INTMAX_C(1) << (sizeof(boost::intmax_t) * CHAR_BIT - 1)) + 1;
      static const boost::intmax_t max = -min;
      #else
      static const boost::intmax_t min = boost::integer_traits<boost::intmax_t>::const_min;
      static const boost::intmax_t max = boost::integer_traits<boost::intmax_t>::const_max;
      #endif

      BOOST_RATIO_STATIC_ASSERT(min - Y <= X, BOOST_RATIO_OVERFLOW_IN_ADD, ());
  public:
      static const boost::intmax_t value = X + Y;
  };

  template <boost::intmax_t X, boost::intmax_t Y, boost::intmax_t = static_sign<Y>::value>
  class ll_sub;

  template <boost::intmax_t X, boost::intmax_t Y>
  class ll_sub<X, Y, 1>
  {
      #if BOOST_RATIO_OLD_MIN_MAX
      static const boost::intmax_t min =
        (BOOST_INTMAX_C(1) << (sizeof(boost::intmax_t) * CHAR_BIT - 1)) + 1;
      static const boost::intmax_t max = -min;
      #else
      static const boost::intmax_t min = boost::integer_traits<boost::intmax_t>::const_min;
      static const boost::intmax_t max = boost::integer_traits<boost::intmax_t>::const_max;
      #endif

      BOOST_RATIO_STATIC_ASSERT(min + Y <= X, BOOST_RATIO_OVERFLOW_IN_SUB, ());
  public:
      static const boost::intmax_t value = X - Y;
  };

  template <boost::intmax_t X, boost::intmax_t Y>
  class ll_sub<X, Y, 0>
  {
  public:
      static const boost::intmax_t value = X;
  };

  template <boost::intmax_t X, boost::intmax_t Y>
  class ll_sub<X, Y, -1>
  {
      #if BOOST_RATIO_OLD_MIN_MAX
      static const boost::intmax_t min =
        (BOOST_INTMAX_C(1) << (sizeof(boost::intmax_t) * CHAR_BIT - 1)) + 1;
      static const boost::intmax_t max = -min;
      #else
      static const boost::intmax_t min = boost::integer_traits<boost::intmax_t>::const_min;
      static const boost::intmax_t max = boost::integer_traits<boost::intmax_t>::const_max;
      #endif

      BOOST_RATIO_STATIC_ASSERT(X <= max + Y, BOOST_RATIO_OVERFLOW_IN_SUB, ());
  public:
      static const boost::intmax_t value = X - Y;
  };

  template <boost::intmax_t X, boost::intmax_t Y>
  class ll_mul
  {
      static const boost::intmax_t nan =
        (BOOST_INTMAX_C(1) << (sizeof(boost::intmax_t) * CHAR_BIT - 1));
      #if BOOST_RATIO_OLD_MIN_MAX
      static const boost::intmax_t min = nan + 1;
      static const boost::intmax_t max = -min;
      #else
      static const boost::intmax_t min = boost::integer_traits<boost::intmax_t>::const_min;
      static const boost::intmax_t max = boost::integer_traits<boost::intmax_t>::const_max;
      #endif

      static const boost::intmax_t a_x = static_abs<X>::value;
      static const boost::intmax_t a_y = static_abs<Y>::value;

      BOOST_RATIO_STATIC_ASSERT(X != nan, BOOST_RATIO_OVERFLOW_IN_MUL, ());
      BOOST_RATIO_STATIC_ASSERT(Y != nan, BOOST_RATIO_OVERFLOW_IN_MUL, ());
      BOOST_RATIO_STATIC_ASSERT(a_x <= max / a_y, BOOST_RATIO_OVERFLOW_IN_MUL, ());
  public:
      static const boost::intmax_t value = X * Y;
  };

  template <boost::intmax_t Y>
  class ll_mul<0, Y>
  {
  public:
      static const boost::intmax_t value = 0;
  };

  template <boost::intmax_t X>
  class ll_mul<X, 0>
  {
  public:
      static const boost::intmax_t value = 0;
  };

  template <>
  class ll_mul<0, 0>
  {
  public:
      static const boost::intmax_t value = 0;
  };

  // Not actually used but left here in case needed in future maintenance
  template <boost::intmax_t X, boost::intmax_t Y>
  class ll_div
  {
      static const boost::intmax_t nan = (1LL << (sizeof(boost::intmax_t) * CHAR_BIT - 1));
      #if BOOST_RATIO_OLD_MIN_MAX
      static const boost::intmax_t min = nan + 1;
      static const boost::intmax_t max = -min;
      #else
      static const boost::intmax_t min = boost::integer_traits<boost::intmax_t>::const_min;
      static const boost::intmax_t max = boost::integer_traits<boost::intmax_t>::const_max;
      #endif

      BOOST_RATIO_STATIC_ASSERT(X != nan, BOOST_RATIO_OVERFLOW_IN_DIV, ());
      BOOST_RATIO_STATIC_ASSERT(Y != nan, BOOST_RATIO_OVERFLOW_IN_DIV, ());
      BOOST_RATIO_STATIC_ASSERT(Y != 0, BOOST_RATIO_RATIO_DIVIDE_BY_0, ());
  public:
      static const boost::intmax_t value = X / Y;
  };

  template <class T>
    struct is_ratio : public boost::false_type {};
  template <intmax_t N, intmax_t D>
    struct is_ratio<ratio<N, D> > : public boost::true_type  {};
  //template <class T>
  //  struct is_ratio : is_ratio<typename remove_cv<T>::type> {};


}  // namespace detail

//----------------------------------------------------------------------------//
//                                                                            //
//                20.4.1 Class template ratio [ratio.ratio]                   //
//                                                                            //
//----------------------------------------------------------------------------//

template <boost::intmax_t N, boost::intmax_t D>
class ratio
{
    BOOST_RATIO_STATIC_ASSERT(boost::detail::static_abs<N>::value >= 0, BOOST_RATIO_RATIO_NUMERATOR_IS_OUT_OF_RANGE, ());
    BOOST_RATIO_STATIC_ASSERT(boost::detail::static_abs<D>::value > 0, BOOST_RATIO_RATIO_DENOMINATOR_IS_OUT_OF_RANGE, ());
    BOOST_RATIO_STATIC_ASSERT(D != 0, BOOST_RATIO_RATIO_DIVIDE_BY_0 , ());
    static const boost::intmax_t m_na = boost::detail::static_abs<N>::value;
    static const boost::intmax_t m_da = boost::detail::static_abs<D>::value;
    static const boost::intmax_t m_s = boost::detail::static_sign<N>::value
      * boost::detail::static_sign<D>::value;
    static const boost::intmax_t m_gcd = boost::detail::static_gcd<m_na, m_da>::value;
public:
    static const boost::intmax_t num = m_s * m_na / m_gcd;
    static const boost::intmax_t den = m_da / m_gcd;

    ratio() {}

    template <intmax_t _N2, intmax_t _D2>
    ratio(const ratio<_N2, _D2>&,
        typename enable_if_c
            <
                (ratio<_N2, _D2>::num == num &&
                ratio<_N2, _D2>::den == den)
            >::type* = 0) {}

    template <intmax_t _N2, intmax_t _D2>
        typename enable_if_c
        <
            (ratio<_N2, _D2>::num == num &&
            ratio<_N2, _D2>::den == den),
            ratio&
        >::type
    operator=(const ratio<_N2, _D2>&) {return *this;}

    typedef ratio<num, den> type;
};

//----------------------------------------------------------------------------//
//                                                                            //
//                             Implementation                                 //
//                                                                            //
//----------------------------------------------------------------------------//

template <class R1, class R2>
struct ratio_add
{
#if 0
public:
    //The nested typedef type shall be a synonym for ratio<T1, T2>::type where T1 has the value R1::num *
    //R2::den + R2::num * R1::den and T2 has the value R1::den * R2::den.
    typedef typename ratio<R1::num * R2::den + R2::num * R1::den,R1::den * R2::den>::type type;
    // The preceding declaration doesn't works because of overflow on intmax_t.
#else
private:
    static const boost::intmax_t gcd_n1_n2 = boost::detail::static_gcd<R1::num, R2::num>::value;
    static const boost::intmax_t gcd_d1_d2 = boost::detail::static_gcd<R1::den, R2::den>::value;
public:
    // No need to normalize as ratio_multiply is already normalized
    typedef typename ratio_multiply
       <
           ratio<gcd_n1_n2, R1::den / gcd_d1_d2>,
           ratio
           <
               boost::detail::ll_add
               <
                   boost::detail::ll_mul<R1::num / gcd_n1_n2, R2::den / gcd_d1_d2>::value,
                   boost::detail::ll_mul<R2::num / gcd_n1_n2, R1::den / gcd_d1_d2>::value
               >::value,
               R2::den
           >
       >::type type;
#endif
};

template <class R1, class R2>
struct ratio_subtract
{
#if 0
public:
    //The nested typedef type shall be a synonym for ratio<T1, T2>::type where T1 has the value
    // R1::num *R2::den - R2::num * R1::den and T2 has the value R1::den * R2::den.
    typedef typename ratio<R1::num * R2::den - R2::num * R1::den, R1::den * R2::den>::type type;
    // The preceding declaration doesn't works because of overflow on intmax_t.
#else
private:
    static const boost::intmax_t gcd_n1_n2 = boost::detail::static_gcd<R1::num, R2::num>::value;
    static const boost::intmax_t gcd_d1_d2 = boost::detail::static_gcd<R1::den, R2::den>::value;
public:
    // No need to normalize as ratio_multiply is already normalized
    typedef typename ratio_multiply
       <
           ratio<gcd_n1_n2, R1::den / gcd_d1_d2>,
           ratio
           <
               boost::detail::ll_sub
               <
                   boost::detail::ll_mul<R1::num / gcd_n1_n2, R2::den / gcd_d1_d2>::value,
                   boost::detail::ll_mul<R2::num / gcd_n1_n2, R1::den / gcd_d1_d2>::value
               >::value,
               R2::den
           >
       >::type type;
#endif
};

template <class R1, class R2>
struct ratio_multiply
{
#if 0
public:
    // The nested typedef type  shall be a synonym for ratio<R1::num * R2::den - R2::num * R1::den, R1::den * R2::den>::type.
    typedef typename ratio<R1::num * R2::num, R1::den * R2::den>::type type;
    // The preceding declaration doesn't works because of overflow on intmax_t.
#else
private:
   static const boost::intmax_t gcd_n1_d2 = boost::detail::static_gcd<R1::num, R2::den>::value;
   static const boost::intmax_t gcd_d1_n2 = boost::detail::static_gcd<R1::den, R2::num>::value;
public:
    typedef typename ratio
       <
           boost::detail::ll_mul<R1::num / gcd_n1_d2, R2::num / gcd_d1_n2>::value,
           boost::detail::ll_mul<R2::den / gcd_n1_d2, R1::den / gcd_d1_n2>::value
       >::type type;
#endif
};

template <class R1, class R2>
struct ratio_divide
{
#if 0
public:
    // The nested typedef type  shall be a synonym for ratio<R1::num * R2::den, R2::num * R1::den>::type.
    typedef typename ratio<R1::num * R2::den, R1::den * R2::num>::type type;
#else
private:
    static const boost::intmax_t gcd_n1_n2 = boost::detail::static_gcd<R1::num, R2::num>::value;
    static const boost::intmax_t gcd_d1_d2 = boost::detail::static_gcd<R1::den, R2::den>::value;
public:
    typedef typename ratio
       <
           boost::detail::ll_mul<R1::num / gcd_n1_n2, R2::den / gcd_d1_d2>::value,
           boost::detail::ll_mul<R2::num / gcd_n1_n2, R1::den / gcd_d1_d2>::value
       >::type type;
#endif
};

// ratio_equal

template <class R1, class R2>
struct ratio_equal
    : public boost::integral_constant<bool,
                               (R1::num == R2::num && R1::den == R2::den)> {};

template <class R1, class R2>
struct ratio_not_equal
    : public boost::integral_constant<bool, !ratio_equal<R1, R2>::value> {};

// ratio_less


//----------------------------------------------------------------------------//
//                              more helpers                                  //
//----------------------------------------------------------------------------//

namespace detail
{
  // Protect against overflow, and still get the right answer as much as possible.
  //   This just demonstrates for fun how far you can push things without hitting
  //   overflow.  The obvious and simple implementation is conforming.

  template <class R1, class R2, bool ok1, bool ok2>
  struct ratio_less3 // true, true and false, false
  {
      static const bool value = ll_mul<R1::num,
        R2::den>::value < ll_mul<R2::num, R1::den>::value;
  };

  template <class R1, class R2>
  struct ratio_less3<R1, R2, true, false>
  {
      static const bool value = true;
  };

  template <class R1, class R2>
  struct ratio_less3<R1, R2, false, true>
  {
      static const bool value = false;
  };

  template <class R1, class R2, bool = (R1::num < R1::den == R2::num < R2::den) >
  struct ratio_less2  // N1 < D1 == N2 < D2
  {
      static const intmax_t max = -((1LL << (sizeof(intmax_t) * CHAR_BIT - 1)) + 1);
      static const bool ok1 = R1::num <= max / R2::den;
      static const bool ok2 = R2::num <= max / R1::den;
      static const bool value = ratio_less3<R1, R2, ok1, ok2>::value;
  };

  template <class R1, class R2>
  struct ratio_less2<R1, R2, false>  // N1 < D1 != N2 < D2
  {
      static const bool value = R1::num < R1::den;
  };

  template <class R1, class R2, bool = (R1::num < R1::den == R2::num < R2::den) >
  struct ratio_less1  // N1 < D1 == N2 < D2
  {
      static const bool value = ratio_less2<ratio<R1::num, R2::num>,
                                            ratio<R1::den, R2::den> >::value;
  };

  template <class R1, class R2>
  struct ratio_less1<R1, R2, false>  // N1 < D1 != N2 < D2
  {
      static const bool value = R1::num < R1::den;
  };

  template <class R1, class R2, intmax_t S1 = static_sign<R1::num>::value,
                                intmax_t S2 = static_sign<R2::num>::value>
  struct ratio_less
  {
      static const bool value = S1 < S2;
  };

  template <class R1, class R2>
  struct ratio_less<R1, R2, 1LL, 1LL>
  {
      static const bool value = ratio_less1<R1, R2>::value;
  };

  template <class R1, class R2>
  struct ratio_less<R1, R2, -1LL, -1LL>
  {
      static const bool value = ratio_less1<ratio<-R2::num, R2::den>,
                                            ratio<-R1::num, R1::den> >::value;
  };

  template <class R1, class R2>
  struct ratio_gcd
  {
    typedef ratio<boost::detail::static_gcd<R1::num, R2::num>::value,
                  boost::detail::static_lcm<R1::den, R2::den>::value> type;
  };
}  // namespace detail

//----------------------------------------------------------------------------//
//                                                                            //
//                           more implementation                              //
//                                                                            //
//----------------------------------------------------------------------------//

template <class R1, class R2>
struct ratio_less
  : public boost::integral_constant<bool, boost::detail::ratio_less<R1, R2>::value> {};

template <class R1, class R2>
struct ratio_less_equal
    : public boost::integral_constant<bool, !ratio_less<R2, R1>::value> {};

template <class R1, class R2>
struct ratio_greater
    : public boost::integral_constant<bool, ratio_less<R2, R1>::value> {};

template <class R1, class R2>
struct ratio_greater_equal
    : public boost::integral_constant<bool, !ratio_less<R1, R2>::value> {};

}  // namespace boost

#ifdef BOOST_RATIO_STATIC_ASSERT
#undef BOOST_RATIO_STATIC_ASSERT
#endif

#endif  // BOOST_RATIO_HPP

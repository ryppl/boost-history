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
                   Daniel Krügler,
                   Anthony Williams.
*/

#ifndef BOOST_RATIO_HPP
#define BOOST_RATIO_HPP

#include <cstdlib>
#include <climits>
#include <limits>
#include <boost/cstdint.hpp>
#include <boost/type_traits.hpp>
#include <boost/utility/enable_if.hpp>

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
typedef ratio<INTMAX_C(1), INTMAX_C(1000000000000000000)> atto;
typedef ratio<INTMAX_C(1),    INTMAX_C(1000000000000000)> femto;
typedef ratio<INTMAX_C(1),       INTMAX_C(1000000000000)> pico;
typedef ratio<INTMAX_C(1),          INTMAX_C(1000000000)> nano;
typedef ratio<INTMAX_C(1),             INTMAX_C(1000000)> micro;
typedef ratio<INTMAX_C(1),                INTMAX_C(1000)> milli;
typedef ratio<INTMAX_C(1),                 INTMAX_C(100)> centi;
typedef ratio<INTMAX_C(1),                  INTMAX_C(10)> deci;
typedef ratio<                 INTMAX_C(10), INTMAX_C(1)> deca;
typedef ratio<                INTMAX_C(100), INTMAX_C(1)> hecto;
typedef ratio<               INTMAX_C(1000), INTMAX_C(1)> kilo;
typedef ratio<            INTMAX_C(1000000), INTMAX_C(1)> mega;
typedef ratio<         INTMAX_C(1000000000), INTMAX_C(1)> giga;
typedef ratio<      INTMAX_C(1000000000000), INTMAX_C(1)> tera;
typedef ratio<   INTMAX_C(1000000000000000), INTMAX_C(1)> peta;
typedef ratio<INTMAX_C(1000000000000000000), INTMAX_C(1)> exa;

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
      static const boost::intmax_t min =
        (1LL << (sizeof(boost::intmax_t) * CHAR_BIT - 1)) + 1;
      static const boost::intmax_t max = -min;

      static char test[X <= max - Y];
  //    static_assert(X <= max - Y, "overflow in ll_add");
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
      static const boost::intmax_t min =
        (1LL << (sizeof(boost::intmax_t) * CHAR_BIT - 1)) + 1;
      static const boost::intmax_t max = -min;

      static char test[min - Y <= X];
  //    static_assert(min - Y <= X, "overflow in ll_add");
  public:
      static const boost::intmax_t value = X + Y;
  };

  template <boost::intmax_t X, boost::intmax_t Y, boost::intmax_t = static_sign<Y>::value>
  class ll_sub;

  template <boost::intmax_t X, boost::intmax_t Y>
  class ll_sub<X, Y, 1>
  {
      static const boost::intmax_t min =
        (1LL << (sizeof(boost::intmax_t) * CHAR_BIT - 1)) + 1;
      static const boost::intmax_t max = -min;

      static char test[min + Y <= X];
  //    static_assert(min + Y <= X, "overflow in ll_sub");
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
      static const boost::intmax_t min =
        (1LL << (sizeof(boost::intmax_t) * CHAR_BIT - 1)) + 1;
      static const boost::intmax_t max = -min;

      static char test[X <= max + Y];
  //    static_assert(X <= max + Y, "overflow in ll_sub");
  public:
      static const boost::intmax_t value = X - Y;
  };

  template <boost::intmax_t X, boost::intmax_t Y>
  class ll_mul
  {
      static const boost::intmax_t nan =
        (1LL << (sizeof(boost::intmax_t) * CHAR_BIT - 1));
      static const boost::intmax_t min = nan + 1;
      static const boost::intmax_t max = -min;
      static const boost::intmax_t a_x = static_abs<X>::value;
      static const boost::intmax_t a_y = static_abs<Y>::value;

      static char test1[X != nan];
      static char test2[Y != nan];
      static char test[a_x <= max / a_y];
  //    static_assert(X != nan && Y != nan && a_x <= max / a_y, "overflow in ll_mul");
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
      static const boost::intmax_t min = nan + 1;
      static const boost::intmax_t max = -min;

      static char test1[X != nan];
      static char test2[Y != nan];
      static char test3[Y != 0];
  //    static_assert(X != nan && Y != nan && Y != 0, "overflow in ll_div");
  public:
      static const boost::intmax_t value = X / Y;
  };

  //template <class T>
  //  struct is_ratio : public false_type {};
  //template <intmax_t N, intmax_t D>
  //  struct is_ratio<ratio<N, D> > : public true_type  {};
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
    static char test1[detail::static_abs<N>::value >= 0];
    static char test2[detail::static_abs<D>::value >  0];
//    static_assert(detail::static_abs<N>::value >= 0, "ratio numerator is out of range");
//    static_assert(D != 0, "ratio divide by 0");
//    static_assert(detail::static_abs<D>::value >  0, "ratio denominator is out of range");
    static const boost::intmax_t m_na = detail::static_abs<N>::value;
    static const boost::intmax_t m_da = detail::static_abs<D>::value;
    static const boost::intmax_t m_s = detail::static_sign<N>::value
      * detail::static_sign<D>::value;
    static const boost::intmax_t m_gcd = detail::static_gcd<m_na, m_da>::value;
public:
    static const boost::intmax_t num = m_s * m_na / m_gcd;
    static const boost::intmax_t den = m_da / m_gcd;

#if 1   
    ratio() {}
        
    template <intmax_t _N2, intmax_t _D2>
    ratio(const ratio<_N2, _D2>&,
        typename enable_if_c
            <
                ratio<_N2, _D2>::num == num &&
                ratio<_N2, _D2>::den == den
            >::type* = 0) {}
    
    template <intmax_t _N2, intmax_t _D2>
        typename enable_if_c
        <
            ratio<_N2, _D2>::num == num &&
            ratio<_N2, _D2>::den == den,
            ratio&
        >::type
    operator=(const ratio<_N2, _D2>&) {return *this;}
#endif

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
private:
#if 1
#else
    static const boost::intmax_t gcd_n1_n2 = detail::static_gcd<R1::num, R2::num>::value;
    static const boost::intmax_t gcd_d1_d2 = detail::static_gcd<R1::den, R2::den>::value;
#endif
public:
    //The nested typedef type shall be a synonym for ratio<T1, T2> where T1 has the value R1::num *
    //R2::den + R2::num * R1::den and T2 has the value R1::den * R2::den.
#if 1
    typedef typename ratio<R1::num * R2::den + R2::num * R1::den,R1::den * R2::den>::type type;
    //typedef ratio<aux_type::num ,aux_type::den > type;
#else
    typedef typename ratio_multiply
       <
           ratio<gcd_n1_n2, R1::den / gcd_d1_d2>,
           ratio
           <
               detail::ll_add
               <
                   detail::ll_mul<R1::num / gcd_n1_n2, R2::den / gcd_d1_d2>::value,
                   detail::ll_mul<R2::num / gcd_n1_n2, R1::den / gcd_d1_d2>::value
               >::value,
               R2::den
           >
       >::type type;
#endif
};

template <class R1, class R2>
struct ratio_subtract
{
private:
#if 1
#else
    static const boost::intmax_t gcd_n1_n2 = detail::static_gcd<R1::num, R2::num>::value;
    static const boost::intmax_t gcd_d1_d2 = detail::static_gcd<R1::den, R2::den>::value;
#endif
public:
//The nested typedef type shall be a synonym for ratio<T1, T2> where T1 has the value
// R1::num *R2::den - R2::num * R1::den and T2 has the value R1::den * R2::den.
#if 1
    typedef typename ratio<R1::num * R2::den - R2::num * R1::den,R1::den * R2::den>::type type;
    //typedef ratio<aux_type::num ,aux_type::den > type;
#else
    typedef typename ratio_multiply
       <
           ratio<gcd_n1_n2, R1::den / gcd_d1_d2>,
           ratio
           <
               detail::ll_sub
               <
                   detail::ll_mul<R1::num / gcd_n1_n2, R2::den / gcd_d1_d2>::value,
                   detail::ll_mul<R2::num / gcd_n1_n2, R1::den / gcd_d1_d2>::value
               >::value,
               R2::den
           >
       >::type type;
#endif
};

template <class R1, class R2>
struct ratio_multiply
{
private:
#if 0    
   static const boost::intmax_t gcd_n1_d2 = detail::static_gcd<R1::num, R2::den>::value;
   static const boost::intmax_t gcd_d1_n2 = detail::static_gcd<R1::den, R2::num>::value;
#endif
public:
#if 0
   typedef ratio
       <
           detail::ll_mul<R1::num / gcd_n1_d2, R2::num / gcd_d1_n2>::value,
           detail::ll_mul<R2::den / gcd_n1_d2, R1::den / gcd_d1_n2>::value
       > type;
#else
   typedef typename ratio<R1::num * R2::num, R1::den * R2::den>::type type;
#endif
};

template <class R1, class R2>
struct ratio_divide
{
private:
#if 0    
   static const boost::intmax_t gcd_n1_n2 = detail::static_gcd<R1::num, R2::num>::value;
   static const boost::intmax_t gcd_d1_d2 = detail::static_gcd<R1::den, R2::den>::value;
#endif
public:
#if 0
   typedef ratio
       <
           detail::ll_mul<R1::num / gcd_n1_n2, R2::den / gcd_d1_d2>::value,
           detail::ll_mul<R2::num / gcd_n1_n2, R1::den / gcd_d1_d2>::value
       > type;
#else
   typedef typename ratio<R1::num * R2::den, R1::den * R2::num>::type type;
#endif
};

// ratio_equal

template <class R1, class R2>
struct ratio_equal
    : public boost::integral_constant<bool,
                               R1::num == R2::num && R1::den == R2::den> {};

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
    typedef ratio<detail::static_gcd<R1::num, R2::num>::value,
                  detail::static_lcm<R1::den, R2::den>::value> type;
  };
}  // namespace detail

//----------------------------------------------------------------------------//
//                                                                            //
//                           more implementation                              //
//                                                                            //
//----------------------------------------------------------------------------//

template <class R1, class R2>
struct ratio_less
  : public boost::integral_constant<bool, detail::ratio_less<R1, R2>::value> {};

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

#endif  // BOOST_RATIO_HPP

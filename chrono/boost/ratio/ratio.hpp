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
                   Daniel Krugler,
                   Anthony Williams.
*/

// The way overflow is managed for ratio_less is taken from llvm/libcxx/include/ratio

#ifndef BOOST_RATIO_RATIO_HPP
#define BOOST_RATIO_RATIO_HPP

#include <boost/config.hpp>
#include <boost/static_integer/static_abs.hpp>
#include <boost/static_integer/static_sign.hpp>
#include <boost/static_integer/static_gcd.hpp>
#include <cstdlib>
#include <climits>
#include <limits>
#include <boost/cstdint.hpp>
#include <boost/type_traits/integral_constant.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/integer_traits.hpp>
#include <boost/ratio/ratio_fwd.hpp>
#include <boost/ratio/detail/overflow_helpers.hpp>

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
//                20.6.1 Class template ratio [ratio.ratio]                   //
//                                                                            //
//----------------------------------------------------------------------------//

template <boost::intmax_t N, boost::intmax_t D>
class ratio
{
    static const boost::intmax_t ABS_N = boost::integer::static_signed_abs<N>::value;
    static const boost::intmax_t ABS_D = boost::integer::static_signed_abs<D>::value;
    BOOST_RATIO_STATIC_ASSERT(ABS_N >= 0, BOOST_RATIO_NUMERATOR_IS_OUT_OF_RANGE, ());
    BOOST_RATIO_STATIC_ASSERT(ABS_D > 0, BOOST_RATIO_DENOMINATOR_IS_OUT_OF_RANGE, ());
    BOOST_RATIO_STATIC_ASSERT(D != 0, BOOST_RATIO_DIVIDE_BY_0 , ());
    static const boost::intmax_t SIGN_N = boost::integer::static_signed_sign<N>::value
      * boost::integer::static_signed_sign<D>::value;
    static const boost::intmax_t GCD = boost::integer::static_signed_gcd<ABS_N, ABS_D>::value;
public:
    BOOST_STATIC_CONSTEXPR boost::intmax_t num = SIGN_N * ABS_N / GCD;
    BOOST_STATIC_CONSTEXPR boost::intmax_t den = ABS_D / GCD;

#ifdef BOOST_RATIO_EXTENSIONS    
    ratio() 
    {}
    template <intmax_t _N2, boost::intmax_t _D2>
    ratio(const ratio<_N2, _D2>&,
        typename enable_if_c
            <
                (ratio<_N2, _D2>::num == num &&
                ratio<_N2, _D2>::den == den)
            >::type* = 0) 
    {}

    template <intmax_t _N2, boost::intmax_t _D2>
        typename enable_if_c
        <
            (ratio<_N2, _D2>::num == num &&
            ratio<_N2, _D2>::den == den),
            ratio&
        >::type
    operator=(const ratio<_N2, _D2>&) {return *this;}
#endif
    typedef ratio<num, den> type;
};

//----------------------------------------------------------------------------//
//                                                                            //
//                20.6.2 Arithmetic on ratio types [ratio.arithmetic]                   //
//                                                                            //
//----------------------------------------------------------------------------//

template <class R1, class R2>
struct ratio_add 
: boost::ratio_detail::ratio_add<R1, R2>::type 
{
};

template <class R1, class R2>
struct ratio_subtract
: boost::ratio_detail::ratio_subtract<R1, R2>::type 
{
};

template <class R1, class R2>
struct ratio_multiply
: boost::ratio_detail::ratio_multiply<R1, R2>::type 
{
};

template <class R1, class R2>
struct ratio_divide
: boost::ratio_detail::ratio_divide<R1, R2>::type 
{
};

//----------------------------------------------------------------------------//
//                                                                            //
//                20.6.3 Comparasion of ratio types [ratio.comparison]                   //
//                                                                            //
//----------------------------------------------------------------------------//

// ratio_equal

template <class R1, class R2>
struct ratio_equal
    : public boost::integral_constant<bool,
                               (R1::num == R2::num && R1::den == R2::den)> 
{};

template <class R1, class R2>
struct ratio_not_equal
    : public boost::integral_constant<bool, !ratio_equal<R1, R2>::value> 
{};

// ratio_less

template <class R1, class R2>
struct ratio_less
    : boost::integral_constant<bool, boost::ratio_detail::ratio_less<R1, R2>::value> 
{};

template <class R1, class R2>
struct ratio_less_equal
    : boost::integral_constant<bool, !ratio_less<R2, R1>::value> 
{};

template <class R1, class R2>
struct ratio_greater
    : boost::integral_constant<bool, ratio_less<R2, R1>::value> 
{};

template <class R1, class R2>
struct ratio_greater_equal
    : boost::integral_constant<bool, !ratio_less<R1, R2>::value> 
{};

template <class R1, class R2>
struct ratio_gcd
{
    typedef ratio<boost::integer::static_signed_gcd<R1::num, R2::num>::value,
        boost::integer::static_signed_lcm<R1::den, R2::den>::value> type;
};
    
}  // namespace boost


#endif  // BOOST_RATIO_HPP

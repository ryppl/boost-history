// Copyright (C) 2005 Peder Holt
// Use, modification and distribution is subject to the Boost Software
// License, Version 1.0. (http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_METAMATH_DOUBLE_INTEGRAL_TO_DOUBLE_HPP
#define BOOST_METAMATH_DOUBLE_INTEGRAL_TO_DOUBLE_HPP

#include <boost/metamath/double/double.hpp>
#include <boost/metamath/double/detail/shift.hpp>
#include <boost/metamath/double/detail/mantissa.hpp>

namespace boost { namespace metamath {
    namespace detail {
        template<typename T,T Number>
        struct integral_exp
        {
            BOOST_STATIC_CONSTANT(int32_t,sign=(Number<0?-1:1));
            BOOST_STATIC_CONSTANT(uint32_t,number=sign*Number);
	        BOOST_STATIC_CONSTANT(int16_t,exp16=(number/65536ul)>=1?16:0);
	        BOOST_STATIC_CONSTANT(int16_t,exp8=(number/((1<<exp16)*256ul))>=1?8:0);
	        BOOST_STATIC_CONSTANT(int16_t,exp4=(number/((1<<(exp16+exp8))*16ul))>=1?4:0);
	        BOOST_STATIC_CONSTANT(int16_t,exp2=(number/((1<<(exp16+exp8+exp4))*4ul))>=1?2:0);
	        BOOST_STATIC_CONSTANT(int16_t,exp1=(number/((1<<(exp16+exp8+exp4+exp2))*2ul))>=1?1:0);
	        BOOST_STATIC_CONSTANT(int16_t,value=exp16+exp8+exp4+exp2+exp1);
        };
    } //namespace detail

    template<typename T,T Value>
    struct integral_c_to_double
    {
        BOOST_STATIC_CONSTANT(bool,sign=Value<0);
        BOOST_STATIC_CONSTANT(T,unsigned_number=(Value*(sign?-1:1)));
        BOOST_STATIC_CONSTANT(int16_t,exponent=(detail::integral_exp<T,unsigned_number>::value));
        BOOST_STATIC_CONSTANT(uint32_t,part1=(detail::shift_integral<uint32_t,unsigned_number,30-exponent>::value));
        BOOST_STATIC_CONSTANT(uint32_t,part2=(detail::shift_integral<uint32_t,unsigned_number,61-exponent>::value));
        BOOST_STATIC_CONSTANT(int16_t,exponent_adjusted=(exponent==0 && part1==0 && part2==0)?-1023:exponent);
        typedef double_<detail::mantissa<part1,part2>,exponent_adjusted,sign> type;
    };

    template<long Value>
    struct long_to_double : integral_c_to_double<long,Value>::type
    {};

    template<typename T>
    struct integral_to_double : integral_c_to_double<typename T::value_type,T::value>::type
    {};

}}//namespace boost::metamath

#endif

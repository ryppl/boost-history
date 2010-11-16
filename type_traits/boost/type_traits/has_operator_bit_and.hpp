// Copyright 2010 Frédéric Bron (frederic.bron@m4x.org)
//
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.

#ifndef BOOST_TT_bit_and_HPP_INCLUDED
#define BOOST_TT_bit_and_HPP_INCLUDED

#define BOOST_TT_TRAIT_NAME has_operator_bit_and
#define BOOST_TT_TRAIT_OP &
#define BOOST_TT_DEFAULT_RET void

#include <boost/type_traits/detail/has_binary_operator.hpp>

namespace boost {

template < typename R > struct BOOST_TT_TRAIT_NAME < bool, float, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < bool, double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < bool, long double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < char, float, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < char, double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < char, long double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < signed char, float, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < signed char, double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < signed char, long double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < short int, float, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < short int, double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < short int, long double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < int, float, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < int, double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < int, long double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long int, float, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long int, double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long int, long double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < unsigned char, float, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < unsigned char, double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < unsigned char, long double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < short unsigned int, float, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < short unsigned int, double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < short unsigned int, long double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < unsigned int, float, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < unsigned int, double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < unsigned int, long double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long unsigned int, float, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long unsigned int, double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long unsigned int, long double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < wchar_t, float, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < wchar_t, double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < wchar_t, long double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < float, bool, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < float, char, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < float, signed char, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < float, short int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < float, int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < float, long int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < float, unsigned char, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < float, short unsigned int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < float, unsigned int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < float, long unsigned int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < float, wchar_t, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < float, float, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < float, double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < float, long double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < double, bool, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < double, char, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < double, signed char, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < double, short int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < double, int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < double, long int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < double, unsigned char, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < double, short unsigned int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < double, unsigned int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < double, long unsigned int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < double, wchar_t, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < double, float, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < double, double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < double, long double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long double, bool, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long double, char, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long double, signed char, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long double, short int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long double, int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long double, long int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long double, unsigned char, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long double, short unsigned int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long double, unsigned int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long double, long unsigned int, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long double, wchar_t, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long double, float, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long double, double, R > : ::boost::integral_constant<bool,false> { };
template < typename R > struct BOOST_TT_TRAIT_NAME < long double, long double, R > : ::boost::integral_constant<bool,false> { };

}

#undef BOOST_TT_TRAIT_NAME
#undef BOOST_TT_TRAIT_OP
#undef BOOST_TT_DEFAULT_RET

#endif

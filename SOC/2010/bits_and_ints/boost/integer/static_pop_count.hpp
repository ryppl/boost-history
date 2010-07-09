//  Boost integer/pop_count.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_STATIC_POP_COUNT
#define BOOST_STATIC_POP_COUNT

#include <boost/mpl/integral_c.hpp>
#include <boost/config/suffix.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/integer/is_integral_constant.hpp>

namespace boost {

/*
 *	Static population count (pop_count).
 *
 *	The static_pop_count<> version accepts one integral value as parameter.
 *  The mpl::pop_count<> version accepts an mpl::integral_c<> as parameter.
 *
 */

template <uintmax_t Value>
struct static_pop_count
{
private:
#ifndef BOOST_NO_INT64_T
	BOOST_STATIC_CONSTANT(uintmax_t, m0 = 0x5555555555555555LLU);
	BOOST_STATIC_CONSTANT(uintmax_t, m1 = 0x3333333333333333LLU);
	BOOST_STATIC_CONSTANT(uintmax_t, m2 = 0x0F0F0F0F0F0F0F0FLLU);
	BOOST_STATIC_CONSTANT(uintmax_t, m3 = 0x00FF00FF00FF00FFLLU);
	BOOST_STATIC_CONSTANT(uintmax_t, m4 = 0x0000FFFF0000FFFFLLU);
	BOOST_STATIC_CONSTANT(uintmax_t, m5 = 0x00000000FFFFFFFFLLU);
#else
	BOOST_STATIC_CONSTANT(uintmax_t, m0 = 0x55555555);
	BOOST_STATIC_CONSTANT(uintmax_t, m1 = 0x33333333);
	BOOST_STATIC_CONSTANT(uintmax_t, m2 = 0x0F0F0F0F);
	BOOST_STATIC_CONSTANT(uintmax_t, m3 = 0x00FF00FF);
	BOOST_STATIC_CONSTANT(uintmax_t, m4 = 0x0000FFFF);
#endif
	BOOST_STATIC_CONSTANT(uintmax_t, v0 = (Value & m0) + ((Value >> 1) & m0));
	BOOST_STATIC_CONSTANT(uintmax_t, v1 = (v0 & m1) + ((v0 >> 2) & m1));
	BOOST_STATIC_CONSTANT(uintmax_t, v2 = (v1 & m2) + ((v1 >> 4) & m2));
	BOOST_STATIC_CONSTANT(uintmax_t, v3 = (v2 & m3) + ((v2 >> 8) & m3));
	BOOST_STATIC_CONSTANT(uintmax_t, v4 = (v3 & m4) + ((v3 >> 16) & m4));
	
public:
#ifndef BOOST_NO_INT64_T
	BOOST_STATIC_CONSTANT(int, value = (v4 & m5) + ((v4 >> 32) & m5));
#else
	BOOST_STATIC_CONSTANT(int, value = v4);
#endif
};
	
namespace mpl {
	
template <typename IC, class Enable = typename enable_if< is_integral_constant<IC> >::type>
struct pop_count : integral_c<int, static_pop_count<IC::value>::value>
{};
	
}
	
}

#endif

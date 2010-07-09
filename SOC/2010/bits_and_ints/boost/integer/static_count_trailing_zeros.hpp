//  Boost integer/static_count_trailing_zeros.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_STATIC_COUNT_TRAILING_ZEROS_INCLUDED
#define BOOST_STATIC_COUNT_TRAILING_ZEROS_INCLUDED

#include <boost/cstdint.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/integer/static_pop_count.hpp>
#include <boost/integer/is_integral_constant.hpp>

/*
 *	static_count_trailing_zeros and mpl::count_trailing_zeros
 *		counts the number of consecutive 0's 
 *		from the least significant bit of integral constants.
 *
 *	For example:
 *	
 *                                         trailing zeros: vvv vvvv
 *	const unsigned value = 0xF080; // 0000 0000 1111 0000 1000 0000
 *  int x = static_count_trailing_zeros<value>::value; // x = 7
 *
 *	typedef mpl::integral_c<unsigned, 0xF080> value2;
 *	int y = mpl::count_trailing_zeros<value2>::value;  // y = 7
 * 
 */

namespace boost {

namespace mpl {
	
/*
 *	MPL integral_c<> compatible version
 */
template <typename IC, 
	class Enable = typename enable_if< is_integral_constant<IC> >::type>
struct count_trailing_zeros : pop_count< integral_c<uintmax_t, ~IC::value & (IC::value - 1)> > 
{};

}

template <uintmax_t Value>
struct static_count_trailing_zeros : 
	mpl::count_trailing_zeros< mpl::integral_c<uintmax_t, Value> >
{};
	
} // boost

#endif

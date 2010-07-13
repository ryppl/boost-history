//  Boost integer/clear_least_bit_set.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_STATIC_CLEAR_LEAST_BIT_SET
#define BOOST_STATIC_CLEAR_LEAST_BIT_SET

#include <boost/static_assert.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/type_traits/is_integral.hpp>
#include <boost/integer/is_integral_constant.hpp>

/*
 *	Clears last 1 bit in `value' in compile-time.
 *	For example:
 *
 *	const int value = 0xFA; // 0000 0000 1111 1010
 *  const int x = static_clear_least_bit_set<value>::value; 
 *	So x will be 0xF8 (0000 0000 1111 1000)
 *
 *	For the MPL compatible:
 *
 *	typedef mpl::integral_c<int, 0xFA> my_value;
 *	const int x = mpl::clear_least_bit_set<my_value>::value;
 *	`x' will be 0xF8 (0000 0000 1111 1000)
 */

namespace boost {

namespace mpl {
	
/*
 *	Requires IC to be a mpl::integral_c<> type
 */
template <typename IC>
struct clear_least_bit_set : 
	integral_c<typename IC::value_type, (IC::value & (IC::value - 1))>
{
	BOOST_STATIC_ASSERT((is_integral_constant<IC>::value));
};
	
}

/*
 *	Requires T to be an integral type
 */
template <typename T, T Value>
struct static_clear_least_bit_set : 
	mpl::clear_least_bit_set< mpl::integral_c<T, Value> >
{
	BOOST_STATIC_ASSERT((is_integral<T>::value));
};

}

#endif

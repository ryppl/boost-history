//  Boost integer/bit_utils.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_BIT_UTILS_INCLUDED
#define BOOST_BIT_UTILS_INCLUDED

#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/bool.hpp>

/*
 *	Some utilities to handle integers.
 */
namespace boost
{
	
template <typename T>
inline T set_bit(T data, unsigned char pos)
{
	return data | (T(1) << pos); 
}

template <typename T>
inline T clear_bit(T data, unsigned char pos)
{
	return data & ~(T(1) << pos); 
}

template <typename T>
inline T flip_bit(T data, unsigned char pos)
{
	return data ^ (T(1) << pos); 
}
	
template <typename T>
inline bool test_bit(T data, unsigned char pos)
{
	return bool((data >> pos) & T(1)); 
}
	
/*** Static metafunctions ***/
	
	
namespace mpl {
	
/*
 *	MPL compatible metafunctions
 */

template <typename IC, unsigned char pos>
struct set_bit : mpl::integral_c<typename IC::value_type, 
	(IC::value | (typename IC::value_type(1) << pos))
>
{};
	
template <typename IC, unsigned char pos>
struct clear_bit : mpl::integral_c<typename IC::value_type, 
	(IC::value & ~(typename IC::value_type(1) << pos))
>
{};

template <typename IC, unsigned char pos>
struct flip_bit : mpl::integral_c<typename IC::value_type, 
	(IC::value ^ (typename IC::value_type(1) << pos))
>
{};

template <typename IC, unsigned char pos>
struct test_bit : mpl::bool_<((IC::value >> pos) & typename IC::value_type(1))>
{};

} // mpl
	
// Sets the bit `pos' in data
template <typename T, T data, unsigned char pos>
struct static_set_bit : mpl::set_bit<mpl::integral_c<T, data>, pos>
{};

// Clear the bit `pos' in data
template <typename T, T data, unsigned char pos>
struct static_clear_bit : mpl::clear_bit<mpl::integral_c<T, data>, pos>
{};

// If the bit `pos' is 1 then it will be 0 if not the bit will be 1
template <typename T, T data, unsigned char pos>
struct static_flip_bit : mpl::flip_bit<mpl::integral_c<T, data>, pos>
{};

// Test if the bit in `pos' positon is set or not
template <typename T, T data, unsigned char pos>
struct static_test_bit : mpl::test_bit<mpl::integral_c<T, data>, pos>
{};
	
} // boost

#endif
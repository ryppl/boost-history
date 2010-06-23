//  Boost integer/swap.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

/*
 *	This function swaps the values of x and y without a temporary variable.
 */

#ifndef BOOST_SWAP_INCLUDED
#define BOOST_SWAP_INCLUDED

namespace boost
{

template <typename T>
inline void swap(T& x, T& y)
{
	x ^= y;
	y ^= x;
	x ^= y;
} // swap
	
} // boost

#endif
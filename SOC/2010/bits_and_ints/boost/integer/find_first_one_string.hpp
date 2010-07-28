//  Boost integer/isqrt.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_FFOS_INCLUDED
#define BOOST_FFOS_INCLUDED

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/math/policies/policy.hpp>
#include <boost/math/policies/error_handling.hpp>
#include <boost/integer/count_leading_zeros.hpp>

namespace boost {
	
/*
 *	This function finds the first occurrence of a string of 
 *		ones in `value` with a given lenght `size`.
 *
 *	`size` must be greater than 0 and less than or equal the
 *		length in bits of type `T`. If these requirements are
 *		no met, one error is raised according to the policy
 *		used.
 *
 *	See the math::policies documentation for more details about
 *		policies.
 *
 *	`T` must be an unsigned type.
 */

template <typename T, typename Policy>
typename enable_if<is_unsigned<T>, int>::type 
find_first_one_string(T value, unsigned size, const Policy& pol)
{
	
	if (size == 0 || size > (sizeof(T) << 3)) {
		return math::policies::raise_domain_error(
			"boost::find_first_one_string(%1%, %1%)",
			"find_first_one_string requires size greater than 0 "
			"and less than or equal the size in bits of value "
			"size=%1% given,",
			size, pol
		);
	}
	
	int shift;
	while (size > 1) {
		shift = size >> 1;
		value &= (value << shift);
		size -= shift;
	}
	
	unsigned ret = count_leading_zeros(value);
	
	if (ret < (sizeof(T) << 3)) return ret;
	
	return -1; // not found
}
	
template <typename T>
typename enable_if<is_unsigned<T>, int>::type 
find_first_one_string(T value, int size)
{
	return find_first_one_string(value, size, math::policies::policy<>());
}
	
}

#endif
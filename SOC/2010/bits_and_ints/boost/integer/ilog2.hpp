//  Boost integer/ilog2.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_ILOG2_INCLUDED
#define BOOST_ILOG2_INCLUDED

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/math/policies/policy.hpp>
#include <boost/math/policies/error_handling.hpp>
#include <boost/integer/count_leading_zeros.hpp>

namespace boost {

/*
 *	This function returns the integer value of 
 *		logarithm in base 2 of an unsigned integral `value`
 *
 *	If `value` is equal to 0, the value returned is -1 and
 *		one error is raised according with the policy used.
 *
 *	See the math::policies documentation for more details.
 *
 *	`T` must be unsigned.
 */
template <typename T, typename Policy>
inline typename enable_if<is_unsigned<T>, int>::type
ilog2(T value, const Policy& pol)
{
	if (value == 0) {
		return math::policies::raise_indeterminate_result_error(
			"boost::ilog2(%1%)",
			"ilog2 is indeterminate for value %1%, returning -1",
			0, -1, pol
		);
	}
	
	return (sizeof(T) << 3) - count_leading_zeros(value) - 1;
}

template <typename T>
inline typename enable_if<is_unsigned<T>, int>::type
ilog2(T value)
{
	return ilog2(value, math::policies::policy<>());
}
	
}

#endif

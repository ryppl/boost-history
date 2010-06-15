//  Boost integer/morton.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_MORTON_INCLUDED
#define BOOST_MORTON_INCLUDED

#include <boost/integer/detail/morton.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/cstdint.hpp>

namespace boost {
/*
 *	The morton<size> types holds an unsigned integer with `size' bytes.
 *	These types are used by interleave functions.
 */
template <std::size_t size, class Enable = typename enable_if_c<size == 8 || size == 16 || size == 32 || size == 64, int>::type>
struct morton
{
	typedef morton<size> type;
	typedef typename integer_detail::morton_type<size>::type value_type;
	static const std::size_t bits = size;
	
	morton<size, Enable>() : value(0) {}
	morton<size, Enable>(const value_type& val) : value(val) {}
	morton<size, Enable>(const morton<size, Enable>& copy) : value(copy.value) {}
	
	value_type value;
};
	
}

#endif

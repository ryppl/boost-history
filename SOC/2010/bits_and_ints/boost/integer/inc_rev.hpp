//  Boost integer/inc_rev.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_INC_REV_INCLUDED
#define BOOST_INC_REV_INCLUDED

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_unsigned.hpp>
#include <boost/integer/bit_reversal.hpp>

namespace boost
{

template <typename T>
inline typename enable_if<is_unsigned<T>, T>::type
inc_rev(T value)
{
	value = bit_reversal(value);
	++value;
	
	return bit_reversal(value);
}

}

#endif

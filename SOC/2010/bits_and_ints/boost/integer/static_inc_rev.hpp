//  Boost integer/inc_rev.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_STATIC_INC_REV_INCLUDED
#define BOOST_STATIC_INC_REV_INCLUDED

#include <boost/mpl/integral_c.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/integer/is_integral_constant.hpp>
#include <boost/integer/static_bit_reversal.hpp>

namespace boost
{

namespace mpl {

template <typename IC, class Enable = 
	typename enable_if< is_unsigned_constant<IC> >::type>
struct inc_rev : integral_c<typename IC::value_type, 
	bit_reversal< 
		typename next< bit_reversal<IC> >::type
	>
>
{};
	
}
	
template <typename T, T Value, class Enable = typename is_unsigned<T>::type>
struct static_inc_rev : mpl::inc_rev< mpl::integral_c<T, Value> >
{};
	
}

#endif

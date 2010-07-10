//  Boost integer/detail/pop_count.hpp header file  ------------------------------//

//  (C) Copyright Murilo Adriano Vasconcelos 2010.
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt

//  See http://www.boost.org for updates, documentation, and revision history. 

#ifndef BOOST_DETAIL_POPCOUNT_INCLUDED
#define BOOST_DETAIL_POPCOUNT_INCLUDED

namespace boost { 

namespace integer_detail {

static const uintmax_t pop_count_mask[6] = {
#ifndef BOOST_NO_INT64_T
	0x5555555555555555ULL, 0x3333333333333333ULL, 0x0F0F0F0F0F0F0F0FULL, 
	0x00FF00FF00FF00FFULL, 0x0000FFFF0000FFFFULL, 0x00000000FFFFFFFFULL
#else
	0x55555555, 0x33333333, 0x0F0F0F0F, 0x00FF00FF, 0x0000FFFF, 0x0
#endif
};
	
} 

} // boost

#endif

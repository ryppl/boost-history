//  Boost string_algo library string_traits.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_TRAITS_HPP
#define BOOST_STRING_TRAITS_HPP

#include "string_funct.hpp"

namespace boost {

	namespace string_algo{

		// Search element compare traits
		template< typename T1, typename T2 >
		struct compare_traits
		{
			// Element comparison functor
			typedef typename detail::equal_toF< T1, T2 > compare_type;
		};

		// Substring search traits ( generic )
		template< typename InputIterator, typename SubstrIterator >
		struct search_traits
		{
			// Element comparison functor
			typedef typename compare_traits<
				typename InputIterator::value_type,
				typename SubstrIterator::value_type>::compare_type compare_function_type;
			
			// Range type
			typedef std::pair< InputIterator, InputIterator> range_type;
		};

	} // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_TRAITS_HPP

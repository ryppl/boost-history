//  Boost string_algo library container_traits.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_CONTAINER_TRAITS_HPP
#define BOOST_STRING_CONTAINER_TRAITS_HPP

#include "detail/container.hpp"
#include "detail/container_basic_string.hpp"

namespace boost {

    namespace string_algo{

//  contatiner caps traits  -----------------------------------------------//

        template< typename Container >
        struct container_caps_traits
        {
            // Input types
            typedef Container type;
            typedef typename type::iterator iterator_type;
            typedef typename type::const_iterator const_iterator_type;
            typedef type& reference_type;
            typedef const type& const_reference_type;

            // Operations
			
			// replace range
			typedef detail::replaceF<type> replace_type;        
			// insert range
            typedef detail::insertF<type> insert_type; 
			// erase range                
            typedef detail::eraseF<type> erase_type;       

			// iterative replace 
			// ( allows a replacement of multiple occurences in the same container )
			typedef detail::replace_iterF<type> replace_iter_type;
        };		

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_CONTAINER_TRAITS_HPP

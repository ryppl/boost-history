//  Boost string_algo library traits.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_TRAITS_HPP
#define BOOST_STRING_TRAITS_HPP

namespace boost {

    namespace string_algo {

//  input policy  -----------------------------------------------//
        
        // default input policy for mutable input
        struct mutable_input_policy
        {
            template<typename InputT>
            struct input_policy
            {   
                typedef InputT input_type;
                typedef typename InputT::iterator iterator_type;
                typedef input_type& reference_type;
            };
        };

        // input policy for const input
        struct const_input_policy
        {
            template<typename InputT>
            struct input_policy
            {   
                typedef InputT input_type;
                typedef typename InputT::const_iterator iterator_type;
                typedef const input_type& reference_type;
            };
        };

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_TRAITS_HPP

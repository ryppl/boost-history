//  Boost string_algo library input_policy.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_INPUT_POLICY_HPP
#define BOOST_STRING_INPUT_POLICY_HPP

#include <boost/string_algo/config.hpp>
#include <boost/mpl/if.hpp>
#include <boost/type_traits/is_const.hpp>
#include <boost/string_algo/iterator_range.hpp>

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
                typedef BOOST_STRING_DEDUCED_TYPENAME InputT::iterator iterator_type;
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
                typedef BOOST_STRING_DEDUCED_TYPENAME InputT::const_iterator iterator_type;
                typedef const input_type& reference_type;
            };
        };

//  input policy selector  -----------------------------------------------//

        template< typename InputT >
        struct input_policy
        {
            typedef BOOST_STRING_DEDUCED_TYPENAME mpl::if_<  
                is_const<InputT>,
                const_input_policy,
                mutable_input_policy >::type policy;

        private:
            typedef BOOST_STRING_DEDUCED_TYPENAME policy::
                template input_policy<InputT> input_policy_type;

        public:
            typedef InputT input_type;
            typedef BOOST_STRING_DEDUCED_TYPENAME 
                input_policy_type::iterator_type iterator_type;
            typedef BOOST_STRING_DEDUCED_TYPENAME 
                input_policy_type::reference_type reference_type;
        };

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_INPUT_POLICY_HPP

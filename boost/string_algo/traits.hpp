//  Boost string_algo library traits.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_TRAITS_HPP
#define BOOST_STRING_TRAITS_HPP

#include "iterator_range.hpp"

namespace boost {

    namespace string_algo {

//  search traits  -----------------------------------------------//

        // Substring search traits ( contatiner based )
        template< typename InputT, typename SearchT >
        struct search_traits
        {
            // Input types
            typedef InputT input_type;
            typedef input_type& input_reference_type;
            typedef const input_type& input_const_reference_type;
            typedef typename input_type::iterator input_iterator_type;
            typedef typename input_type::const_iterator input_const_iterator_type;

            typedef iterator_range< input_iterator_type > range_type;
            typedef iterator_range< input_const_iterator_type > const_range_type;

            // Search types
            typedef SearchT search_type;
            typedef search_type& search_reference_type;
            typedef const search_type& search_const_reference_type;
            typedef typename search_type::iterator search_iterator_type;
            typedef typename search_type::const_iterator search_const_iterator_type;
        };


        // Substring search traits ( generic )
        template< typename InputIteratorT, typename SearchIteratorT > 
        struct iter_search_traits : 
            public search_traits< 
                iterator_range<InputIteratorT>,
                iterator_range<SearchIteratorT> >
        {
            // Base type
            typedef search_traits< 
                iterator_range<InputIteratorT>,
                iterator_range<SearchIteratorT> > base_type;

            // Input types
            typedef typename base_type::input_type input_type;
            typedef typename base_type::input_reference_type input_reference_type;
            typedef typename base_type::input_const_reference_type input_const_reference_type;
            typedef typename base_type::input_iterator_type input_iterator_type;
            typedef typename base_type::input_const_iterator_type input_const_iterator_type;

            typedef typename base_type::range_type range_type;
            typedef typename base_type::const_range_type const_range_type;

            // Search types
            typedef typename base_type::search_type search_type;
            typedef typename base_type::search_reference_type search_reference_type;
            typedef typename base_type::search_const_reference_type search_const_reference_type ;
            typedef typename base_type::search_iterator_type search_iterator_type;
            typedef typename base_type::search_const_iterator_type search_const_iterator_type;
        };

//  search policy  -----------------------------------------------//
        
        // default search policy for mutable input
        struct search_policy
        {
            template<typename SearchTraits>
            struct input_policy
            {   
                typedef typename SearchTraits::input_type input_type;
                typedef typename SearchTraits::input_iterator_type input_iterator_type;
                typedef typename SearchTraits::input_reference_type input_reference_type;
                typedef typename SearchTraits::range_type range_type;
            };
        };

        // search policy for const input
        struct const_input_search_policy
        {
            template<typename SearchTraits>
            struct input_policy
            {   
                typedef typename SearchTraits::input_type input_type;
                typedef typename SearchTraits::input_const_iterator_type input_iterator_type;
                typedef typename SearchTraits::input_const_reference_type input_reference_type;
                typedef typename SearchTraits::const_range_type range_type;
            };
        };

//  replate traits  -----------------------------------------------//

        template< typename InputT,  typename ReplaceT >
        struct replace_traits
        {
            // Input types
            typedef InputT input_type;
            typedef typename input_type::iterator input_iterator_type;
            typedef typename input_type::const_iterator input_const_iterator_type;
            typedef input_type& input_reference_type;
            typedef const input_type& input_const_reference_type;

            // Replace type
            typedef ReplaceT replace_type;
            typedef typename replace_type::const_iterator replace_iterator_type;
            typedef const replace_type& replace_reference_type;
        };

        template< typename InputT, typename FormatFT >
        struct replace_format_traits : 
            public replace_traits<InputT, typename FormatFT::result_type >
        {
            typedef replace_traits<
                InputT, 
                typename FormatFT::result_type > base_type;

            typedef typename base_type::input_type input_type;
            typedef typename base_type::input_iterator_type input_iterator_type;
            typedef typename base_type::input_const_iterator_type input_const_iterator_type;
            typedef typename base_type::input_reference_type input_reference_type;
            typedef typename base_type::input_const_reference_type input_const_reference_type;

            // Replace type
            typedef typename base_type::replace_type replace_type;
            typedef typename base_type::replace_iterator_type replace_iterator_type;
            typedef typename base_type::replace_reference_type replace_reference_type;
        };

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_TRAITS_HPP

//  Boost string_algo library replace.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_HPP
#define BOOST_STRING_REPLACE_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/replace_impl.hpp>
#include <boost/string_algo/find_impl.hpp>

namespace boost {

//  replace_range --------------------------------------------------------------------//

    // replace_range output iterator version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename FormatT >
    inline OutputIteratorT replace_range_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const iterator_range<
            BOOST_STRING_TYPENAME 
                string_algo::container_traits<InputT>::const_iterator>& SearchRange,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::range_finder( SearchRange ),
            string_algo::identity_formater( Format ) );
    }

    // replace_range sequence version
    template< typename InputT, typename FormatT >
    inline InputT replace_range_copy( 
        const InputT& Input,
        const iterator_range<
            BOOST_STRING_TYPENAME 
                string_algo::container_traits<InputT>::const_iterator>& SearchRange,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Input,
            string_algo::range_finder( SearchRange ),
            string_algo::identity_formater( Format ) );
    }

    // replace_range in-place sequence version
    template< typename InputT, typename FormatT >
    inline InputT& replace_range( 
        InputT& Input,
        const iterator_range<
            BOOST_STRING_TYPENAME 
                string_algo::container_traits<InputT>::iterator>& SearchRange,
        const FormatT& Format )
    {
        return string_algo::replace(
            Input,
            string_algo::range_finder( SearchRange ),
            string_algo::identity_formater( Format ) );
    }

//  replace_first --------------------------------------------------------------------//

    // replace_first output iterator version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT,
        typename FormatT >
    inline OutputIteratorT replace_first_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::first_finder( Search ),
            string_algo::identity_formater( Format ) );
    }

    // replace_first sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT replace_first_copy( 
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::first_finder( Search ),
            string_algo::identity_formater( Format ) );
    }

    // replace_first in-place sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT& replace_first( 
        InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::first_finder( Search ),
            string_algo::identity_formater( Format ) );
    }

//  replace_last --------------------------------------------------------------------//

    // replace_last output iterator version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT,
        typename FormatT >
    inline OutputIteratorT replace_last_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::last_finder( Search ),
            string_algo::identity_formater( Format ) );
    }

    // replace_last sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT replace_last_copy( 
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::last_finder( Search ),
            string_algo::identity_formater( Format ) );
    }

    // replace_last in-place sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT& replace_last( 
        InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::last_finder( Search ),
            string_algo::identity_formater( Format ) );
    }

//  replace_nth --------------------------------------------------------------------//

    // replace_nth output iterator version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT,
        typename FormatT >
    inline OutputIteratorT replace_nth_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::nth_finder( Search, Nth ),
            string_algo::identity_formater( Format ) );
    }

    // replace_nth sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT replace_nth_copy( 
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth,
        const FormatT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::nth_finder( Search, Nth ),
            string_algo::identity_formater( Format ) );
    }

    // replace_nth in-place sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT& replace_nth( 
        InputT& Input,
        const SearchT& Search,
        unsigned int Nth,
        const FormatT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::nth_finder( Search, Nth ),
            string_algo::identity_formater( Format ) );
    }

//  replace_all --------------------------------------------------------------------//

    // replace_all output iterator version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT,
        typename FormatT >
    inline OutputIteratorT replace_all_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_all_copy(
            Output,
            Input,
            string_algo::first_finder( Search ),
            string_algo::identity_formater( Format ) );
    }

    // replace_all sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT replace_all_copy( 
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_all_copy( 
            Input,
            string_algo::first_finder( Search ),
            string_algo::identity_formater( Format ) );
    }

    // replace_all in-place sequence version
    template< typename InputT, typename SearchT, typename FormatT >
    inline InputT& replace_all( 
        InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_all( 
            Input, 
            string_algo::first_finder( Search ),
            string_algo::identity_formater( Format ) );
    }
    

//  replace_head --------------------------------------------------------------------//

    // replace_head output iterator version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename FormatT >
    inline OutputIteratorT replace_head_copy(
        OutputIteratorT Output,
        const InputT& Input,
        unsigned int N,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::head_finder(N),
            string_algo::identity_formater( Format ) );
    }

    // replace_head sequence version
    template< typename InputT, typename FormatT >
    inline InputT replace_head_copy( 
        const InputT& Input,
        unsigned int N,
        const FormatT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::head_finder(N),
            string_algo::identity_formater( Format ) );
    }

    // replace_head in-place sequence version
    template< typename InputT, typename FormatT >
    inline InputT& replace_head( 
        InputT& Input,
        unsigned int N,
        const FormatT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::head_finder(N),
            string_algo::identity_formater( Format ) );
    }

//  replace_tail --------------------------------------------------------------------//

    // replace_tail output iterator version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename FormatT >
    inline OutputIteratorT replace_tail_copy(
        OutputIteratorT Output,
        const InputT& Input,
        unsigned int N,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::tail_finder(N),
            string_algo::identity_formater( Format ) );
    }

    // replace_tail sequence version
    template< typename InputT, typename FormatT >
    inline InputT replace_tail_copy( 
        const InputT& Input,
        unsigned int N,
        const FormatT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::tail_finder(N),
            string_algo::identity_formater( Format ) );
    }

    // replace_tail in-place sequence version
    template< typename InputT, typename FormatT >
    inline InputT& replace_tail( 
        InputT& Input,
        unsigned int N,
        const FormatT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::tail_finder(N),
            string_algo::identity_formater( Format ) );
    }

} // namespace boost


#endif  // BOOST_REPLACE_HPP

//  Boost string_algo library erase.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_ERASE_HPP
#define BOOST_STRING_ERASE_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/replace2.hpp>
#include <boost/string_algo/find2.hpp>
#include <boost/string_algo/format2.hpp>

namespace boost {

//  erase_range -------------------------------------------------------//

    // erase_range iterator-output version
    template< typename OutputIteratorT, typename InputT >
    inline OutputIteratorT erase_range_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const iterator_range<
            BOOST_STRING_TYPENAME 
                string_algo::container_traits<InputT>::const_iterator>& SearchRange )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::range_finder(SearchRange),
            string_algo::empty_formatter(Input) );
    }

    // erase_range sequence version
    template< typename InputT >
    inline InputT erase_range_copy( 
        const InputT& Input,
        const iterator_range<
            BOOST_STRING_TYPENAME 
                string_algo::container_traits<InputT>::const_iterator>& SearchRange )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::range_finder(SearchRange),
            string_algo::empty_formatter(Input) );
    }

    // replace_first in-place sequence version
    template< typename InputT >
    inline InputT& erase_range( 
        InputT& Input,
        const iterator_range<
            BOOST_STRING_TYPENAME 
                string_algo::container_traits<InputT>::iterator>& SearchRange )
    {
        return string_algo::replace( 
            Input, 
            string_algo::range_finder(SearchRange),
            string_algo::empty_formatter(Input) );
    }

//  erase_first  --------------------------------------------------------//

    // erase_first iterator-output version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT erase_first_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::first_finder(Search),
            string_algo::empty_formatter(Input) );
    }

    // erase_first sequence version
    template< typename InputT, typename SearchT >
    inline InputT erase_first_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::first_finder(Search),
            string_algo::empty_formatter(Input) );
    }

    // erase_first in-place sequence version
    template< typename InputT, typename SearchT >
    inline InputT& erase_first( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace( 
            Input, 
            string_algo::first_finder(Search),
            string_algo::empty_formatter(Input) );
    }

//  erase_first ( case insensitive ) ------------------------------------//

    // erase_first iterator-output version ( case insensitive )
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT ierase_first_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::first_finder(Search, string_algo::is_iequal()),
            string_algo::empty_formatter(Input) );
    }

    // erase_first sequence version ( case insensitive )
    template< typename InputT, typename SearchT >
    inline InputT ierase_first_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::first_finder(Search, string_algo::is_iequal()),
            string_algo::empty_formatter(Input) );
    }

    // erase_first in-place sequence version ( case insensitive )
    template< typename InputT, typename SearchT >
    inline InputT& ierase_first( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace( 
            Input, 
            string_algo::first_finder(Search, string_algo::is_iequal()),
            string_algo::empty_formatter(Input) );
    }

//  erase_last  --------------------------------------------------------//

    // erase_last iterator-output version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT erase_last_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::last_finder(Search),
            string_algo::empty_formatter(Input) );
    }

    // erase_last sequence version
    template< typename InputT, typename SearchT >
    inline InputT erase_last_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::last_finder(Search),
            string_algo::empty_formatter(Input) );
    }

    // erase_last in-place sequence version
    template< typename InputT, typename SearchT >
    inline InputT& erase_last( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace( 
            Input, 
            string_algo::last_finder(Search),
            string_algo::empty_formatter(Input) );
    }

//  erase_last ( case insensitive ) ------------------------------------//

    // erase_last iterator-output version ( case insensitive )
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT ierase_last_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::last_finder(Search, string_algo::is_iequal()),
            string_algo::empty_formatter(Input) );
    }

    // erase_last sequence version ( case insensitive )
    template< typename InputT, typename SearchT >
    inline InputT ierase_last_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::last_finder(Search, string_algo::is_iequal()),
            string_algo::empty_formatter(Input) );
    }

    // erase_last in-place sequence version ( case insensitive )
    template< typename InputT, typename SearchT >
    inline InputT& ierase_last( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace( 
            Input, 
            string_algo::last_finder(Search, string_algo::is_iequal()),
            string_algo::empty_formatter(Input) );
    }

//  erase_nth --------------------------------------------------------------------//

    // erase_nth iterator-output version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT erase_nth_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::nth_finder(Search, Nth),
            string_algo::empty_formatter(Input) );
    }

    // erase_nth sequence version
    template< typename InputT, typename SearchT >
    inline InputT erase_nth_copy( 
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::nth_finder(Search, Nth),
            string_algo::empty_formatter(Input) );
    }

    // erase_nth in-place sequence version
    template< typename InputT, typename SearchT >
    inline InputT& erase_nth( 
        InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::replace( 
            Input, 
            string_algo::nth_finder(Search, Nth),
            string_algo::empty_formatter(Input) );
    }

//  erase_nth ( case insensitive ) ---------------------------------------------//

    // erase_nth iterator-output version ( case insensitive )
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT ierase_nth_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::nth_finder(Search, Nth, string_algo::is_iequal()),
            string_algo::empty_formatter(Input) );
    }

    // erase_nth sequence version ( case insensitive )
    template< typename InputT, typename SearchT >
    inline InputT ierase_nth_copy( 
        const InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::replace_copy( 
            Input, 
            string_algo::nth_finder(Search, Nth, string_algo::is_iequal()),
            string_algo::empty_formatter(Input) );
    }

    // erase_nth in-place sequence version ( case insensitive )
    template< typename InputT, typename SearchT >
    inline InputT& ierase_nth( 
        InputT& Input,
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::replace( 
            Input, 
            string_algo::nth_finder(Search, Nth, string_algo::is_iequal()),
            string_algo::empty_formatter(Input) );
    }


//  erase_all  --------------------------------------------------------//

    // erase_all iterator-output version
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT erase_all_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_all_copy(
            Output,
            Input,
            string_algo::first_finder(Search),
            string_algo::empty_formatter(Input) );
    }

    // erase_all sequence version
    template< typename InputT, typename SearchT >
    inline InputT erase_all_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_all_copy( 
            Input, 
            string_algo::first_finder(Search),
            string_algo::empty_formatter(Input) );
    }

    // erase_all in-place sequence version
    template< typename InputT, typename SearchT >
    inline InputT& erase_all( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_all( 
            Input, 
            string_algo::first_finder(Search),
            string_algo::empty_formatter(Input) );
    }

//  erase_all ( case insensitive ) ------------------------------------//

    // erase_all iterator-output version ( case insensitive )
    template< 
        typename OutputIteratorT,
        typename InputT, 
        typename SearchT >
    inline OutputIteratorT ierase_all_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_all_copy(
            Output,
            Input,
            string_algo::first_finder(Search, string_algo::is_iequal()),
            string_algo::empty_formatter(Input) );
    }

    // erase_all sequence version ( case insensitive ) 
    template< typename InputT, typename SearchT >
    inline InputT ierase_all_copy( 
        const InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_all_copy( 
            Input, 
            string_algo::first_finder(Search, string_algo::is_iequal()),
            string_algo::empty_formatter(Input) );
    }

    // erase_all in-place sequence version ( case insensitive )
    template< typename InputT, typename SearchT >
    inline InputT& ierase_all( 
        InputT& Input,
        const SearchT& Search )
    {
        return string_algo::replace_all( 
            Input, 
            string_algo::first_finder(Search, string_algo::is_iequal()),
            string_algo::empty_formatter(Input) );
    }

//  erase_head --------------------------------------------------------------------//

    // erase_head iterator-output version
    template< 
        typename OutputIteratorT,
        typename InputT >
    inline OutputIteratorT erase_head_copy(
        OutputIteratorT Output,
        const InputT& Input,
        unsigned int N )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::head_finder(N),
            string_algo::empty_formatter( Input ) );
    }

    // erase_head sequence version
    template< typename InputT >
    inline InputT erase_head_copy( 
        const InputT& Input,
        unsigned int N )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::head_finder(N),
            string_algo::empty_formatter( Input ) );
    }

    // erase_head in-place sequence version
    template< typename InputT >
    inline InputT& erase_head( 
        InputT& Input,
        unsigned int N )
    {
        return string_algo::replace( 
            Input, 
            string_algo::head_finder(N),
            string_algo::empty_formatter( Input ) );
    }

//  erase_tail --------------------------------------------------------------------//

    // erase_tail iterator-output version
    template< 
        typename OutputIteratorT,
        typename InputT >
    inline OutputIteratorT erase_tail_copy(
        OutputIteratorT Output,
        const InputT& Input,
        unsigned int N )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::tail_finder(N),
            string_algo::empty_formatter( Input ) );
    }

    // erase_tail sequence version
    template< typename InputT >
    inline InputT erase_tail_copy( 
        const InputT& Input,
        unsigned int N )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::tail_finder(N),
            string_algo::empty_formatter( Input ) );
    }

    // erase_tail in-place sequence version
    template< typename InputT >
    inline InputT& erase_tail( 
        InputT& Input,
        unsigned int N )
    {
        return string_algo::replace( 
            Input, 
            string_algo::tail_finder(N),
            string_algo::empty_formatter( Input ) );
    }
    
} // namespace boost


#endif  // BOOST_ERASE_HPP

//  Boost string_algo library find.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_HPP
#define BOOST_STRING_FIND_HPP

#include <boost/string_algo/traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/find.hpp>
#include <boost/string_algo/find_impl.hpp>

namespace boost {

//  find_first  -----------------------------------------------//

    // find the first match a subsequnce in the sequence
    template< typename InputIteratorT, typename SearchIteratorT >
    inline iterator_range<InputIteratorT>
    find_first( 
        InputIteratorT Begin, 
        InputIteratorT End, 
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd )
    {
        return string_algo::find( 
            string_algo::make_range( Begin, End ),
            string_algo::detail::
                create_find_firstF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ) ) );
    }

    // find_first sequence version
    template< typename InputT, typename SearchT >
	inline iterator_range< typename string_algo::input_policy<InputT>::iterator_type >
    find_first( 
		InputT& Input, 
        const SearchT& Search )
    {
        return string_algo::find( 
            Input, 
            string_algo::detail::find_firstF<
				InputT, 
				SearchT, 
				typename string_algo::input_policy<InputT>::policy>( Search ) );
    }

//  find_last  -----------------------------------------------//

    // find the last match a subsequnce in the sequence
    template< typename InputIteratorT, typename SearchIteratorT >
    inline iterator_range<InputIteratorT>
    find_last( 
        InputIteratorT Begin, 
        InputIteratorT End, 
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd )
    {
        return string_algo::find( 
            string_algo::make_range( Begin, End ),
            string_algo::detail::
                create_find_lastF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ) ) );
    }

    // find_last sequence version
    template< typename InputT, typename SearchT >
	inline iterator_range< typename string_algo::input_policy<InputT>::iterator_type >
    find_last( 
		InputT& Input, 
        const SearchT& Search )
    {
        return string_algo::find( 
            Input, 
            string_algo::detail::find_lastF<
				InputT, 
				SearchT, 
				typename string_algo::input_policy<InputT>::policy>( Search ) );
    }

//  find_nth ----------------------------------------------------------------------//

    // find the n-th match of a subsequnce in the sequence
    template< typename InputIteratorT, typename SearchIteratorT >
    inline iterator_range<InputIteratorT>
    find_nth( 
        InputIteratorT Begin, 
        InputIteratorT End, 
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        unsigned int Nth )
    {
        return string_algo::find( 
            string_algo::make_range( Begin, End ),
            string_algo::detail::
                create_find_nthF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ), Nth ) );
    }

    template< typename InputT, typename SearchT >
	inline iterator_range< typename string_algo::input_policy<InputT>::iterator_type >
    find_nth( 
		InputT& Input, 
        const SearchT& Search,
        unsigned int Nth )
    {
        return string_algo::find( 
            Input, 
            string_algo::detail::find_nthF<
				InputT, 
				SearchT, 
				typename string_algo::input_policy<InputT>::policy>( Search, Nth ) );
    }

} // namespace boost


#endif  // BOOST_STRING_FIND_HPP

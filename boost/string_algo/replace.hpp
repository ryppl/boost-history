//  Boost string_algo library replace.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_HPP
#define BOOST_STRING_REPLACE_HPP

#include <boost/string_algo/traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/replace_impl.hpp>
#include <boost/string_algo/detail/find.hpp>
#include <boost/string_algo/detail/format.hpp>

namespace boost {

//  replace_range --------------------------------------------------------------------//

    // replace_range iterator version
    template< 
        typename InputIteratorT, 
        typename FormatIteratorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_range_copy(
        OutputIteratorT Output,
        InputIteratorT Begin,
        InputIteratorT End,
        InputIteratorT SearchBegin,
        InputIteratorT SearchEnd,
        FormatIteratorT FormatBegin,
        FormatIteratorT FormatEnd )
    {
        return string_algo::replace_copy(
            Output,
            string_algo::make_range( Begin, End ), 
            string_algo::detail::
                create_find_rangeF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF< iterator_range<FormatIteratorT> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    // replace_range output iterator version
    template< 
        typename InputT, 
        typename FormatT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_range_copy(
        OutputIteratorT Output,
        const InputT& Input,
        typename InputT::const_iterator SearchBegin,
        typename InputT::const_iterator SearchEnd,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_rangeF<InputT>::create_const( 
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

    // replace_range sequence version
    template< typename InputT, typename FormatT >
    inline InputT replace_range_copy( 
        const InputT& Input,
        typename InputT::const_iterator SearchBegin,
        typename InputT::const_iterator SearchEnd,
        const FormatT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::detail::
                create_find_rangeF<InputT>::create_const(
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

    // replace_range in-place sequence version
    template< typename InputT, typename FormatT >
    inline InputT& replace_range( 
        InputT& Input,
        typename InputT::iterator SearchBegin,
        typename InputT::iterator SearchEnd,
        const FormatT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::detail::
                create_find_rangeF<InputT>::create(
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

//  replace_first --------------------------------------------------------------------//

    // replace_first iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename FormatIteratorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_first_copy(
        OutputIteratorT Output,
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        FormatIteratorT FormatBegin,
        FormatIteratorT FormatEnd )
    {
        return string_algo::replace_copy(
            Output,
            string_algo::make_range( Begin, End ), 
            string_algo::detail::
                create_find_firstF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF< iterator_range<FormatIteratorT> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    // replace_first output iterator version
    template< 
        typename InputT, 
        typename SearchT,
        typename FormatT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_first_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_firstF< InputT >::create_const( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_firstF< InputT >::create_const( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_firstF< InputT >::create( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

//  replace_last --------------------------------------------------------------------//

    // replace_last iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename FormatIteratorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_last_copy(
        OutputIteratorT Output,
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        FormatIteratorT FormatBegin,
        FormatIteratorT FormatEnd )
    {
        return string_algo::replace_copy(
            Output,
            string_algo::make_range( Begin, End ), 
            string_algo::detail::
                create_find_lastF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF< iterator_range<FormatIteratorT> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    // replace_last output iterator version
    template< 
        typename InputT, 
        typename SearchT,
        typename FormatT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_last_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_lastF< InputT >::create_const( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_lastF< InputT >::create_const( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_lastF< InputT >::create( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

//  replace_nth --------------------------------------------------------------------//

    // replace_nth iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename FormatIteratorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_nth_copy(
        OutputIteratorT Output,
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        unsigned int Nth,
        FormatIteratorT FormatBegin,
        FormatIteratorT FormatEnd )
    {
        return string_algo::replace_copy(
            Output,
            string_algo::make_range( Begin, End ), 
            string_algo::detail::
                create_find_nthF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ), Nth ),
            string_algo::detail::
                identity_formatF< iterator_range<FormatIteratorT> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    template< 
        typename InputT, 
        typename SearchT,
        typename FormatT,
        typename OutputIteratorT >
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
            string_algo::detail::
                create_find_nthF< InputT >::create_const( Search, Nth ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_nthF< InputT >::create_const( Search, Nth ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_nthF< InputT >::create( Search, Nth ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }


//  replace_all --------------------------------------------------------------------//

    // replace_all iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename FormatIteratorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_all_copy(
        OutputIteratorT Output,
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        FormatIteratorT FormatBegin,
        FormatIteratorT FormatEnd )
    {
        return string_algo::replace_all_copy(
            Output,
            string_algo::make_range( Begin, End ), 
            string_algo::detail::
                create_find_firstF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF< iterator_range<FormatIteratorT> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    // replace_all iterator version
    template< 
        typename InputT, 
        typename SearchT,
        typename FormatT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_all_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_all_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_firstF< InputT >::create_const( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_firstF< InputT >::create_const( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_firstF< InputT >::create( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }
    

} // namespace boost


#endif  // BOOST_REPLACE_HPP
//  Boost string_algo library replace.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_HPP
#define BOOST_STRING_REPLACE_HPP

#include <boost/string_algo/traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/replace_impl.hpp>
#include <boost/string_algo/detail/find.hpp>
#include <boost/string_algo/detail/format.hpp>

namespace boost {

//  replace_range --------------------------------------------------------------------//

    // replace_range iterator version
    template< 
        typename InputIteratorT, 
        typename FormatIteratorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_range_copy(
        OutputIteratorT Output,
        InputIteratorT Begin,
        InputIteratorT End,
        InputIteratorT SearchBegin,
        InputIteratorT SearchEnd,
        FormatIteratorT FormatBegin,
        FormatIteratorT FormatEnd )
    {
        return string_algo::replace_copy(
            Output,
            string_algo::make_range( Begin, End ), 
            string_algo::detail::
                create_find_rangeF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF< iterator_range<FormatIteratorT> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    // replace_range iterator version
    template< 
        typename InputT, 
        typename FormatT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_range_copy(
        OutputIteratorT Output,
        const InputT& Input,
        typename InputT::const_iterator SearchBegin,
        typename InputT::const_iterator SearchEnd,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_rangeF< InputT >::create_const(
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

    // replace_range sequence version
    template< typename InputT, typename FormatT >
    inline InputT replace_range_copy( 
        const InputT& Input,
        typename InputT::const_iterator SearchBegin,
        typename InputT::const_iterator SearchEnd,
        const FormatT& Format )
    {
        return string_algo::replace_copy( 
            Input,
            string_algo::detail::
                create_find_rangeF< InputT >::create_const(
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

    // replace_range in-place sequence version
    template< typename InputT, typename FormatT >
    inline InputT& replace_range( 
        InputT& Input,
        typename InputT::iterator SearchBegin,
        typename InputT::iterator SearchEnd,
        const FormatT& Format )
    {
        return string_algo::replace( 
            Input, 
            string_algo::detail::
                create_find_rangeF< InputT >::create(
                    string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

//  replace_first --------------------------------------------------------------------//

    // replace_first iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename FormatIteratorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_first_copy(
        OutputIteratorT Output,
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        FormatIteratorT FormatBegin,
        FormatIteratorT FormatEnd )
    {
        return string_algo::replace_copy(
            Output,
            string_algo::make_range( Begin, End ), 
            string_algo::detail::
                create_find_firstF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF< iterator_range<FormatIteratorT> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    // replace_first iterator version
    template< 
        typename InputT, 
        typename SearchT,
        typename FormatT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_first_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_firstF< InputT >::create_const( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_firstF< InputT >::create_const( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_firstF< InputT >::create( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

//  replace_last --------------------------------------------------------------------//

    // replace_last iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename FormatIteratorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_last_copy(
        OutputIteratorT Output,
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        FormatIteratorT FormatBegin,
        FormatIteratorT FormatEnd )
    {
        return string_algo::replace_copy(
            Output,
            string_algo::make_range( Begin, End ), 
            string_algo::detail::
                create_find_lastF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF< iterator_range<FormatIteratorT> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    // replace_last iterator version
    template< 
        typename InputT, 
        typename SearchT,
        typename FormatT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_last_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_lastF< InputT >::create_const( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_lastF< InputT >::create_const( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_lastF< InputT >::create( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }

//  replace_nth --------------------------------------------------------------------//

    // replace_nth iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename FormatIteratorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_nth_copy(
        OutputIteratorT Output,
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        unsigned int Nth,
        FormatIteratorT FormatBegin,
        FormatIteratorT FormatEnd )
    {
        return string_algo::replace_copy(
            Output,
            string_algo::make_range( Begin, End ), 
            string_algo::detail::
                create_find_nthF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ), Nth ),
            string_algo::detail::
                identity_formatF< iterator_range<FormatIteratorT> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    template< 
        typename InputT, 
        typename SearchT,
        typename FormatT,
        typename OutputIteratorT >
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
            string_algo::detail::
                create_find_nthF< InputT >::create_const( Search, Nth ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_nthF< InputT >::create_const( Search, Nth ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_nthF< InputT >::create( Search, Nth ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }


//  replace_all --------------------------------------------------------------------//

    // replace_all iterator version
    template< 
        typename InputIteratorT, 
        typename SearchIteratorT, 
        typename FormatIteratorT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_all_copy(
        OutputIteratorT Output,
        InputIteratorT Begin,
        InputIteratorT End,
        SearchIteratorT SearchBegin,
        SearchIteratorT SearchEnd,
        FormatIteratorT FormatBegin,
        FormatIteratorT FormatEnd )
    {
        return string_algo::replace_all_copy(
            Output,
            string_algo::make_range( Begin, End ), 
            string_algo::detail::
                create_find_firstF< iterator_range<InputIteratorT> >::
                    create_const( 
                        string_algo::make_range( SearchBegin, SearchEnd ) ),
            string_algo::detail::
                identity_formatF< iterator_range<FormatIteratorT> >(
                    string_algo::make_range( FormatBegin, FormatEnd ) ) );
    }

    // replace_all iterator version
    template< 
        typename InputT, 
        typename SearchT,
        typename FormatT,
        typename OutputIteratorT >
    inline OutputIteratorT replace_all_copy(
        OutputIteratorT Output,
        const InputT& Input,
        const SearchT& Search,
        const FormatT& Format )
    {
        return string_algo::replace_all_copy(
            Output,
            Input,
            string_algo::detail::
                create_find_firstF< InputT >::create_const( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_firstF< InputT >::create_const( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
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
            string_algo::detail::
                create_find_firstF< InputT >::create( Search ),
            string_algo::detail::
                identity_formatF<FormatT>( Format ) );
    }
    

} // namespace boost


#endif  // BOOST_REPLACE_HPP

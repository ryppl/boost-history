//  Boost string_algo library trim.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_TRIM_HPP
#define BOOST_STRING_TRIM_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/detail/trim.hpp>
#include <boost/string_algo/classification.hpp>
#include <locale>

// some macros for simplify definition of trim functions
#define BOOST_STRING_TRIM_SEQ_FWD( Alg, Seq, Pred ) Alg( Seq, Pred )

namespace boost {

//  left trim  -----------------------------------------------//

    // iterator version of left trim
    template< typename OutputIteratorT, typename SeqT, typename PredicateT >
    inline OutputIteratorT trim_left_copy_if( 
        OutputIteratorT Output,
        const SeqT& Input,
        PredicateT IsSpace )
    {
        std::copy( 
            string_algo::detail::trim_begin_if( 
                string_algo::begin(Input), 
                string_algo::end(Input), 
                IsSpace ),
            string_algo::end(Input),
            Output );

        return Output;
    }

    // const version of left trim
    template< typename SeqT, typename PredicateT >
    inline SeqT trim_left_copy_if( const SeqT& Input, PredicateT IsSpace )
    {
        return SeqT( 
            string_algo::detail::trim_begin_if( 
                string_algo::begin(Input), 
                string_algo::end(Input), 
                IsSpace ),
            string_algo::end(Input) );
    }

    // in-place version of left trim
    template< typename SeqT, typename PredicateT >
    inline SeqT& trim_left_if( SeqT& Input, PredicateT IsSpace )
    {
        Input.erase( 
            string_algo::begin(Input),
            string_algo::detail::trim_begin_if( 
                string_algo::begin(Input), 
                string_algo::end(Input), 
                IsSpace ));

        return Input;
    }

//  right trim  -----------------------------------------------//

    // iterator version of left trim
    template< typename OutputIteratorT, typename SeqT, typename PredicateT >
    inline OutputIteratorT trim_right_copy_if( 
        OutputIteratorT Output,
        const SeqT& Input,
        PredicateT IsSpace )
    {
        std::copy( 
            string_algo::begin(Input),
            string_algo::detail::trim_end_if( 
                string_algo::begin(Input), 
                string_algo::end(Input), 
                IsSpace ),
            Output );

        return Output;
    }

    // const version of right trim
    template< typename SeqT, typename PredicateT >
    inline SeqT trim_right_copy_if( const SeqT& Input, PredicateT IsSpace )
    {
        return SeqT( 
            string_algo::begin(Input),
            string_algo::detail::trim_end_if( 
                string_algo::begin(Input), 
                string_algo::end(Input), 
                IsSpace )
            );
    }
    
    // in-place version of right trim
    template< typename SeqT, typename PredicateT >
    inline SeqT& trim_right_if( SeqT& Input, PredicateT IsSpace )
    {
        Input.erase(
            string_algo::detail::trim_end_if( 
                string_algo::begin(Input), 
                string_algo::end(Input), 
                IsSpace ),
            string_algo::end(Input)
            );

        return Input;
    }

//  both side trim  -----------------------------------------------//

    // iterator version of left trim
    template< typename OutputIteratorT, typename SeqT, typename PredicateT >
    inline OutputIteratorT trim_copy_if( 
        OutputIteratorT Output,
        const SeqT& Input,
        PredicateT IsSpace )
    {
        BOOST_STRING_TYPENAME 
            string_algo::container_traits<SeqT>::const_iterator TrimEnd=
            string_algo::detail::trim_end_if( 
                string_algo::begin(Input), 
                string_algo::end(Input), 
                IsSpace);

        std::copy( 
            string_algo::detail::trim_begin_if( 
                string_algo::begin(Input), TrimEnd, IsSpace ),
            TrimEnd,
            Output
            );

        return Output;
    }

    // const version of trim
    template< typename SeqT, typename PredicateT >
    inline SeqT trim_copy_if( const SeqT& Input, PredicateT IsSpace )
    {
        BOOST_STRING_TYPENAME SeqT::const_iterator TrimEnd=
            string_algo::detail::trim_end_if( string_algo::begin(Input), string_algo::end(Input), IsSpace);

        return SeqT( 
            string_algo::detail::trim_begin_if( 
                string_algo::begin(Input), 
                TrimEnd, 
                IsSpace ),
            TrimEnd
            );
    }
    
    // in-place version of trim
    template< typename SeqT, typename PredicateT >
    inline SeqT& trim_if( SeqT& Input, PredicateT IsSpace )
    {
        return trim_left_if( trim_right_if( Input, IsSpace ), IsSpace );
    }
    
//  standard shortcuts  -----------------------------------------------//

    template< typename SeqT >
    inline SeqT trim_left_copy( const SeqT& Input, const std::locale& Loc=std::locale() )
    {
        return 
            BOOST_STRING_TRIM_SEQ_FWD( 
                trim_left_copy_if, 
                Input, 
                is_space<BOOST_STRING_TYPENAME SeqT::value_type>( Loc ) );
    }
    template< typename SeqT >
    inline SeqT& trim_left( SeqT& Input, const std::locale& Loc=std::locale() )
    {
        return 
            BOOST_STRING_TRIM_SEQ_FWD( 
                trim_left_if, 
                Input, 
                is_space<BOOST_STRING_TYPENAME SeqT::value_type>( Loc ) );
    }
    template< typename SeqT >
    inline SeqT trim_right_copy( const SeqT& Input, const std::locale& Loc=std::locale() )
    {
        return 
            BOOST_STRING_TRIM_SEQ_FWD( 
                trim_right_copy_if, 
                Input, 
                is_space<BOOST_STRING_TYPENAME SeqT::value_type>( Loc ) );
    }
    template< typename SeqT >
    inline SeqT& trim_right( SeqT& Input, const std::locale& Loc=std::locale() )
    {
        return 
            BOOST_STRING_TRIM_SEQ_FWD( 
                trim_right_if, 
                Input, 
                is_space<BOOST_STRING_TYPENAME SeqT::value_type>( Loc ) );
    }
    template< typename SeqT >
    inline SeqT trim_copy( const SeqT& Input, const std::locale& Loc=std::locale() )
    {
        return
            BOOST_STRING_TRIM_SEQ_FWD( 
                trim_copy_if, 
                Input, 
                is_space<BOOST_STRING_TYPENAME SeqT::value_type>( Loc ) );
    }
    template< typename SeqT >
    inline SeqT& trim( SeqT& Input, const std::locale& Loc=std::locale() )
    {
        return
            BOOST_STRING_TRIM_SEQ_FWD( 
                trim_if, 
                Input, 
                is_space<BOOST_STRING_TYPENAME SeqT::value_type>( Loc ) );
    }


} // namespace boost

// remove macro definitions
#undef BOOST_STRING_TRIM_SEQ_FWD

#endif  // BOOST_STRING_TRIM_HPP

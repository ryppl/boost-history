//  Boost string_algo library trim.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_TRIM_HPP
#define BOOST_STRING_TRIM_HPP

#include <locale>
#include <boost/string_algo/detail/trim.hpp>
#include <boost/string_algo/classification.hpp>

// some macros for simplify definition of trim functions
#define BOOST_STRING_TRIM_SEQ_FWD( Alg, Seq, Pred ) Alg( Seq, Pred )

namespace boost {

//  left trim  -----------------------------------------------//

    // iterator version of left trim
    template< typename OutputIteratorT, typename ForwardIteratorT, typename PredicateT >
    inline OutputIteratorT trim_left_copy_if( 
        OutputIteratorT Output,
        ForwardIteratorT InBegin,
        ForwardIteratorT InEnd,
        PredicateT IsSpace )
    {
        std::copy( 
            string_algo::detail::trim_begin_if( InBegin, InEnd, IsSpace ),
            InEnd,
            Output );

        return Output;
    }

    // const version of left trim
    template< typename SeqT, typename PredicateT >
    inline SeqT trim_left_copy_if( const SeqT& Input, PredicateT IsSpace )
    {
        return SeqT( 
                string_algo::detail::trim_begin_if( Input.begin(), Input.end(), IsSpace ),
                Input.end() );
    }

    // in-place version of left trim
    template< typename SeqT, typename PredicateT >
    inline SeqT& trim_left_if( SeqT& Input, PredicateT IsSpace )
    {
        Input.erase( 
            Input.begin(),
            string_algo::detail::trim_begin_if( Input.begin(), Input.end(), IsSpace ));

        return Input;
    }

//  right trim  -----------------------------------------------//

    // iterator version of left trim
    template< typename OutputIteratorT, typename ForwardIteratorT, typename PredicateT >
    inline OutputIteratorT trim_right_copy_if( 
        OutputIteratorT Output,
        ForwardIteratorT InBegin,
        ForwardIteratorT InEnd,
        PredicateT IsSpace )
    {
        std::copy( 
            InBegin,
            string_algo::detail::trim_end_if( InBegin, InEnd, IsSpace ),
            Output );

        return Output;
    }

    // const version of right trim
    template< typename SeqT, typename PredicateT >
    inline SeqT trim_right_copy_if( const SeqT& Input, PredicateT IsSpace )
    {
        return SeqT( 
            Input.begin(),
            string_algo::detail::trim_end_if( Input.begin(), Input.end(), IsSpace )
            );
    }
    
    // in-place version of right trim
    template< typename SeqT, typename PredicateT >
    inline SeqT& trim_right_if( SeqT& Input, PredicateT IsSpace )
    {
        Input.erase(
            string_algo::detail::trim_end_if( Input.begin(), Input.end(), IsSpace ),
            Input.end()
            );

        return Input;
    }

//  both side trim  -----------------------------------------------//

    // iterator version of left trim
    template< typename OutputIteratorT, typename ForwardIteratorT, typename PredicateT >
    inline OutputIteratorT trim_copy_if( 
        OutputIteratorT Output,
        ForwardIteratorT InBegin,
        ForwardIteratorT InEnd,
        PredicateT IsSpace )
    {
        ForwardIteratorT TrimEnd=
            string_algo::detail::trim_end_if( InBegin, InEnd, IsSpace);

        std::copy( 
            string_algo::detail::trim_begin_if( InBegin, TrimEnd, IsSpace ),
            TrimEnd,
            Output
            );

        return Output;
    }

    // const version of trim
    template< typename SeqT, typename PredicateT >
    inline SeqT trim_copy_if( const SeqT& Input, PredicateT IsSpace )
    {
        typename SeqT::const_iterator TrimEnd=
            string_algo::detail::trim_end_if( Input.begin(), Input.end(), IsSpace);

        return SeqT( 
            string_algo::detail::trim_begin_if( Input.begin(), TrimEnd, IsSpace ),
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
				is_space<BOOST_STRING_DEDUCED_TYPENAME SeqT::value_type>( Loc ) );
    }
    template< typename SeqT >
    inline SeqT& trim_left( SeqT& Input, const std::locale& Loc=std::locale() )
    {
        return 
            BOOST_STRING_TRIM_SEQ_FWD( 
                trim_left_if, 
				Input, 
				is_space<BOOST_STRING_DEDUCED_TYPENAME SeqT::value_type>( Loc ) );
    }
    template< typename SeqT >
    inline SeqT trim_right_copy( const SeqT& Input, const std::locale& Loc=std::locale() )
    {
        return 
            BOOST_STRING_TRIM_SEQ_FWD( 
                trim_right_copy_if, 
				Input, 
				is_space<BOOST_STRING_DEDUCED_TYPENAME SeqT::value_type>( Loc ) );
    }
    template< typename SeqT >
    inline SeqT& trim_right( SeqT& Input, const std::locale& Loc=std::locale() )
    {
        return 
            BOOST_STRING_TRIM_SEQ_FWD( 
                trim_right_if, 
				Input, 
				is_space<BOOST_STRING_DEDUCED_TYPENAME SeqT::value_type>( Loc ) );
    }
    template< typename SeqT >
    inline SeqT trim_copy( const SeqT& Input, const std::locale& Loc=std::locale() )
    {
        return
            BOOST_STRING_TRIM_SEQ_FWD( 
                trim_copy_if, 
				Input, 
				is_space<BOOST_STRING_DEDUCED_TYPENAME SeqT::value_type>( Loc ) );
    }
    template< typename SeqT >
    inline SeqT& trim( SeqT& Input, const std::locale& Loc=std::locale() )
    {
        return
            BOOST_STRING_TRIM_SEQ_FWD( 
                trim_if, 
				Input, 
				is_space<BOOST_STRING_DEDUCED_TYPENAME SeqT::value_type>( Loc ) );
    }


} // namespace boost

// remove macro definitions
#undef BOOST_STRING_TRIM_SEQ_FWD

#endif  // BOOST_STRING_TRIM_HPP

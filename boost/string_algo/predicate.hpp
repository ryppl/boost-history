//  Boost string_algo library predicate.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_PREDICATE_HPP
#define BOOST_STRING_PREDICATE_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/compare.hpp>
#include <boost/string_algo/predicate2.hpp>

namespace boost {

//  starts_with predicate  -----------------------------------------------//

    // starts_with
    template< typename SeqT1, typename SeqT2 >
    inline bool starts_with( 
        const SeqT1& Input, 
        const SeqT2& Test )
    {
        return string_algo::starts_with( Input, Test, string_algo::is_equal() );
    }

    // starts_with ( case insensitive )
    template< typename SeqT1, typename SeqT2 >
    inline bool istarts_with( 
        const SeqT1& Input, 
        const SeqT2& Test )
    {
        return string_algo::starts_with( Input, Test, string_algo::is_iequal() );
    }

//  ends_with predicate  -----------------------------------------------//

    // ends_with
    template< typename SeqT1, typename SeqT2 >
    inline bool ends_with( 
        const SeqT1& Input, 
        const SeqT2& Test )
    {
        return string_algo::ends_with( Input, Test, string_algo::is_equal() );
    }

    // ends_with ( case insensitive )
    template< typename SeqT1, typename SeqT2 >
    inline bool iends_with( 
        const SeqT1& Input, 
        const SeqT2& Test )
    {
        return string_algo::ends_with( Input, Test, string_algo::is_iequal() );
    }

//  contains predicate  -----------------------------------------------//

    // contains
    template< typename SeqT1, typename SeqT2 >
    inline bool contains( 
        const SeqT1& Input, 
        const SeqT2& Test )
    {
        return string_algo::contains( Input, Test, string_algo::is_equal() );
    }

    // contains ( case insensitive )
    template< typename SeqT1, typename SeqT2 >
    inline bool icontains( 
        const SeqT1& Input, 
        const SeqT2& Test )
    {
        return string_algo::contains( Input, Test, string_algo::is_iequal() );
    }

//  equal predicate  -----------------------------------------------//

    // equals 
    template< typename SeqT1, typename SeqT2 >
    inline bool equals( 
        const SeqT1& Input, 
        const SeqT2& Test )
    {
        return string_algo::equals( Input, Test, string_algo::is_equal() );
    }

    // equals ( case insensitive )
    template< typename SeqT1, typename SeqT2 >
    inline bool iequals( 
        const SeqT1& Input, 
        const SeqT2& Test )
    {
        return string_algo::equals( Input, Test, string_algo::is_iequal() );
    }

//  all predicate  -----------------------------------------------//

    // all predicate sequence version
    template< typename SeqT1, typename PredicateT >
    inline bool all( 
        const SeqT1& Input, 
        PredicateT Predicate )
    {
        typedef BOOST_STRING_TYPENAME 
            string_algo::container_traits<SeqT1>::const_iterator Iterator1T;

        Iterator1T InputEnd=string_algo::end(Input);
        for( Iterator1T It=string_algo::begin(Input); It!=InputEnd; It++ )
        {
            if ( !Predicate(*It) )
                return false;
        }
        
        return true;
    }

} // namespace boost


#endif  // BOOST_STRING_PREDICATE_HPP

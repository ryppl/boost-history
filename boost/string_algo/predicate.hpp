//  Boost string_algo library predicate.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_PREDICATE_HPP
#define BOOST_STRING_PREDICATE_HPP

#include <algorithm>

#include <boost/string_algo/traits.hpp>
#include <boost/string_algo/find.hpp>

namespace boost {

//  prefix predicates  -----------------------------------------------//

    // start_with iterator version
    template< typename InputIterator, typename SubIterator >
    inline bool starts_with( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd )
    {
        InputIterator it=Begin;
        SubIterator pit=SubBegin;
        for(;
            it!=End && pit!=SubEnd;
            it++,pit++)
        {
            if( !( (*it)==(*pit) ) )
                return false;
        }

        return pit==SubEnd;
    }

    template< 
        typename InputIterator, 
        typename SubIterator, 
        typename BinaryPredicate >
    inline bool start_with( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd,
        BinaryPredicate Comp )
    {
        InputIterator it=Begin;
        SubIterator pit=SubBegin;
        for(;
            it!=End && pit!=SubEnd;
            it++,pit++)
        {
            if( !Comp(*it,*pit ) )
                return false;
        }

        return pit==SubEnd;
    }

    // start_with sequence version
    template< typename SeqT1, typename SeqT2 >
    inline bool starts_with( 
        const SeqT1& Input, 
        const SeqT2& Substr )
    {
        return starts_with( Input.begin(), Input.end(), Substr.begin(), Substr.end() );
    }

    // end_with sequence version
    template< typename SeqT1, typename SeqT2 >
    inline bool ends_with( 
        const SeqT1& Input, 
        const SeqT2& Substr )
    {
        return starts_with( Input.rbegin(), Input.rend(), Substr.rbegin(), Substr.rend() );
    }

//  contains predicates  -----------------------------------------------//

    // contains iterator version
    template< typename InputIterator, typename SubIterator >
    inline bool contains( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd )
    {
        if ( SubBegin==SubEnd )
        {
            // Empty range is contained always
            return true;
        }
        
        return ( !find_first( Begin, End, SubBegin, SubEnd ).empty() );
    }

    // contains sequence version
    template< typename SeqT1, typename SeqT2 >
    inline bool contains( 
        const SeqT1& Input, 
        const SeqT2& Substr )
    {
        return contains( Input.begin(), Input.end(), Substr.begin(), Substr.end() );
    }


//  equal predicate  -----------------------------------------------//

    // equal iterator version
    template< typename InputIterator, typename SubIterator >
    inline bool equals( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd )
    {
        InputIterator it=Begin;
        SubIterator pit=SubBegin;
        for(;
            it!=End && pit!=SubEnd;
            it++,pit++)
        {
            if( !( (*it)==(*pit) ) )
                return false;
        }

        return ( pit==SubEnd ) && ( it==End );
    }

    template< 
        typename InputIterator, 
        typename SubIterator, 
        typename BinaryPredicate >
    inline bool equals( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd,
        BinaryPredicate Comp )
    {
        InputIterator it=Begin;
        SubIterator pit=SubBegin;
        for(;
            it!=End && pit!=SubEnd;
            it++,pit++)
        {
            if( !Comp(*it,*pit) )
                return false;
        }

        return ( pit==SubEnd ) && ( it==End );
    }

    // start_with sequence version
    template< typename SeqT1, typename SeqT2 >
    inline bool equals( 
        const SeqT1& Input, 
        const SeqT2& Substr )
    {
        return equals( Input.begin(), Input.end(), Substr.begin(), Substr.end() );
    }


} // namespace boost


#endif  // BOOST_STRING_PREDICATE_HPP

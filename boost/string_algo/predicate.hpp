//  Boost string_algo library predicate.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_PREDICATE_HPP
#define BOOST_STRING_PREDICATE_HPP

#include <boost/string_algo/find.hpp>
#include <boost/string_algo/detail/predicate.hpp>

namespace boost {

//  starts_with predicate  -----------------------------------------------//

    // start_with iterator version
    template< typename InputIteratorT, typename SubIteratorT >
    inline bool starts_with( 
        InputIteratorT Begin, 
        InputIteratorT End, 
        SubIteratorT SubBegin,
        SubIteratorT SubEnd )
    {
        InputIteratorT it=Begin;
        SubIteratorT pit=SubBegin;
        for(;
            it!=End && pit!=SubEnd;
            it++,pit++)
        {
            if( !( (*it)==(*pit) ) )
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

//  ends_with predicate  -----------------------------------------------//

    template< typename InputIteratorT, typename SubIteratorT >
    inline bool ends_with( 
        InputIteratorT Begin, 
        InputIteratorT End, 
        SubIteratorT SubBegin,
        SubIteratorT SubEnd )
    {
        typedef typename boost::detail::
            iterator_traits<InputIteratorT>::iterator_category category;

        return string_algo::detail::
            ends_with_iter_select( 
                Begin, End, SubBegin, SubEnd, category() );
    }

    // end_with sequence version
    template< typename SeqT1, typename SeqT2 >
    inline bool ends_with( 
        const SeqT1& Input, 
        const SeqT2& Substr )
    {
        return ends_with( Input.begin(), Input.end(), Substr.begin(), Substr.end() );
    }

//  contains predicate  -----------------------------------------------//

    // contains iterator version
    template< typename InputIteratorT, typename SubIteratorT >
    inline bool contains( 
        InputIteratorT Begin, 
        InputIteratorT End, 
        SubIteratorT SubBegin,
        SubIteratorT SubEnd )
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
    template< typename InputIteratorT, typename SubIteratorT >
    inline bool equals( 
        InputIteratorT Begin, 
        InputIteratorT End, 
        SubIteratorT SubBegin,
        SubIteratorT SubEnd )
    {
        InputIteratorT it=Begin;
        SubIteratorT pit=SubBegin;
        for(;
            it!=End && pit!=SubEnd;
            it++,pit++)
        {
            if( !( (*it)==(*pit) ) )
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

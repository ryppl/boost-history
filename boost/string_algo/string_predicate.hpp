//  Boost string_algo library string_predicate.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_PREDICATE_HPP
#define BOOST_STRING_PREDICATE_HPP

#include <algorithm>
#include <functional>

#include "string_funct.hpp"
#include "string_traits.hpp"
#include "string_find.hpp"

namespace boost {

//  string predicates  -----------------------------------------------//

    // start_with iterator version
    template< typename InputIterator, typename SubIterator >
    inline bool starts_with( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd )
    {
        // Instantiate comp from search traits
        typename string_algo::search_traits< InputIterator, SubIterator >
            ::compare_function_type Comp;

        InputIterator it=Begin;
        SubIterator pit=SubBegin;
        for(;
            it!=End && pit!=SubEnd;
            it++,pit++)
        {
            if( !Comp(*it,*pit) )
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
        
        std::pair<InputIterator, InputIterator> M=
            find_first( Begin, End, SubBegin, SubEnd );

        return ( M.first != M.second );
    }

    // contains sequence version
    template< typename SeqT1, typename SeqT2 >
    inline bool contains( 
        const SeqT1& Input, 
        const SeqT2& Substr )
    {
        return contains( Input.begin(), Input.end(), Substr.begin(), Substr.end() );
    }

} // namespace boost


#endif  // BOOST_STRING_PREDICATE_HPP

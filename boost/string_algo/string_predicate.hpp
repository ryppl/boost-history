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
#include "string_substr.hpp"

namespace boost {

//  string predicates  -----------------------------------------------//

    // start_with iterator version
    template< typename InputIterator, typename SubIterator, typename BinaryPredicate >
    inline bool starts_with( 
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

        return pit==SubEnd;
    }

    // start_with iterator version with implicit comparison predicate
    template< typename InputIterator, typename SubIterator >
    inline bool starts_with( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd )
    {
        return starts_with( 
            Begin,
            End,
            SubBegin,
            SubEnd,
            string_util_impl::equal_toF< typename InputIterator::value_type, typename SubIterator::value_type >() );
    }


    // start_with sequence version
    template< typename Seq1, typename Seq2, typename BinaryPredicate >
    inline bool starts_with( 
        const Seq1& Input, 
        const Seq2& Substr, 
        BinaryPredicate Comp )
    {
        return starts_with( Input.begin(), Input.end(), Substr.begin(), Substr.end(), Comp );
    }

    // start_with sequence version with implicit comparison predicate
    template< typename Seq1, typename Seq2 >
    inline bool starts_with( const Seq1& Input, const Seq2& Substr )
    {
        return starts_with( 
            Input, 
            Substr, 
            string_util_impl::equal_toF<typename Seq1::value_type, typename Seq2::value_type>());
    }

    // end_with sequence version
    template< typename Seq1, typename Seq2, typename BinaryPredicate >
    inline bool ends_with( 
        const Seq1& Input, 
        const Seq2& Substr, 
        BinaryPredicate Comp )
    {
        return starts_with( Input.rbegin(), Input.rend(), Substr.rbegin(), Substr.rend(), Comp );
    }

    // end_with sequence version with implicit comparison predicate
    template< typename Seq1, typename Seq2 >
    inline bool ends_with( const Seq1& Input, const Seq2& Substr )
    {
        return ends_with( 
            Input, 
            Substr, 
            string_util_impl::equal_toF<typename Seq1::value_type, typename Seq2::value_type>());
    }

    // contains iterator version
    template< typename InputIterator, typename SubIterator, typename BinaryPredicate >
    inline bool contains( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd,
        BinaryPredicate Comp )
    {
        if ( SubBegin==SubEnd )
        {
            // Empty range is contained always
            return true;
        }
        
        std::pair<InputIterator, InputIterator> M=
            find( Begin, End, SubBegin, SubEnd );

        return ( M.first != M.second );
    }

    // contains iterator version with implicit comparison predicate
    template< typename InputIterator, typename SubIterator >
    inline bool contains( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd )
    {
        return contains( 
            Begin,
            End,
            SubBegin,
            SubEnd,
            string_util_impl::equal_toF< typename InputIterator::value_type, typename SubIterator::value_type >() );
    }

    // contains sequence version
    template< typename Seq1, typename Seq2, typename BinaryPredicate >
    inline bool contains( 
        const Seq1& Input, 
        const Seq2& Substr, 
        BinaryPredicate Comp )
    {
        return contains( Input.begin(), Input.end(), Substr.begin(), Substr.end(), Comp );
    }

    // contains sequence version with implicit comparison predicate
    template< typename Seq1, typename Seq2 >
    inline bool contains( const Seq1& Input, const Seq2& Substr )
    {
        return contains( 
            Input, 
            Substr, 
            string_util_impl::equal_toF<typename Seq1::value_type, typename Seq2::value_type>());
    }

} // namespace boost


#endif  // BOOST_STRING_PREDICATE_HPP

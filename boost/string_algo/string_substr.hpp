//  Boost string_algo library string_substr.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_SUBSTR_HPP
#define BOOST_STRING_SUBSTR_HPP

#include <algorithm>
#include <functional>
#include <utility>

#include "string_funct.hpp"

namespace boost {

//  sequence find  -----------------------------------------------//

    // find a subsequnce in the sequence
    /*
        Returns a pair <begin,end> marking the subsequence in the sequence. It the find fails,
        returns <End,End>
    */
    template< typename InputIterator, typename SubIterator, typename BinaryPredicate >
    inline std::pair<InputIterator,InputIterator> 
    find( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd,
        BinaryPredicate Comp )
    {
        // Outer loop
        for(InputIterator OuterIt=Begin;
            OuterIt!=End;
            OuterIt++)
        {
            InputIterator InnerIt=OuterIt;
            SubIterator SubstrIt=SubBegin;
            for(;
                InnerIt!=End && SubstrIt!=SubEnd;
                InnerIt++,SubstrIt++)
            {
                if( !Comp(*InnerIt,*SubstrIt) )
                    break;
            }

            // Substring matching succeeded
            if ( SubstrIt==SubEnd )
                return std::make_pair( OuterIt, InnerIt );
        }

        return std::make_pair( End, End );
    }

    // find a subsequnce in the sequence
    /*
        Implicit parameters are not correctly handled by VC7 so we have to provide
        two variants of the same method
    */
    template< typename InputIterator, typename SubIterator >
    inline std::pair<InputIterator,InputIterator> 
    find( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd )
    {
        return find( 
            Begin, End, 
            SubBegin, SubEnd, 
            string_util_impl::equal_toF<
                typename InputIterator::value_type, 
                typename SubIterator::value_type>());
    }

//  find_first  -----------------------------------------------//

    // find_first sequence const version
    template< typename Seq1, typename Seq2, typename BinaryPredicate >
    inline std::pair<typename Seq1::const_iterator, typename Seq1::const_iterator> 
    find_first( 
        const Seq1& Input, 
        const Seq2& Substr,
        BinaryPredicate Comp )
    {
        return find( Input.begin(), Input.end(), Substr.begin(), Substr.end(), Comp );
    }

    // find_first sequence const version
    /*
        Implicit parameters are not correctly handled by VC7 so we have to provide
        two variants of the same method
    */
    template< typename Seq1, typename Seq2 >
    inline std::pair<typename Seq1::const_iterator, typename Seq1::const_iterator> 
    find_first( 
        const Seq1& Input, 
        const Seq2& Substr )
    {
        return find_first( 
            Input, 
            Substr,
            string_util_impl::equal_toF<
                typename Seq1::value_type, 
                typename Seq2::value_type>());
    }

    // find_first sequence non-const version
    template< typename Seq1, typename Seq2, typename BinaryPredicate >
    inline std::pair<typename Seq1::iterator, typename Seq1::iterator> 
    BOOST_STRING_NON_CONST_FUNCTION(find_first)( 
        Seq1& Input, 
        const Seq2& Substr,
        BinaryPredicate Comp )
    {
        return find( Input.begin(), Input.end(), Substr.begin(), Substr.end(), Comp );
    }

    // find_first sequence non-const version
    /*
        Implicit parameters are not correctly handled by VC7 so we have to provide
        two variants of the same method
    */
    template< typename Seq1, typename Seq2 >
    inline std::pair<typename Seq1::iterator, typename Seq1::iterator> 
    BOOST_STRING_NON_CONST_FUNCTION(find_first)( 
        Seq1& Input, 
        const Seq2& Substr )
    {
        return BOOST_STRING_NON_CONST_FUNCTION(find_first)( 
            Input, 
            Substr,
            string_util_impl::equal_toF<
                typename Seq1::value_type, 
                typename Seq2::value_type>()); 
    }

//  find_last  -----------------------------------------------//

    // find_last sequence const version
    template< typename Seq1, typename Seq2, typename BinaryPredicate >
    inline std::pair<typename Seq1::const_iterator, typename Seq1::const_iterator> 
    find_last( 
        const Seq1& Input, 
        const Seq2& Substr,
        BinaryPredicate Comp )
    {
        std::pair<
            typename Seq1::const_reverse_iterator, 
            typename Seq1::const_reverse_iterator> result=
        find( Input.rbegin(), Input.rend(), Substr.rbegin(), Substr.rend(), Comp );

        return std::make_pair( result.second.base(), result.first.base() );
    }

    // find_last sequence const version
    /*
        Implicit parameters are not correctly handled by VC7 so we have to provide
        two variants of the same method
    */
    template< typename Seq1, typename Seq2 >
    inline std::pair<typename Seq1::const_iterator, typename Seq1::const_iterator> 
    find_last( 
        const Seq1& Input, 
        const Seq2& Substr )
    {
        return find_last( 
            Input, 
            Substr,
            string_util_impl::equal_toF<
                typename Seq1::value_type, 
                typename Seq2::value_type>());
    }

    // find_last sequence non-const version
    template< typename Seq1, typename Seq2, typename BinaryPredicate >
    inline std::pair<typename Seq1::iterator, typename Seq1::iterator> 
    BOOST_STRING_NON_CONST_FUNCTION(find_last)( 
        Seq1& Input, 
        const Seq2& Substr,
        BinaryPredicate Comp )
    {
        std::pair<
            typename Seq1::reverse_iterator, 
            typename Seq1::reverse_iterator> result=
        find( Input.rbegin(), Input.rend(), Substr.rbegin(), Substr.rend(), Comp );

        return std::make_pair( result.second.base(), result.first.base() );
    }

    // find_last sequence non-const version
    /*
        Implicit parameters are not correctly handled by VC7 so we have to provide
        two variants of the same method
    */
    template< typename Seq1, typename Seq2 >
    inline std::pair<typename Seq1::iterator, typename Seq1::iterator> 
    BOOST_STRING_NON_CONST_FUNCTION(find_last)( 
        Seq1& Input, 
        const Seq2& Substr )
    {
        return BOOST_STRING_NON_CONST_FUNCTION(find_last)( 
            Input, 
            Substr,
            string_util_impl::equal_toF<
                typename Seq1::value_type, 
                typename Seq2::value_type>());
    }

//  replace_substr ----------------------------------------------------------------------//

template< typename Container > 
class Iterator : public Container::iterator {};

    // Replace range in the sequence with another range
    /*
        Returns the iterator pointing just after inserted subrange.
        Note:
            Function first tries to replace elements in the input sequence,
            up to To position with the elements form Repl sequence.
            The rest of the Repl is then inserted thereafter.
            If the Repl sequence is shorter, overlaping elements are erased from the Input
    */
    template< typename InputSeq, typename ReplSeq >
    inline typename InputSeq::iterator
    replace_substr(
        InputSeq& Input,
        typename InputSeq::iterator From,
        typename InputSeq::iterator To,
        ReplSeq& Repl )
    {       
        typename InputSeq::iterator InsertIt=From;
        bool bReplace=(InsertIt!=To);
        
        for( typename ReplSeq::const_iterator ReplIt=Repl.begin();
            ReplIt!=Repl.end();
            ReplIt++)
        {
            if ( bReplace )
            {
                // Replace mode
                *InsertIt=*ReplIt;

                InsertIt++;
                bReplace=(InsertIt!=To);
            }
            else
            {
                // Insert mode
                InsertIt=Input.insert( InsertIt, *ReplIt );
                // Advance to the next item
                InsertIt++;
            }
        }

        // Erase the overlapping elements
        if ( bReplace )
        {
            InsertIt=Input.erase( InsertIt, To );
        }

        // Return InputIt iterator
        return InsertIt;
    }
        
//  replace_first --------------------------------------------------------------------//

    // replace_first iterator version
    /*
        Create new sequence into output interator as a copy of input sequence,
        but with the first match replaced with substitute range
    */
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename ReplIterator,
        typename OutputIterator, 
        typename BinaryPredicate>
    inline void replace_first_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
        ReplIterator ReplBegin,
        ReplIterator ReplEnd,
        OutputIterator Output,
        BinaryPredicate Comp )
    {
        // Find first match
        std::pair<InputIterator,InputIterator> M=
            find( Begin, End, MatchBegin, MatchEnd, Comp );

        if ( M.first==M.second )
        {
            // Match not found - return original sequence
            std::copy( Begin, End, Output );
            return;
        }

        // Copy the beggining of the sequence
        std::copy( Begin, M.first, Output );
        // Copy repl range
        std::copy( ReplBegin, ReplEnd, Output );
        // Copy the rest of the sequence
        std::copy( M.second, End, Output );
    }

    // replace_first iterator version ( with default Comp predicate )
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename ReplIterator,
        typename OutputIterator >
    inline void replace_first_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
        ReplIterator ReplBegin,
        ReplIterator ReplEnd,
        OutputIterator Output )
    {
        replace_first_copy(
            Begin, End,
            MatchBegin, MatchEnd,
            ReplBegin, ReplEnd,
            Output,
            string_util_impl::equal_toF<
                typename InputIterator::value_type, 
                typename MatchIterator::value_type>() );
    }

    // replace_first sequence version
    template<
        typename InputSeq,
        typename MatchSeq,
        typename ReplSeq,
        typename BinaryPredicate >
    InputSeq replace_first_copy( 
        const InputSeq& Input,
        const MatchSeq& Match,
        const ReplSeq& Repl,
        BinaryPredicate Comp )
    {
        InputSeq Output;
        replace_first_copy( 
            Input.begin(), Input.end(), 
            Match.begin(), Match.end(), 
            Repl.begin(), Repl.end(), 
            std::back_inserter( Output ),
            Comp );

        return Output;
    }

    // replace_first sequence version ( with default Comp predicate )
    template<
        typename InputSeq,
        typename MatchSeq,
        typename ReplSeq >
    InputSeq replace_first_copy( 
        const InputSeq& Input,
        const MatchSeq& Match,
        const ReplSeq& Repl )
    {
        return replace_first_copy( 
            Input,
            Match,
            Repl,
            string_util_impl::equal_toF<
                typename InputSeq::value_type, 
                typename MatchSeq::value_type>() );
    }


//  replace_first ( in-place version ) ---------------------------------//

    // replace_first in-place sequence version
    /*
        Makes in-place replacement of the first match
    */
    template<
        typename InputSeq,
        typename MatchSeq,
        typename ReplSeq,
        typename BinaryPredicate >
    InputSeq& replace_first( 
        InputSeq& Input,
        const MatchSeq& Match,
        const ReplSeq& Repl,
        BinaryPredicate Comp )
    {
        // Find range for the match
        std::pair<typename InputSeq::iterator, typename InputSeq::iterator> M=
            find( Input.begin(), Input.end(), Match.begin(), Match.end(), Comp );

        if ( M.first==M.second )
        {
            // Match not found - return original sequence
            return Input;
        }

        // Replace match
        replace_substr( Input, M.first, M.second, Repl );
        
        return Input;
    }


    // replace_first in-place sequence version ( with default comp parameter )
    template<
        typename InputSeq,
        typename MatchSeq,
        typename ReplSeq >
    InputSeq& replace_first( 
        InputSeq& Input,
        const MatchSeq& Match,
        const ReplSeq& Repl )
    {
        return replace_first(
            Input,
            Match,
            Repl,
            string_util_impl::equal_toF<
                typename InputSeq::value_type, 
                typename MatchSeq::value_type>() );
    }

//  replace_all    ----------------------------------------------------------------//

    // replace_all iterator version
    /*
        Create new sequence into output interator as a copy of input sequence,
        but with the all matches replaced with substitute range
    */
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename ReplIterator,
        typename OutputIterator, 
        typename BinaryPredicate>
    inline void replace_all_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
        ReplIterator ReplBegin,
        ReplIterator ReplEnd,
        OutputIterator Output,
        BinaryPredicate Comp )
    {
        // Find the first match
        InputIterator LastMatch=Begin;
        std::pair<InputIterator,InputIterator> M=
            find( Begin, End, MatchBegin, MatchEnd, Comp );

        // Iterate throug all matches
        while( M.first != M.second )
        {
            // Copy the beggining of the sequence
            std::copy( LastMatch, M.first, Output );
            // Copy repl range
            std::copy( ReplBegin, ReplEnd, Output );

            // Proceed to the next match
            LastMatch=M.second;
            M=find( LastMatch, End, MatchBegin, MatchEnd, Comp );
        }

        // Copy the rest of the sequence
        std::copy( LastMatch, End, Output );
    }

    // replace_all iterator version ( with default Comp predicate )
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename ReplIterator,
        typename OutputIterator >
    inline void replace_all_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
        ReplIterator ReplBegin,
        ReplIterator ReplEnd,
        OutputIterator Output )
    {
        replace_all_copy(
            Begin, End,
            MatchBegin, MatchEnd,
            ReplBegin, ReplEnd,
            Output,
            string_util_impl::equal_toF<
                typename InputIterator::value_type, 
                typename MatchIterator::value_type>() );
    }


    // replace_all sequence version
    template<
        typename InputSeq,
        typename MatchSeq,
        typename ReplSeq,
        typename BinaryPredicate >
    InputSeq replace_all_copy( 
        const InputSeq& Input,
        const MatchSeq& Match,
        const ReplSeq& Repl,
        BinaryPredicate Comp )
    {
        InputSeq output;
        replace_all_copy( 
            Input.begin(), Input.end(), 
            Match.begin(), Match.end(), 
            Repl.begin(), Repl.end(), 
            std::back_inserter( output ),
            Comp );

        return output;
    }

    // replace_all sequence version ( with default Comp predicate )
    template<
        typename InputSeq,
        typename MatchSeq,
        typename ReplSeq >
    InputSeq replace_all_copy( 
        const InputSeq& Input,
        const MatchSeq& Match,
        const ReplSeq& Repl )
    {
        return replace_all_copy( 
            Input,
            Match,
            Repl,
            string_util_impl::equal_toF<
                typename InputSeq::value_type, 
                typename MatchSeq::value_type>() );
    }

//  replace_all ( in-place version ) ---------------------------------//

    // replace_all in-place sequence version
    /*
        Makes in-place replacement of all matches in the seqence
    */
    template<
        typename InputSeq,
        typename MatchSeq,
        typename ReplSeq,
        typename BinaryPredicate >
    InputSeq& replace_all( 
        InputSeq& Input,
        const MatchSeq& Match,
        const ReplSeq& Repl,
        BinaryPredicate Comp )
    {
        // Find the first match
        std::pair<typename InputSeq::iterator, typename InputSeq::iterator> M=
            find( Input.begin(), Input.end(), Match.begin(), Match.end(), Comp );

        // Iterate throug all matches
        while( M.first != M.second )
        {
            // Replace the match
            typename InputSeq::iterator It=replace_substr(
                Input,
                M.first, M.second,
                Repl );

            // Proceed to the next match
            M=find( It, Input.end(), Match.begin(), Match.end(), Comp );
        }
            
        return Input;
    }


    // replace_first in-place sequence version ( with default comp parameter )
    template<
        typename InputSeq,
        typename MatchSeq,
        typename ReplSeq >
    InputSeq& replace_all( 
        InputSeq& Input,
        const MatchSeq& Match,
        const ReplSeq& Repl )
    {
        return replace_all(
            Input,
            Match,
            Repl,
            string_util_impl::equal_toF<
                typename InputSeq::value_type, 
                typename MatchSeq::value_type>() );
    }

//  erase and variants  --------------------------------------------------------//
/*  
    erase is implemented as by calling replace_* variant with empty repl sequence 
*/

    // erase_first iterator version
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename OutputIterator, 
        typename BinaryPredicate>
    inline void erase_first_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
        OutputIterator Output,
        BinaryPredicate Comp )
    {
        replace_first_copy( Begin, End, MatchBegin, MatchBegin, End, End, Output, Comp );
    }

    // erase_first iterator version ( with defalt comp parameter )
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename OutputIterator >
    inline void erase_first_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
        OutputIterator Output )
    {
        replace_first_copy( Begin, End, MatchBegin, MatchBegin, End, End, Output );
    }

    // erase_first sequence version
    template<
        typename InputSeq,
        typename MatchSeq,
        typename BinaryPredicate >
    InputSeq erase_first_copy( 
        const InputSeq& Input,
        const MatchSeq& Match,
        BinaryPredicate Comp )
    {
        return replace_first_copy( Input, Match, InputSeq(), Comp );
    }

    // erase_first sequence version ( with default Comp predicate )
    template<
        typename InputSeq,
        typename MatchSeq >
    InputSeq erase_first_copy( 
        const InputSeq& Input,
        const MatchSeq& Match )
    {
        return replace_first_copy( Input, Match, InputSeq() );
    }

    // erase_first in-place sequence version
    template<
        typename InputSeq,
        typename MatchSeq,
        typename BinaryPredicate >
    InputSeq& erase_first( 
        InputSeq& Input,
        const MatchSeq& Match,
        BinaryPredicate Comp )
    {
        return replace_first( Input, Match, InputSeq(), Comp );
    }

    // erase_first in-place sequence version ( with default comp parameter )
    template<
        typename InputSeq,
        typename MatchSeq >
    InputSeq& erase_first( 
        InputSeq& Input,
        const MatchSeq& Match )
    {
        return replace_first( Input, Match, InputSeq() );
    }

    // erase_all iterator version
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename OutputIterator, 
        typename BinaryPredicate>
    inline void erase_all_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
        OutputIterator Output,
        BinaryPredicate Comp )
    {
        replace_all_copy( Begin, End, MatchBegin, MatchEnd, End, End, Output, Comp );
    }

    // erase_all iterator version ( with default Comp predicate )
    template< 
        typename InputIterator, 
        typename MatchIterator, 
        typename OutputIterator >
    inline void erase_all_copy(
        InputIterator Begin,
        InputIterator End,
        MatchIterator MatchBegin,
        MatchIterator MatchEnd,
        OutputIterator Output )
    {
        replace_all_copy( Begin, End, MatchBegin, MatchEnd, End, End, Output );
    }

    // erase_all sequence version
    template<
        typename InputSeq,
        typename MatchSeq,
        typename BinaryPredicate >
    InputSeq erase_all_copy( 
        const InputSeq& Input,
        const MatchSeq& Match,
        BinaryPredicate Comp )
    {
        return replace_all_copy( Input, Match, InputSeq(), Comp );
    }

    // erase_all sequence version ( with default Comp predicate )
    template<
        typename InputSeq,
        typename MatchSeq >
    InputSeq erase_all_copy( 
        const InputSeq& Input,
        const MatchSeq& Match )
    {
        return replace_all_copy( Input, Match, InputSeq() );
    }

    // erase_all in-place sequence version
    template<
        typename InputSeq,
        typename MatchSeq,
        typename BinaryPredicate >
    InputSeq& erase_all( 
        InputSeq& Input,
        const MatchSeq& Match,
        BinaryPredicate Comp )
    {
        return replace_all( Input, Match, InputSeq(), Comp );
    }

    // erase_all in-place sequence version ( with default comp parameter )
    template<
        typename InputSeq,
        typename MatchSeq >
    InputSeq& erase_all( 
        InputSeq& Input,
        const MatchSeq& Match )
    {
        return replace_all( Input, Match, InputSeq() );
    }


} // namespace boost


#endif  // BOOST_SUBSTR_HPP

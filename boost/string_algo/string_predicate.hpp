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
#include <locale>

namespace boost {

namespace string_util_impl {

    // equal_to functor reimplementation
    /*
        standard STL implementation does handles only comparison between types
        of the same value. This is too restrictive for us.
    */
    template< typename Type1, typename Type2 >
        struct equal_toF : public std::binary_function< Type1, Type2, bool > 
    {
        typedef typename std::binary_function< Type1, Type2, bool >::result_type result_type;
        typedef typename std::binary_function< Type1, Type2, bool >::first_argument_type first_argument_type;
        typedef typename std::binary_function< Type1, Type2, bool >::second_argument_type second_argument_type;

        result_type operator ()( first_argument_type Ch, const second_argument_type& Loc ) const
        {
            return Ch==Loc;
        }
    };

}; // string_util_impl

//  string predicates  -----------------------------------------------//

    // is_prefix iterator version
    template< typename InputIterator, typename PrefixIterator, typename BinaryPredicate >
    inline bool is_prefix( 
        InputIterator BeginSequence, 
        InputIterator EndSequence, 
        PrefixIterator BeginPrefix,
        PrefixIterator EndPrefix,
        BinaryPredicate Comp )
    {
        InputIterator it=BeginSequence;
        PrefixIterator pit=BeginPrefix;
        for(;
            it!=EndSequence && pit!=EndPrefix;
            it++,pit++)
        {
            if( !Comp(*it,*pit) )
                return false;
        }

        return pit==EndPrefix;
    }

    // is_prefix iterator version with implicit comparison predicate
    /*
        Implicit parameters are not correctly handled by VC7 so we have to provide
        two variants of the same method
    */
    template< typename InputIterator, typename PrefixIterator >
    inline bool is_prefix( 
        InputIterator BeginSequence, 
        InputIterator EndSequence, 
        PrefixIterator BeginPrefix,
        PrefixIterator EndPrefix )
    {
        return is_prefix( 
            BeginSequence,
            EndSequence,
            BeginPrefix,
            EndPrefix,
            string_util_impl::equal_toF< typename InputIterator::value_type, typename PrefixIterator::value_type >() );
    }


    // is_prefix sequence version
    template< typename Seq1, typename Seq2, typename BinaryPredicate >
    inline bool is_prefix( 
        const Seq1& Input, 
        const Seq2& Prefix, 
        BinaryPredicate Comp )
    {
        return is_prefix( Input.begin(), Input.end(), Prefix.begin(), Prefix.end(), Comp );
    }

    // is_prefix sequence version with implicit comparison predicate
    /*
        Implicit parameters are not correctly handled by VC7 so we have to provide
        two variants of the same method
    */
    template< typename Seq1, typename Seq2 >
    inline bool is_prefix( const Seq1& Input, const Seq2& Prefix )
    {
        return is_prefix( 
            Input, 
            Prefix, 
            string_util_impl::equal_toF<typename Seq1::value_type, typename Seq2::value_type>());
    }

    // is_suffix sequence version
    template< typename Seq1, typename Seq2, typename BinaryPredicate >
    inline bool is_suffix( 
        const Seq1& Input, 
        const Seq2& Prefix, 
        BinaryPredicate Comp )
    {
        return is_prefix( Input.rbegin(), Input.rend(), Prefix.rbegin(), Prefix.rend(), Comp );
    }

    // is_suffix sequence version with implicit comparison predicate
    /*
        Implicit parameters are not correctly handled by VC7 so we have to provide
        two variants of the same method
    */
    template< typename Seq1, typename Seq2 >
    inline bool is_suffix( const Seq1& Input, const Seq2& Prefix )
    {
        return is_suffix( 
            Input, 
            Prefix, 
            string_util_impl::equal_toF<typename Seq1::value_type, typename Seq2::value_type>());
    }


} // namespace boost


#endif  // BOOST_STRING_PREDICATE_HPP

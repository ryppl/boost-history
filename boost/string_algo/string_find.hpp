//  Boost string_algo library string_find.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_HPP
#define BOOST_STRING_FIND_HPP

#include <functional>

#include "string_traits.hpp"

namespace boost {

//  generic find functors -----------------------------------------------//

    namespace string_algo{

        // find a subsequnce in the sequence ( functor )
        /*
            Returns a pair <begin,end> marking the subsequence in the sequence. It the find fails,
            returns <End,End>
        */
        template< typename InputIterator, typename SubstrIterator, 
            typename SearchTraits=search_traits<InputIterator, SubstrIterator> >
        struct find_firstF
        {
            typedef typename SearchTraits::range_type range_type;
            typedef typename SearchTraits::compare_function_type compare_function_type;

            // Operation
            range_type operator()(        
                InputIterator Begin, 
                InputIterator End, 
                SubstrIterator SubBegin,
                SubstrIterator SubEnd )
            {
                // Instantiate comp from search traits
                compare_function_type Comp;
                
                // Outer loop
                for(InputIterator OuterIt=Begin;
                    OuterIt!=End;
                    OuterIt++)
                {
                    InputIterator InnerIt=OuterIt;
                    SubstrIterator SubstrIt=SubBegin;
                    for(;
                        InnerIt!=End && SubstrIt!=SubEnd;
                        InnerIt++,SubstrIt++)
                    {
                        if( !Comp(*InnerIt,*SubstrIt) )
                            break;
                    }

                    // Substring matching succeeded
                    if ( SubstrIt==SubEnd )
                        return range_type( OuterIt, InnerIt );
                }

                return range_type( End, End );
            }
        };

        // find the n-th match of a subsequnce in the sequence ( functor )
        /*
            Returns a pair <begin,end> marking the subsequence in the sequence. If the find fails,
            returns <End,End>
        */
        template< typename InputIterator, typename SubstrIterator, 
            typename SearchTraits=search_traits<InputIterator, SubstrIterator> >
        struct find_nthF
        {
            typedef typename SearchTraits::range_type range_type;
            typedef typename SearchTraits::compare_function_type compare_function_type;
            typedef find_firstF<InputIterator, SubstrIterator> find_firstF;

            // Constructor
            find_nthF( unsigned int Nth ) : m_Nth( Nth ) {}

            // Operation
            range_type operator()(        
                InputIterator Begin, 
                InputIterator End, 
                SubstrIterator SubBegin,
                SubstrIterator SubEnd   )
            {
                // Instantiate find funtor 
                find_firstF find_first;

                range_type M( Begin, Begin );

                for( unsigned int n=0; n<=m_Nth; n++ )
                {
                    // find next match        
                    M=find_first( M.second, End, SubBegin, SubEnd );

                    if ( M.first==M.second )
                    {
                        // Subsequence not found, return 
                        return range_type( End, End );
                    }
                }

                return M;
            }

        private:
            unsigned int m_Nth;
        };

        // find the last match a subsequnce in the sequence ( functor )
        /*
            Returns a pair <begin,end> marking the subsequence in the sequence. If the find fails,
            returns <End,End>
        */
        template< typename InputIterator, typename SubstrIterator, 
            typename SearchTraits=search_traits<InputIterator, SubstrIterator> >
        struct find_lastF
        {
            typedef typename SearchTraits::range_type range_type;
            typedef typename SearchTraits::compare_function_type compare_function_type;

            // Operation
            range_type operator()(        
                InputIterator Begin, 
                InputIterator End, 
                SubstrIterator SubBegin,
                SubstrIterator SubEnd   )
            {
                // Instantiate comp from search traits
                compare_function_type Comp;
                
                // Outer loop
                for(InputIterator OuterIt=End;
                    OuterIt!=Begin; )
                {
                    InputIterator OuterIt2=--OuterIt;

                    InputIterator InnerIt=OuterIt2;
                    SubstrIterator SubstrIt=SubBegin;
                    for(;
                        InnerIt!=End && SubstrIt!=SubEnd;
                        InnerIt++,SubstrIt++)
                    {
                        if( !Comp(*InnerIt,*SubstrIt) )
                            break;
                    }

                    // Substring matching succeeded
                    if ( SubstrIt==SubEnd )
                        return range_type( OuterIt2, InnerIt );
                }

                return range_type( End, End );
            }
        };
    } // namespace string_algo

//  Generic find functions -------------------------------------//

    template< typename InputIterator, typename SubIterator, typename FindF >
    inline typename string_algo::search_traits<InputIterator,SubIterator>::range_type 
    find( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd,
        FindF Find)
    {
        // Forward the call to the functor
        return Find( Begin, End, SubBegin, SubEnd );
    }

    // find sequence const version
    template< typename SeqT1, typename SeqT2, typename FindF >
    inline typename string_algo::search_traits<
        typename SeqT1::const_iterator, 
        typename SeqT2::const_iterator>::range_type 
    find( 
        const SeqT1& Input, 
        const SeqT2& Substr,
        FindF Find)
    {
        return Find( Input.begin(), Input.end(), Substr.begin(), Substr.end() );
    }

    // find sequence non-const version
    template< typename SeqT1, typename SeqT2, typename FindF >
    inline std::pair<typename SeqT1::iterator, typename SeqT1::iterator> 
    BOOST_STRING_NON_CONST_FUNCTION(find)( 
        SeqT1& Input, 
        const SeqT2& Substr,
        FindF Find )
    {
        return Find( Input.begin(), Input.end(), Substr.begin(), Substr.end() );
    }


//  find_first  -----------------------------------------------//

    // find a subsequnce in the sequence
    template< typename InputIterator, typename SubIterator >
    inline typename string_algo::search_traits<InputIterator,SubIterator>::range_type 
    find_first( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd )
    {
        // Forward the call to the functor
        return find( 
            Begin, End, 
            SubBegin, SubEnd, 
            string_algo::find_firstF<InputIterator, SubIterator>() );
    }

    // find_first sequence const version
    template< typename SeqT1, typename SeqT2 >
    inline typename string_algo::search_traits<
        typename SeqT1::const_iterator, 
        typename SeqT2::const_iterator>::range_type 
    find_first( 
        const SeqT1& Input, 
        const SeqT2& Substr )
    {
        return find( 
            Input.begin(), Input.end(), 
            Substr.begin(), Substr.end(), 
            string_algo::find_firstF<typename SeqT1::const_iterator, typename SeqT2::const_iterator>() );
    }

    // find_first sequence non-const version
    template< typename SeqT1, typename SeqT2 >
    inline std::pair<typename SeqT1::iterator, typename SeqT1::iterator> 
    BOOST_STRING_NON_CONST_FUNCTION(find_first)( 
        SeqT1& Input, 
        const SeqT2& Substr )
    {
        return find( 
            Input.begin(), Input.end(), 
            Substr.begin(), Substr.end(), 
            string_algo::find_firstF<typename SeqT1::iterator, typename SeqT2::const_iterator>() );
    }

//  find_last  -----------------------------------------------//

    // find the last match a subsequnce in the sequence
    template< typename InputIterator, typename SubIterator >
    inline typename string_algo::search_traits<InputIterator,SubIterator>::range_type 
    find_last( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd )
    {
        // Forward the call to the functor
        return find( 
            Begin, End, 
            SubBegin, SubEnd, 
            string_algo::find_lastF<InputIterator, SubIterator>() );
    }

    // find_last sequence const version
    template< typename SeqT1, typename SeqT2 >
    inline typename string_algo::search_traits<
        typename SeqT1::const_iterator, 
        typename SeqT2::const_iterator>::range_type 
    find_last( 
        const SeqT1& Input, 
        const SeqT2& Substr )
    {
        return find( 
            Input.begin(), Input.end(), 
            Substr.begin(), Substr.end(), 
            string_algo::find_lastF<typename SeqT1::const_iterator, typename SeqT2::const_iterator>() );
    }

    // find_last sequence non-const version
    template< typename SeqT1, typename SeqT2 >
    inline std::pair<typename SeqT1::iterator, typename SeqT1::iterator> 
    BOOST_STRING_NON_CONST_FUNCTION(find_last)( 
        SeqT1& Input, 
        const SeqT2& Substr )
    {
        return find( 
            Input.begin(), Input.end(), 
            Substr.begin(), Substr.end(), 
            string_algo::find_lastF<typename SeqT1::iterator, typename SeqT2::const_iterator>() );
    }

//  find_nth ----------------------------------------------------------------------//

    // find the n-th match of a subsequnce in the sequence
    template< typename InputIterator, typename SubIterator >
    inline typename string_algo::search_traits<InputIterator,SubIterator>::range_type 
    find_nth( 
        InputIterator Begin, 
        InputIterator End, 
        SubIterator SubBegin,
        SubIterator SubEnd,
        unsigned int Nth )
    {
        // Forward the call to the functor
        return find( 
            Begin, End, 
            SubBegin, SubEnd, 
            string_algo::find_nthF<InputIterator, SubIterator>(Nth) );
    }

    // find_nth sequence const version
    template< typename SeqT1, typename SeqT2 >
    inline typename string_algo::search_traits<
        typename SeqT1::const_iterator, 
        typename SeqT2::const_iterator>::range_type 
    find_nth( 
        const SeqT1& Input, 
        const SeqT2& Substr,
        unsigned int Nth )
    {
        return find( 
            Input.begin(), Input.end(), 
            Substr.begin(), Substr.end(), 
            string_algo::find_nthF<typename SeqT1::const_iterator, typename SeqT2::const_iterator>(Nth) );
    }

    // find_nth sequence non-const version
    template< typename SeqT1, typename SeqT2 >
    inline std::pair<typename SeqT1::iterator, typename SeqT1::iterator> 
    BOOST_STRING_NON_CONST_FUNCTION(find_nth)( 
        SeqT1& Input, 
        const SeqT2& Substr,
        unsigned int Nth )
    {
        return find( 
            Input.begin(), Input.end(), 
            Substr.begin(), Substr.end(), 
            string_algo::find_nthF<typename SeqT1::iterator, typename SeqT2::const_iterator>(Nth) );
    }

} // namespace boost


#endif  // BOOST_STRING_FIND_HPP

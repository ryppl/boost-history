//  Boost string_algo library find.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_DETAIL_HPP
#define BOOST_STRING_FIND_DETAIL_HPP

#include <boost/string_algo/config.hpp>
#include <boost/detail/iterator.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/input_policy.hpp>

namespace boost {

    namespace string_algo {

        namespace detail {

//  find range functor -----------------------------------------------//

            // find a range in the sequence ( functor )
            /*
                This functor actually does not perform any find operation.
                It always returns given iterator range as a result.
            */
            template< typename ForwardIteratorT >
            struct find_rangeF
            {
                typedef ForwardIteratorT input_iterator_type;
                typedef iterator_range<input_iterator_type> result_type;

                // Construction
                find_rangeF( 
                    input_iterator_type Begin, 
                    input_iterator_type End ) : m_Range(Begin, End) {}

                find_rangeF(const iterator_range<input_iterator_type>& Range) : 
                    m_Range(Range) {}

                // Operation
                result_type operator()( 
                    input_iterator_type, 
                    input_iterator_type ) const
                {
                    return m_Range;
                }

            private:
                iterator_range<input_iterator_type> m_Range;
            };

            // Construction helper
            template<typename InputT>
            inline find_rangeF<
                BOOST_STRING_DEDUCED_TYPENAME input_policy<InputT>::iterator_type>
            create_find_range( 
                InputT&, 
                const iterator_range<
                    BOOST_STRING_DEDUCED_TYPENAME 
                        input_policy<InputT>::iterator_type>& Range )
            {
                return find_rangeF<
                    BOOST_STRING_DEDUCED_TYPENAME 
                        input_policy<InputT>::iterator_type>(Range); 
            };
        
//  find first functor -----------------------------------------------//

            // find a subsequnce in the sequence ( functor )
            /*
                Returns a pair <begin,end> marking the subsequence in the sequence. 
                If the find fails, functor returns <End,End>
            */
            template< typename ForwardIteratorT, typename SearchT >
            struct find_firstF
            {
                typedef ForwardIteratorT input_iterator_type;
                typedef iterator_range<input_iterator_type> result_type;

                typedef SearchT search_type;
                typedef const SearchT& search_reference_type;
                typedef BOOST_STRING_DEDUCED_TYPENAME 
                    SearchT::const_iterator search_iterator_type;

                typedef iterator_range<input_iterator_type> result_type;

                // Construction
                find_firstF( search_reference_type Search ) : m_Search( Search ) {}

                // Operation
                result_type operator()( 
                    input_iterator_type Begin, 
                    input_iterator_type End ) const
                {
                    // Outer loop
                    for(input_iterator_type OuterIt=Begin;
                        OuterIt!=End;
                        OuterIt++)
                    {
                        input_iterator_type InnerIt=OuterIt;
                        search_iterator_type SubstrIt=m_Search.begin();
                        for(;
                            InnerIt!=End && SubstrIt!=m_Search.end();
                            InnerIt++,SubstrIt++)
                        {
                            if( !( (*InnerIt)==(*SubstrIt) ) ) 
                                break;
                        }

                        // Substring matching succeeded
                        if ( SubstrIt==m_Search.end() )
                            return result_type( OuterIt, InnerIt );
                    }

                    return result_type( End, End );
                }

            private:
                search_reference_type m_Search;
            };

            // Construction helper
            template<typename InputT, typename SearchT>
            inline find_firstF<
                BOOST_STRING_DEDUCED_TYPENAME input_policy<InputT>::iterator_type, SearchT>
            create_find_first(
                InputT&,
                const SearchT& Search )
            {
                return find_firstF<
                    BOOST_STRING_DEDUCED_TYPENAME 
                        input_policy<InputT>::iterator_type, SearchT>( Search );
            }

//  find last functor -----------------------------------------------//

            // find the last match a subsequnce in the sequence ( functor )
            /*
                Returns a pair <begin,end> marking the subsequence in the sequence. 
                If the find fails, returns <End,End>
            */
            template< typename ForwardIteratorT, typename SearchT >
            struct find_lastF
            {
                typedef ForwardIteratorT input_iterator_type;

                typedef SearchT search_type;
                typedef const SearchT& search_reference_type;
                typedef BOOST_STRING_DEDUCED_TYPENAME 
                    SearchT::const_iterator search_iterator_type;

                typedef find_firstF< input_iterator_type, search_type > find_first_type;
                typedef iterator_range<input_iterator_type> result_type;

                // Construction
                find_lastF( search_reference_type Search ) : m_Search( Search ) {}

                // Operation
                result_type operator()(                     
                    input_iterator_type Begin, 
                    input_iterator_type End  ) const
                {
                    typedef BOOST_STRING_DEDUCED_TYPENAME boost::detail::
                        iterator_traits<input_iterator_type>::iterator_category category;

                    return findit( Begin, End, category() );
                }   

            private:
                // forward iterator
                result_type findit( 
                    input_iterator_type Begin, 
                    input_iterator_type End, 
                    std::forward_iterator_tag ) const
                {
                    find_first_type find_first( m_Search );

                    result_type M=find_first( Begin, End );
                    result_type Last=M;

                    while( !M.empty() )
                    {
                        Last=M;
                        M=find_first( M.end(), End );
                    }

                    return Last;
                }

                // bidirectional iterator
                result_type findit( 
                    input_iterator_type Begin, 
                    input_iterator_type End, 
                    std::bidirectional_iterator_tag ) const
                {
                    // Outer loop
                    for(input_iterator_type OuterIt=End;
                        OuterIt!=Begin; )
                    {
                        input_iterator_type OuterIt2=--OuterIt;

                        input_iterator_type InnerIt=OuterIt2;
                        search_iterator_type SubstrIt=m_Search.begin();
                        for(;
                            InnerIt!=End && SubstrIt!=m_Search.end();
                            InnerIt++,SubstrIt++)
                        {
                            if( !( (*InnerIt)==(*SubstrIt) ) ) 
                                break;
                        }

                        // Substring matching succeeded
                        if ( SubstrIt==m_Search.end() )
                            return result_type( OuterIt2, InnerIt );
                    }

                    return result_type( End, End );
                }

            private:
                search_reference_type m_Search;
            };

            // Construction helper
            template<typename InputT, typename SearchT>
            inline find_lastF<
                BOOST_STRING_DEDUCED_TYPENAME input_policy<InputT>::iterator_type, SearchT>
            create_find_last(
                InputT&,
                const SearchT& Search )
            {
                return find_lastF<
                    BOOST_STRING_DEDUCED_TYPENAME 
                        input_policy<InputT>::iterator_type, SearchT>( Search );
            }

//  find n-th functor -----------------------------------------------//

            // find the n-th match of a subsequnce in the sequence ( functor )
            /*
                Returns a pair <begin,end> marking the subsequence in the sequence. 
                If the find fails, returns <End,End>
            */
            template< typename ForwardIteratorT, typename SearchT >
            struct find_nthF
            {
                typedef ForwardIteratorT input_iterator_type;

                typedef SearchT search_type;
                typedef const SearchT& search_reference_type;
                typedef BOOST_STRING_DEDUCED_TYPENAME 
                    SearchT::const_iterator search_iterator_type;

                typedef find_firstF< input_iterator_type, search_type> find_first_type;
                typedef BOOST_STRING_DEDUCED_TYPENAME 
                    find_first_type::result_type result_type;

                // Constructor
                find_nthF( search_reference_type Search, unsigned int Nth ) : 
                    m_Search( Search ), m_Nth( Nth ) {}

                // Operation
                result_type operator()( 
                    input_iterator_type Begin, 
                    input_iterator_type End ) const
                {
                    // Instantiate find funtor 
                    find_first_type find_first( m_Search );

                    result_type M( Begin, Begin );

                    for( unsigned int n=0; n<=m_Nth; n++ )
                    {
                        // find next match        
                        M=find_first( M.end(), End );

                        if ( M.empty() )
                        {
                            // Subsequence not found, return 
                            return M;
                        }
                    }

                    return M;
                }

            private:
                search_reference_type m_Search;
                unsigned int m_Nth;
            };

            // Construction helper
            template<typename InputT, typename SearchT>
            inline find_nthF<
                BOOST_STRING_DEDUCED_TYPENAME input_policy<InputT>::iterator_type, SearchT>
            create_find_nth(
                InputT&,
                const SearchT& Search,
                unsigned int Nth )
            {
                return find_nthF<
                    BOOST_STRING_DEDUCED_TYPENAME 
                        input_policy<InputT>::iterator_type, SearchT>( Search, Nth );
            }

//  find head functor -----------------------------------------------//

            // find a head in the sequence ( functor )
            /*
                This functor find a head of the specified range. For 
                a specified N, the head is a subsequence of N starting 
                elements of the range.
            */
            template< typename ForwardIteratorT >
            struct find_headF
            {
                typedef ForwardIteratorT input_iterator_type;
                typedef iterator_range<input_iterator_type> result_type;

                // Construction
                find_headF( unsigned int N ) : m_N(N) {}

                // Operation
                result_type operator()( 
                    input_iterator_type Begin, 
                    input_iterator_type End ) const
                {
                    typedef BOOST_STRING_DEDUCED_TYPENAME boost::detail::
                        iterator_traits<input_iterator_type>::iterator_category category;

                    return findit( Begin, End, category() );
                }

            private:
                // Find operation implementation
                result_type findit( 
                    input_iterator_type Begin,
                    input_iterator_type End,
                    std::forward_iterator_tag ) const
                {
                    input_iterator_type It=Begin;
                    for(
                        unsigned int Index=0; 
                        Index<m_N && It!=End; Index++,It++ );
    
                    return result_type( Begin, It );
                }

                result_type findit( 
                    input_iterator_type Begin,
                    input_iterator_type End,
                    std::random_access_iterator_tag ) const
                {
                    input_iterator_type It=Begin+m_N;
                    if ( It >= End ) It=End;

                    return result_type( Begin, It );
                }

            private:
                unsigned int m_N;
            };

            // Construction helper
            template<typename InputT>
            inline find_headF<
                BOOST_STRING_DEDUCED_TYPENAME input_policy<InputT>::iterator_type>
            create_find_head(
                InputT&,
                unsigned int N )
            {
                return find_headF<
                    BOOST_STRING_DEDUCED_TYPENAME input_policy<InputT>::iterator_type>(N);
            }

//  find tail functor -----------------------------------------------//

            // find a tail in the sequence ( functor )
            /*
                This functor find a tail of the specified range. For 
                a specified N, the head is a subsequence of N starting 
                elements of the range.
            */
            template< typename ForwardIteratorT >
            struct find_tailF
            {
                typedef ForwardIteratorT input_iterator_type;
                typedef iterator_range<input_iterator_type> result_type;

                // Construction
                find_tailF( unsigned int N ) : m_N(N) {}

                // Operation
                result_type operator()( 
                    input_iterator_type Begin, 
                    input_iterator_type End ) const
                {
                    typedef BOOST_STRING_DEDUCED_TYPENAME boost::detail::
                        iterator_traits<input_iterator_type>::iterator_category category;

                    return findit( Begin, End, category() );
                }

            private:
                // Find operation implementation
                result_type findit( 
                    input_iterator_type Begin,
                    input_iterator_type End,
                    std::forward_iterator_tag ) const
                {
                    unsigned int Index=0; 
                    input_iterator_type It=Begin;
                    input_iterator_type It2=Begin;
                    
                    // Advance It2 by N incremets
                    for( Index=0; Index<m_N && It2!=End; Index++,It2++ );

                    // Advance It, It2 to the end
                    for(; It2!=End; It++,It2++ );

                    return result_type( It, It2 );
                }

                result_type findit( 
                    input_iterator_type Begin,
                    input_iterator_type End,
                    std::bidirectional_iterator_tag ) const
                {
                    input_iterator_type It=End;
                    for(
                        unsigned int Index=0; 
                        Index<m_N && It!=Begin; Index++,It-- );
    
                    return result_type( It, End );
                }

                result_type findit( 
                    input_iterator_type Begin,
                    input_iterator_type End,
                    std::random_access_iterator_tag ) const
                {
                    if ( (End<=Begin) || (End < (Begin+m_N) ) ) 
                        return result_type( Begin, End );

                    return result_type( End-m_N, End );
                }


            private:
                unsigned int m_N;
            };

            // Construction helper
            template<typename InputT>
            inline find_tailF<
                BOOST_STRING_DEDUCED_TYPENAME input_policy<InputT>::iterator_type>
            create_find_tail(
                InputT&,
                unsigned int N )
            {
                return find_tailF<
                    BOOST_STRING_DEDUCED_TYPENAME input_policy<InputT>::iterator_type>(N);
            }

        } // namespace detail

    } // namespace string_algo

} // namespace boost

#endif  // BOOST_STRING_FIND_DETAIL_HPP

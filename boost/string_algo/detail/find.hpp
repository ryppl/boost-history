//  Boost string_algo library find.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_DETAIL_HPP
#define BOOST_STRING_FIND_DETAIL_HPP

#include <functional>

#include "../traits.hpp"

namespace boost {

    namespace string_algo {

        namespace detail {

//  find functor base -----------------------------------------------//

            // base for find functors
            /*
                defines types and provides no-match functionality
            */
            template< typename InputT, typename SearchT, typename SearchPolicy=search_policy >
            struct find_baseF
            {
                typedef search_traits<InputT, SearchT> search_traits;
                typedef typename SearchPolicy::input_policy<search_traits> input_policy;

                typedef typename search_traits::search_type search_type;
                typedef typename search_traits::search_const_iterator_type search_iterator_type;
                typedef typename search_traits::search_const_reference_type search_reference_type;

                typedef typename input_policy::input_type input_type;
                typedef typename input_policy::input_iterator_type input_iterator_type;
                typedef typename input_policy::input_reference_type input_reference_type;
                typedef typename input_policy::range_type range_type;
            };


//  generic find functors -----------------------------------------------//

            // find a subsequnce in the sequence ( functor )
            /*
                Returns a pair <begin,end> marking the subsequence in the sequence. It the find fails,
                returns <End,End>
            */
            template< typename InputT, typename SearchT, typename SearchPolicy=search_policy >
            struct find_firstF : find_baseF<InputT, SearchT, SearchPolicy>
            {
                typedef find_baseF<InputT, SearchT, SearchPolicy> base_type;

                typedef typename base_type::search_type search_type;
                typedef typename base_type::search_iterator_type search_iterator_type;
                typedef typename base_type::search_reference_type search_reference_type;
                typedef typename base_type::input_type input_type;
                typedef typename base_type::input_iterator_type input_iterator_type;
                typedef typename base_type::input_reference_type input_reference_type;
                typedef typename base_type::range_type range_type;

                // Operation
                range_type operator()(
                    input_reference_type Input, 
                    search_reference_type Search )
                {
                    return operator()( Input, Search, Input.begin() );
                }

                range_type operator()( 
                    input_reference_type Input, 
                    search_reference_type Search,
                    input_iterator_type From )
                {
                    // Outer loop
                    for(input_iterator_type OuterIt=From;
                        OuterIt!=Input.end();
                        OuterIt++)
                    {
                        input_iterator_type InnerIt=OuterIt;
                        search_iterator_type SubstrIt=Search.begin();
                        for(;
                            InnerIt!=Input.end() && SubstrIt!=Search.end();
                            InnerIt++,SubstrIt++)
                        {
                            if( !( (*InnerIt)==(*SubstrIt) ) ) 
                                break;
                        }

                        // Substring matching succeeded
                        if ( SubstrIt==Search.end() )
                            return range_type( OuterIt, InnerIt );
                    }

                    return range_type( Input.end(), Input.end() );
                }
            };

            // find_firstF const version
            template< typename InputT, typename SearchT, 
                typename SearchPolicy=const_input_search_policy >
            struct find_first_constF : public find_firstF< InputT, SearchT, SearchPolicy > {};

            // find_firstF iterator version
            template< 
                typename InputIteratorT, 
                typename SearchIteratorT, 
                typename SearchPolicy=const_input_search_policy >
            struct find_first_iterF : 
                public find_firstF< 
                    iterator_range<InputIteratorT>, 
                    iterator_range<SearchIteratorT>, 
                    SearchPolicy > {};

            // find the last match a subsequnce in the sequence ( functor )
            /*
            Returns a pair <begin,end> marking the subsequence in the sequence. If the find fails,
            returns <End,End>
            */
            template< typename InputT, typename SearchT, typename SearchPolicy=search_policy >
            struct find_lastF : find_baseF<InputT, SearchT, SearchPolicy>
            {
                typedef find_baseF<InputT, SearchT, SearchPolicy> base_type;

                typedef typename base_type::search_type search_type;
                typedef typename base_type::search_iterator_type search_iterator_type;
                typedef typename base_type::search_reference_type search_reference_type;
                typedef typename base_type::input_type input_type;
                typedef typename base_type::input_iterator_type input_iterator_type;
                typedef typename base_type::input_reference_type input_reference_type;
                typedef typename base_type::range_type range_type;

                // Operation
                range_type operator()(
                    input_reference_type Input, 
                    search_reference_type Search )
                {
                    // Outer loop
                    for(input_iterator_type OuterIt=Input.end();
                        OuterIt!=Input.begin(); )
                    {
                        input_iterator_type OuterIt2=--OuterIt;

                        input_iterator_type InnerIt=OuterIt2;
                        search_iterator_type SubstrIt=Search.begin();
                        for(;
                            InnerIt!=Input.end() && SubstrIt!=Search.end();
                            InnerIt++,SubstrIt++)
                        {
                            if( !( (*InnerIt)==(*SubstrIt) ) ) 
								break;
                        }

                        // Substring matching succeeded
                        if ( SubstrIt==Search.end() )
                            return range_type( OuterIt2, InnerIt );
                    }

                    return range_type( Input.end(), Input.end() );
                }            

            };

            // find_lastF const version
            template< typename InputT, typename SearchT, 
                typename SearchPolicy=const_input_search_policy >
            struct find_last_constF : public find_lastF< InputT, SearchT, SearchPolicy > {};

            // find_lastF iterator version
            template< 
                typename InputIteratorT, 
                typename SearchIteratorT, 
                typename SearchPolicy=const_input_search_policy >
            struct find_last_iterF : 
                public find_lastF< 
                    iterator_range<InputIteratorT>, 
                    iterator_range<SearchIteratorT>, 
                    SearchPolicy > {};

            // find the n-th match of a subsequnce in the sequence ( functor )
            /*
            Returns a pair <begin,end> marking the subsequence in the sequence. If the find fails,
            returns <End,End>
            */
            template< typename InputT, typename SearchT, typename SearchPolicy=search_policy >
            struct find_nthF : find_baseF<InputT, SearchT, SearchPolicy>
            {
                typedef find_baseF<InputT, SearchT, SearchPolicy> base_type;

                typedef typename base_type::search_type search_type;
                typedef typename base_type::search_iterator_type search_iterator_type;
                typedef typename base_type::search_reference_type search_reference_type;
                typedef typename base_type::input_type input_type;
                typedef typename base_type::input_iterator_type input_iterator_type;
                typedef typename base_type::input_reference_type input_reference_type;
                typedef typename base_type::range_type range_type;

                typedef find_firstF< input_type, search_type, SearchPolicy > find_firstF;

                // Constructor
                find_nthF( unsigned int Nth ) : m_Nth( Nth ) {}

                // Operation
                range_type operator()(
                    input_reference_type Input, 
                    search_reference_type Search )
                {
                    return operator()( Input, Search, Input.begin() );
                }

                range_type operator()( 
                    input_reference_type Input, 
                    search_reference_type Search,
                    input_iterator_type From )
                {
                    // Instantiate find funtor 
                    find_firstF find_first;

                    range_type M( From, From );

                    for( unsigned int n=0; n<=m_Nth; n++ )
                    {
                        // find next match        
                        M=find_first( Input, Search, M.end() );

                        if ( M.empty() )
                        {
                            // Subsequence not found, return 
                            return M;
                        }
                    }

                    return M;
                }

            private:
                unsigned int m_Nth;
            };

            // find_nthF const version
            template< typename InputT, typename SearchT, 
                typename SearchPolicy=const_input_search_policy >
            struct find_nth_constF : public find_nthF< InputT, SearchT, SearchPolicy >
            {
                // Constructor
                find_nth_constF( unsigned int Nth ) : 
                    find_nthF<InputT, SearchT, SearchPolicy>( Nth ) {}
            };

            // find_nthF iterator version
            template< 
                typename InputIteratorT, 
                typename SearchIteratorT, 
                typename SearchPolicy=const_input_search_policy >
            struct find_nth_iterF : 
                public find_nthF< 
                    iterator_range<InputIteratorT>, 
                    iterator_range<SearchIteratorT>, 
                    SearchPolicy >
            {
                // Constructor
                find_nth_iterF( unsigned int Nth ) : 
                    find_nthF< 
                        iterator_range<InputIteratorT>, 
                        iterator_range<SearchIteratorT>, 
                        SearchPolicy >( Nth ) {}
            };
        
        } // namespace detail

    } // namespace string_algo


} // namespace boost

#endif  // BOOST_STRING_FIND_DETAIL_HPP

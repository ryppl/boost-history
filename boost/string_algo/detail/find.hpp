//  Boost string_algo library find.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_DETAIL_HPP
#define BOOST_STRING_FIND_DETAIL_HPP

#include <boost/detail/iterator.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/traits.hpp>
#include <boost/string_algo/detail/find_base.hpp>

namespace boost {

    namespace string_algo {

        namespace detail {

//  find range functor -----------------------------------------------//

            // find a range in the sequence ( functor )
            /*
                This functor actually does not perform any find operation.
                It always returns given iterator range as a result.
            */
            template< typename InputT, typename InputPolicy=mutable_input_policy >
            struct find_rangeF : public find_baseF<InputT, InputPolicy>
            {
                typedef find_baseF<InputT, InputPolicy> base_type;

                typedef typename base_type::input_type input_type;
                typedef typename base_type::input_iterator_type input_iterator_type;
                typedef typename base_type::input_reference_type input_reference_type;

                typedef iterator_range<input_iterator_type> result_type;

                // Construction
                find_rangeF( 
                    input_iterator_type Begin, 
                    input_iterator_type End ) : m_Range(Begin, End) {}

                find_rangeF(const iterator_range<input_iterator_type>& Range) : 
                    m_Range(Range) {}

                // Operation
                result_type operator()( input_reference_type ) const
                {
                    return m_Range;
                }

            private:
                iterator_range<input_iterator_type> m_Range;
            };

            // Construction helper
            template< typename InputT > 
            struct create_find_rangeF 
            { 
                static find_rangeF<InputT, mutable_input_policy> 
                create(const iterator_range<typename InputT::iterator>& Range) 
                { 
                    return find_rangeF<InputT, mutable_input_policy>(Range); 
                } 
            
                static find_rangeF<InputT, const_input_policy> 
                create_const(const iterator_range<typename InputT::const_iterator>& Range) 
                { 
                    return find_rangeF<InputT, const_input_policy>(Range); 
                } 
            };

//  find first functor -----------------------------------------------//

            // find a subsequnce in the sequence ( functor )
            /*
                Returns a pair <begin,end> marking the subsequence in the sequence. 
                If the find fails, functor returns <End,End>
            */
            template< typename InputT, typename SearchT, typename InputPolicy=mutable_input_policy >
            struct find_firstF : public find_baseF<InputT, InputPolicy>
            {
                typedef find_baseF<InputT, InputPolicy> base_type;

                typedef typename base_type::input_type input_type;
                typedef typename base_type::input_iterator_type input_iterator_type;
                typedef typename base_type::input_reference_type input_reference_type;

                typedef SearchT search_type;
                typedef const SearchT& search_reference_type;
                typedef typename SearchT::const_iterator search_iterator_type;

                typedef iterator_range<input_iterator_type> result_type;

                // Construction
                find_firstF( search_reference_type Search ) : m_Search( Search ) {}

                // Operation
                result_type operator()( input_reference_type Input ) const
                {
                    return operator()( Input, Input.begin() );
                }

                result_type operator()( 
                    input_reference_type Input, 
                    input_iterator_type From ) const
                {
                    // Outer loop
                    for(input_iterator_type OuterIt=From;
                        OuterIt!=Input.end();
                        OuterIt++)
                    {
                        input_iterator_type InnerIt=OuterIt;
                        search_iterator_type SubstrIt=m_Search.begin();
                        for(;
                            InnerIt!=Input.end() && SubstrIt!=m_Search.end();
                            InnerIt++,SubstrIt++)
                        {
                            if( !( (*InnerIt)==(*SubstrIt) ) ) 
                                break;
                        }

                        // Substring matching succeeded
                        if ( SubstrIt==m_Search.end() )
                            return result_type( OuterIt, InnerIt );
                    }

                    return result_type( Input.end(), Input.end() );
                }

            private:
                search_reference_type m_Search;
            };

            // Construction helper
            template< typename InputT > 
            struct create_find_firstF 
            { 
                template< typename SearchT > 
                static 
                find_firstF<InputT, SearchT, mutable_input_policy> 
                create( const SearchT& Search ) 
                { 
                    return find_firstF<InputT, SearchT, mutable_input_policy>( Search ); 
                } 
            
                template< typename SearchT > 
                static 
                find_firstF<InputT, SearchT, const_input_policy> 
                create_const( const SearchT& Search ) 
                { 
                    return find_firstF<InputT, SearchT, const_input_policy>( Search ); 
                } 
            };

//  find last functor -----------------------------------------------//

            // find the last match a subsequnce in the sequence ( functor )
            /*
                Returns a pair <begin,end> marking the subsequence in the sequence. 
                If the find fails, returns <End,End>
            */
            template< typename InputT, typename SearchT, typename InputPolicy=mutable_input_policy >
            struct find_lastF : public find_baseF<InputT, InputPolicy>
            {
                typedef find_baseF<InputT, InputPolicy> base_type;

                typedef typename base_type::input_type input_type;
                typedef typename base_type::input_iterator_type input_iterator_type;
                typedef typename base_type::input_reference_type input_reference_type;

                typedef SearchT search_type;
                typedef const SearchT& search_reference_type;
                typedef typename SearchT::const_iterator search_iterator_type;

                typedef find_firstF< input_type, search_type, InputPolicy > find_first_type;
                typedef iterator_range<input_iterator_type> result_type;

                // Construction
                find_lastF( search_reference_type Search ) : m_Search( Search ) {}

                // Operation
                result_type operator()( input_reference_type Input ) const
                {
                    typedef typename boost::detail::
                        iterator_traits<input_iterator_type>::iterator_category category;

                    return findit( Input, category() );
                }   

            private:
                // forward iterator
                result_type findit( 
                    input_reference_type Input,
                    std::forward_iterator_tag ) const
                {
                    find_first_type find_first( m_Search );

                    result_type M=find_first( Input );
                    result_type Last=M;

                    while( !M.empty() )
                    {
                        Last=M;
                        M=find_first( Input, M.end() );
                    }

                    return Last;
                }

                // bidirectional iterator
                result_type findit( 
                    input_reference_type Input,
                    std::bidirectional_iterator_tag ) const
                {
                    // Outer loop
                    for(input_iterator_type OuterIt=Input.end();
                        OuterIt!=Input.begin(); )
                    {
                        input_iterator_type OuterIt2=--OuterIt;

                        input_iterator_type InnerIt=OuterIt2;
                        search_iterator_type SubstrIt=m_Search.begin();
                        for(;
                            InnerIt!=Input.end() && SubstrIt!=m_Search.end();
                            InnerIt++,SubstrIt++)
                        {
                            if( !( (*InnerIt)==(*SubstrIt) ) ) 
                                break;
                        }

                        // Substring matching succeeded
                        if ( SubstrIt==m_Search.end() )
                            return result_type( OuterIt2, InnerIt );
                    }

                    return result_type( Input.end(), Input.end() );
                }

            private:
                search_reference_type m_Search;
            };

            // Construction helper
            template< typename InputT > 
            struct create_find_lastF 
            { 
                template< typename SearchT > 
                    static 
                    find_lastF<InputT, SearchT, mutable_input_policy> 
                    create( const SearchT& Search ) 
                { 
                    return find_lastF<InputT, SearchT, mutable_input_policy>( Search ); 
                } 
            
                template< typename SearchT > 
                    static 
                    find_lastF<InputT, SearchT, const_input_policy> 
                    create_const( const SearchT& Search ) 
                { 
                    return find_lastF<InputT, SearchT, const_input_policy>( Search ); 
                } 
            };

//  find n-th functor -----------------------------------------------//

            // find the n-th match of a subsequnce in the sequence ( functor )
            /*
                Returns a pair <begin,end> marking the subsequence in the sequence. 
                If the find fails, returns <End,End>
            */
            template< typename InputT, typename SearchT, typename InputPolicy=mutable_input_policy >
            struct find_nthF : public find_baseF<InputT, InputPolicy>
            {
                typedef find_baseF<InputT, InputPolicy> base_type;

                typedef typename base_type::input_type input_type;
                typedef typename base_type::input_iterator_type input_iterator_type;
                typedef typename base_type::input_reference_type input_reference_type;

                typedef SearchT search_type;
                typedef const SearchT& search_reference_type;
                typedef typename SearchT::const_iterator search_iterator_type;

                typedef find_firstF< input_type, search_type, InputPolicy > find_first_type;
                typedef typename find_first_type::result_type result_type;

                // Constructor
                find_nthF( search_reference_type Search, unsigned int Nth ) : 
                    m_Search( Search ), m_Nth( Nth ) {}

                // Operation
                result_type operator()( input_reference_type Input ) const
                {
                    return operator()( Input, Input.begin() );
                }

                result_type operator()( 
                    input_reference_type Input, 
                    input_iterator_type From ) const
                {
                    // Instantiate find funtor 
                    find_first_type find_first( m_Search );

                    result_type M( From, From );

                    for( unsigned int n=0; n<=m_Nth; n++ )
                    {
                        // find next match        
                        M=find_first( Input, M.end() );

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
            template< typename InputT > 
            struct create_find_nthF 
            { 
                template< typename SearchT > 
                    static 
                    find_nthF<InputT, SearchT, mutable_input_policy> 
                    create( const SearchT& Search, unsigned int Nth ) 
                { 
                    return find_nthF<InputT, SearchT, mutable_input_policy>( Search, Nth ); 
                } 
            
                template< typename SearchT > 
                    static 
                    find_nthF<InputT, SearchT, const_input_policy> 
                    create_const( const SearchT& Search, unsigned int Nth ) 
                { 
                    return find_nthF<InputT, SearchT, const_input_policy>( Search, Nth ); 
                } 
            };

        } // namespace detail

    } // namespace string_algo

} // namespace boost

#endif  // BOOST_STRING_FIND_DETAIL_HPP

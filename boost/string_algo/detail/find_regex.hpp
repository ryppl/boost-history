//  Boost string_algo library find_regex.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_REGEX_DETAIL_HPP
#define BOOST_STRING_FIND_REGEX_DETAIL_HPP

#include <functional>
#include <boost/regex.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/find_base.hpp>


namespace boost {

    namespace string_algo {

        namespace detail {

//  regex find functor -----------------------------------------------//

            // regex search result
            template< typename InputIteratorT >
            struct regex_search_result : 
                public iterator_range< InputIteratorT >
            {
                typedef InputIteratorT iterator_type;
                typedef match_results<iterator_type> match_results_type;
                typedef iterator_range<iterator_type> range_type;

                // Contruction

                // Construction from the match result
                regex_search_result( const match_results_type& MatchResults ) :
                    range_type( MatchResults[0].first, MatchResults[0].second ),
                    m_MatchResults( MatchResults ) {}
                
                // Construction of empty match. End iterator has to be specified
                regex_search_result( InputIteratorT End ) :
                    range_type( End, End ) {}

                regex_search_result( const regex_search_result& Other ) :
                    range_type( Other.begin(), Other.end() ),
                    m_MatchResults( Other.m_MatchResults ) {}

                // Match result retrival
                const match_results_type& match_results() const
                {
                    return m_MatchResults;
                }

            private:
                // Saved matchresult
                match_results_type m_MatchResults;
            };

            // find_regex
            /*
                Regex based search functor
            */
            template< 
                typename InputT, 
                typename RegExT, 
                typename InputPolicy=mutable_input_policy >
            struct find_regexF : public find_baseF<InputT, InputPolicy>
            {
                typedef find_baseF<InputT, InputPolicy> base_type;

                typedef typename base_type::input_type input_type;
                typedef typename base_type::input_iterator_type input_iterator_type;
                typedef typename base_type::input_reference_type input_reference_type;

                typedef RegExT regex_type;
                typedef const RegExT& regex_reference_type;

                typedef regex_search_result< input_iterator_type > result_type;
                        
                // Construction
                find_regexF( regex_reference_type Rx, unsigned int MatchFlags = match_default ) : 
                    m_Rx(Rx), m_MatchFlags(MatchFlags) {}   

                // Operation
                result_type operator()( input_reference_type Input ) const
                {
                    return operator()( Input, Input.begin() );
                }

                result_type operator()( 
                    input_reference_type Input, 
                    input_iterator_type From ) const
                {
                    // instantiate match result
                    match_results<input_iterator_type> result;
                    // search for a match
                    if ( regex_search( From, Input.end(), result, m_Rx, m_MatchFlags ) )
                    {
                        // construct a result
                        return result_type( result );
                    }
                    else
                    {
                        // empty result
                        return result_type( Input.end() );
                    }
                }

            private:
                RegExT m_Rx;            // Regexp
                unsigned m_MatchFlags;  // match flags
            };

            // Construction helper
            template< typename InputT > 
            struct create_find_regexF 
            { 
                template< typename RegExT > 
                static 
                find_regexF<InputT, RegExT, mutable_input_policy> 
                create( const RegExT& Rx, unsigned int MatchFlags=match_default ) 
                { 
                    return find_regexF<InputT, RegExT, mutable_input_policy>( Rx, MatchFlags ); 
                } 
            
                template< typename RegExT > 
                static 
                find_regexF<InputT, RegExT, const_input_policy> 
                create_const( const RegExT& Rx, unsigned int MatchFlags=match_default ) 
                { 
                    return find_regexF<InputT, RegExT, const_input_policy>( Rx, MatchFlags ); 
                } 
            };

        
        } // namespace detail

    } // namespace string_algo

} // namespace boost

#endif  // BOOST_STRING_FIND_DETAIL_HPP

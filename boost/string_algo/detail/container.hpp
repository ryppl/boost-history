//  Boost string_algo library container.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_DETAIL_CONTAINER_HPP
#define BOOST_STRING_DETAIL_CONTAINER_HPP

#include <boost/mpl/bool_c.hpp>
#include <boost/string_algo/container_traits.hpp>

namespace boost {

    namespace string_algo {

        namespace detail {

//  insert helpers  -------------------------------------------------//
        
            template< typename InputT, typename InsertT >
            inline void insert(
                InputT& Input,
                typename InputT::iterator At,
                const InsertT& Insert )
            {
                insert( Input, At, Insert.begin(), Insert.end() );
            }

            template< typename InputT, typename InsertIteratorT >
            inline void insert(
                InputT& Input,
                typename InputT::iterator At,
                InsertIteratorT Begin,
                InsertIteratorT End )
            {
                Input.insert( At, Begin, End );
            }
            
//  erase helper  ---------------------------------------------------//

            // Erase a range in the sequence
            /*
                Returns the iterator pointing just after the erase subrange
            */
            template< typename InputT >
            inline typename InputT::iterator erase(
                InputT& Input,
                typename InputT::iterator From,
                typename InputT::iterator To )
            {
                return Input.erase( From, To );
            }

//  replace helper  -------------------------------------------------//
        
            template< typename InputT, typename InsertT >
            inline void replace(
                InputT& Input,
                typename InputT::iterator From,
                typename InputT::iterator To,
                const InsertT& Insert )
            {
                replace( Input, From, To, Insert.begin(), Insert.end() );
            }

            template< typename InputT, typename InsertIteratorT >
            inline void replace(
                InputT& Input,
                typename InputT::iterator From,
                typename InputT::iterator To,
                InsertIteratorT Begin,
                InsertIteratorT End )
            {
                replace_nat( 
                    Input, From, To, Begin, End, 
                    has_replace_select(container_traits_select(Input)) );
            }

//  replace helper implementation  ----------------------------------//

            // Container has native replace method
            template< typename InputT, typename InsertIteratorT >
            inline void replace_nat(
                InputT& Input,
                typename InputT::iterator From,
                typename InputT::iterator To,
                InsertIteratorT Begin,
                InsertIteratorT End,
                boost::mpl::true_c  )
            {
                Input.replace( From, To, Begin, End );
            }

            // No native replace method
            template< typename InputT, typename InsertIteratorT >
            inline void replace_nat(
                InputT& Input,
                typename InputT::iterator From,
                typename InputT::iterator To,
                InsertIteratorT Begin,
                InsertIteratorT End,
                boost::mpl::false_c ) 
            {
                replace_opt(
                    Input, From, To, Begin, End,
                    const_time_insert_select( container_traits_select(Input) ),
                    const_time_erase_select( container_traits_select(Input) ) );
            }
            
            // Const-time erase and insert methods -> use them
            template< typename InputT, typename InsertIteratorT >
            inline void replace_opt(
                InputT& Input,
                typename InputT::iterator From,
                typename InputT::iterator To,
                InsertIteratorT Begin,
                InsertIteratorT End,
                boost::mpl::true_c,
                boost::mpl::true_c ) 
            {
                typename InputT::iterator At=Input.erase( From, To );
                if ( Begin!=End )
                {
                    Input.insert( At, Begin, End );
                }
            }

            // Optimized version of replace for generic sequence containers
            // Assumption: insert and erase are expensive
            template< typename InputT, typename InsertIteratorT >
            inline void replace_opt(
                InputT& Input,
                typename InputT::iterator From,
                typename InputT::iterator To,
                InsertIteratorT Begin,
                InsertIteratorT End,
                ... )
            {
                // Copy data to the container ( as much as possible )
                InsertIteratorT InsertIt=Begin;
                typename InputT::iterator InputIt=From;
                for(; InsertIt!=End && InputIt!=To; InsertIt++, InputIt++ )
                {
                    *InputIt=*InsertIt;
                }
                
                if ( InsertIt!=End )
                {
                    // Replace sequence is longer, insert it
                    Input.insert( InputIt, InsertIt, End );
                }
                else
                {
                    if ( InputIt!=To )
                    {
                        // Replace sequence is shorter, erase the rest
                        Input.erase( InputIt, To );
                    }
                }
            }

        } // namespace detail

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_DETAIL_CONTAINER_HPP

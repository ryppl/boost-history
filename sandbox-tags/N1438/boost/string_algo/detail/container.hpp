//  Boost string_algo library container.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_DETAIL_CONTAINER_HPP
#define BOOST_STRING_DETAIL_CONTAINER_HPP

#include <boost/mpl/bool_c.hpp>
#include <boost/mpl/logical.hpp>
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

            template< typename InputT, typename ForwardIteratorT >
            inline void insert(
                InputT& Input,
                typename InputT::iterator At,
                ForwardIteratorT Begin,
                ForwardIteratorT End )
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

//  replace helper implementation  ----------------------------------//

            // Optimized version of replace for generic sequence containers
            // Assumption: insert and erase are expensive
            template< typename HasConstTimeOperations >
            struct replace_const_time_helper
            {
                template< typename InputT, typename ForwardIteratorT >
                void operator()(
                    InputT& Input,
                    typename InputT::iterator From,
                    typename InputT::iterator To,
                    ForwardIteratorT Begin,
                    ForwardIteratorT End )
                {
                    // Copy data to the container ( as much as possible )
                    ForwardIteratorT InsertIt=Begin;
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
            };

            template<>
            struct replace_const_time_helper< boost::mpl::true_c >
            {
                // Const-time erase and insert methods -> use them
                template< typename InputT, typename ForwardIteratorT >
                void operator()(
                    InputT& Input,
                    typename InputT::iterator From,
                    typename InputT::iterator To,
                    ForwardIteratorT Begin,
                    ForwardIteratorT End ) 
                {
                    typename InputT::iterator At=Input.erase( From, To );
                    if ( Begin!=End )
                    {
                        Input.insert( At, Begin, End );
                    }
                }
            };

            // No native replace method
            template< typename HasNative >
            struct replace_native_helper
            {
                template< typename InputT, typename ForwardIteratorT >
                void operator()(
                    InputT& Input,
                    typename InputT::iterator From,
                    typename InputT::iterator To,
                    ForwardIteratorT Begin,
                    ForwardIteratorT End ) 
                {
                    replace_const_time_helper< 
                        typename boost::mpl::logical_and<
                            typename container_traits<InputT>::const_time_insert,
                            typename container_traits<InputT>::const_time_erase >::type >()(
                        Input, From, To, Begin, End );
                }
            };

            // Container has native replace method
            template<>
            struct replace_native_helper< boost::mpl::true_c >
            {
                template< typename InputT, typename ForwardIteratorT >
                void operator()(
                    InputT& Input,
                    typename InputT::iterator From,
                    typename InputT::iterator To,
                    ForwardIteratorT Begin,
                    ForwardIteratorT End )
                {
                    Input.replace( From, To, Begin, End );
                }
            };

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

            template< typename InputT, typename ForwardIteratorT >
            inline void replace(
                InputT& Input,
                typename InputT::iterator From,
                typename InputT::iterator To,
                ForwardIteratorT Begin,
                ForwardIteratorT End )
            {
                replace_native_helper< 
                    typename container_traits<InputT>::native_replace >()(
                    Input, From, To, Begin, End );
            };

        } // namespace detail

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_DETAIL_CONTAINER_HPP

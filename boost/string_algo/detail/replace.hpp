//  Boost string_algo library replace.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_DETAIL_HPP
#define BOOST_STRING_REPLACE_DETAIL_HPP

#include <algorithm>
#include <boost/mpl/bool_c.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/detail/container.hpp>

namespace boost {

    namespace string_algo {
        
        namespace detail {

//  storage handling routines -----------------------------------------------//
            
            template< typename StorageT, typename OutputIteratorT >
            inline OutputIteratorT move_from_storage(
                StorageT& Storage,
                OutputIteratorT DestBegin,
                OutputIteratorT DestEnd )
            {
                OutputIteratorT OutputIt=DestBegin;
                
                while( !Storage.empty() && OutputIt!=DestEnd )
                {
                    *OutputIt=Storage.front();
                    Storage.pop_front();
                    OutputIt++;
                }

                return OutputIt;
            }

            template< typename StorageT, typename WhatT >
            inline void copy_to_storage(
                StorageT& Storage,
                const WhatT& What )
            {
                Storage.insert( Storage.end(), What.begin(), What.end() );
            }


//  process segment routine -----------------------------------------------//

            // Process one segment in the replace_all algorithm
            template< 
                typename StorageT,
                typename InputT,
                typename InputIteratorT >
            inline InputIteratorT process_segment(
                StorageT& Storage,
                InputT& Input,
                InputIteratorT InsertIt,
                InputIteratorT SegmentBegin,
                InputIteratorT SegmentEnd )
            {
                return process_segment_opt( 
                    Storage, Input, InsertIt, SegmentBegin, SegmentEnd,
                    stable_iterators_select( container_traits_select(Input) ) );
            }

            // Optimized version of process_segment for list-like sequence
            template< 
                typename StorageT,
                typename InputT,
                typename InputIteratorT >
            inline InputIteratorT process_segment_opt(
                StorageT& Storage,
                InputT& Input,
                InputIteratorT InsertIt,
                InputIteratorT SegmentBegin,
                InputIteratorT SegmentEnd,
                boost::mpl::true_c )
            {
                // Call replace to do the job
                replace( Input, InsertIt, SegmentBegin, Storage );
                // Empty the storage
                Storage.clear();
                // Iterators were not changed, simply return the end of segment
                return SegmentEnd;
            }

            // Optimized version of process_segment for generic sequence
            template< 
                typename StorageT,
                typename InputT,
                typename InputIteratorT >
            inline InputIteratorT process_segment_opt(
                StorageT& Storage,
                InputT& Input,
                InputIteratorT InsertIt,
                InputIteratorT SegmentBegin,
                InputIteratorT SegmentEnd,
                boost::mpl::false_c )
            {
                // Copy data from the storage until the beginning of the segment
                InputIteratorT It=move_from_storage( Storage, InsertIt, SegmentBegin );

                // 3 cases are possible :
                //   a) Storage is empty, It==SegmentBegin
                //   b) Storage is empty, It!=SegmentBegin
                //   c) Storage is not empty

                if( Storage.empty() )
                {
                    if( It==SegmentBegin )
                    {
                        // Case a) everything is grand, just return end of segment
                        return SegmentEnd;
                    }
                    else
                    {
                        // Case b) move the segment backwards
                        return std::copy( SegmentBegin, SegmentEnd, It );
                    }
                }
                else
                {
                    // Case c) -> shift the segment to the left and keep the overlap in the storage
                    while( It!=SegmentEnd )
                    {
                        // Store value into storage
                        Storage.push_back( *It );
                        // Get the top from the storage and put it here
                        *It=Storage.front();
                        Storage.pop_front();

                        // Advance
                        It++;
                    }

                    return It;
                }
            }
            

        } // namespace detail

    } // namespace string_algo

} // namespace boost

#endif  // BOOST_STRING_REPLACE_DETAIL_HPP

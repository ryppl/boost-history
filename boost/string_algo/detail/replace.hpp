//  Boost string_algo library replace.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_DETAIL_HPP
#define BOOST_STRING_REPLACE_DETAIL_HPP

#include <boost/string_algo/config.hpp>
#include <algorithm>
#include <boost/mpl/bool.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/sequence_traits.hpp>
#include <boost/string_algo/detail/sequence.hpp>
#include <boost/string_algo/iterator_range.hpp>

namespace boost {
    namespace string_algo {
        namespace detail {

//  temporary format and find result storage --------------------------------//

            template< 
                typename ForwardIteratorT,
                typename FormatT >
            class find_format_store : 
                public iterator_range<ForwardIteratorT>
            {
                typedef BOOST_STRING_TYPENAME
                    FormatT::result_type format_result_type;
            public:
                // Construction
                find_format_store( const FormatT& Format ) :
                    m_Format( Format ) {}

                // Assignment
                template< typename FindResultT >
                find_format_store& operator=( FindResultT FindResult )
                {
                    iterator_range<ForwardIteratorT>::operator=(FindResult);
                    m_FormatResult=m_Format(FindResult);
                    
                    return *this;
                }

                // Retrive format result
                const format_result_type& format_result()
                {   
                    return m_FormatResult;
                }

            private:
                const FormatT& m_Format;
                format_result_type m_FormatResult;
            };

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
                Storage.insert( Storage.end(), begin(What), end(What) );
            }


//  process segment routine -----------------------------------------------//

            template< typename HasStableIterators >
            struct process_segment_helper
            {
                // Optimized version of process_segment for generic sequence
                template< 
                    typename StorageT,
                    typename InputT,
                    typename ForwardIteratorT >
                ForwardIteratorT operator()(
                    StorageT& Storage,
                    InputT& Input,
                    ForwardIteratorT InsertIt,
                    ForwardIteratorT SegmentBegin,
                    ForwardIteratorT SegmentEnd )
                {
                    // Copy data from the storage until the beginning of the segment
                    ForwardIteratorT It=move_from_storage( Storage, InsertIt, SegmentBegin );

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
            };

            template<>
            struct process_segment_helper< boost::mpl::true_ >
            {
                // Optimized version of process_segment for list-like sequence
                template< 
                    typename StorageT,
                    typename InputT,
                    typename ForwardIteratorT >
                ForwardIteratorT operator()(
                    StorageT& Storage,
                    InputT& Input,
                    ForwardIteratorT InsertIt,
                    ForwardIteratorT SegmentBegin,
                    ForwardIteratorT SegmentEnd )
                {
                    // Call replace to do the job
                    replace( Input, InsertIt, SegmentBegin, Storage );
                    // Empty the storage
                    Storage.clear();
                    // Iterators were not changed, simply return the end of segment
                    return SegmentEnd;
                }
            };

            // Process one segment in the replace_all algorithm
            template< 
                typename StorageT,
                typename InputT,
                typename ForwardIteratorT >
            inline ForwardIteratorT process_segment(
                StorageT& Storage,
                InputT& Input,
                ForwardIteratorT InsertIt,
                ForwardIteratorT SegmentBegin,
                ForwardIteratorT SegmentEnd )
            {
                return 
                    process_segment_helper< 
                        BOOST_STRING_TYPENAME 
                            sequence_traits<InputT>::stable_iterators >()(
                                Storage, Input, InsertIt, SegmentBegin, SegmentEnd );
            }
            

        } // namespace detail
    } // namespace string_algo
} // namespace boost

#endif  // BOOST_STRING_REPLACE_DETAIL_HPP

//  Boost string_algo library predicate.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_PREDICATE_DETAIL_HPP
#define BOOST_STRING_PREDICATE_DETAIL_HPP

#include <iterator>
#include <boost/string_algo/find.hpp>

namespace boost {

    namespace string_algo {

        namespace detail {

//  ends_with predicate implementation ----------------------------------//

            template< typename InputIteratorT, typename SubIteratorT >
            inline bool ends_with_iter_select( 
                InputIteratorT Begin, 
                InputIteratorT End, 
                SubIteratorT SubBegin,
                SubIteratorT SubEnd,
                std::bidirectional_iterator_tag )
            {
                InputIteratorT it=End;
                SubIteratorT pit=SubEnd;
                for(;it!=Begin && pit!=SubBegin;)
                {
                    if( !( (*(--it))==(*(--pit)) ) )
                        return false;
                }

                return pit==SubBegin;
            }

            template< typename InputIteratorT, typename SubIteratorT >
            inline bool ends_with_iter_select( 
                InputIteratorT Begin, 
                InputIteratorT End, 
                SubIteratorT SubBegin,
                SubIteratorT SubEnd,
                std::forward_iterator_tag )
            {
                if ( SubBegin==SubEnd )
                {
                    // empty subsequence check
                    return true;
                }

                iterator_range<InputIteratorT> Result
                    =find_last( Begin, End, SubBegin, SubEnd );

                return !Result.empty() && Result.end()==End;
            }

        } // namespace detail

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_PREDICATE_DETAIL_HPP

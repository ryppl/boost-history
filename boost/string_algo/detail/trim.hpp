//  Boost string_algo library trim.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_TRIM_DETAIL_HPP
#define BOOST_STRING_TRIM_DETAIL_HPP

#include <boost/string_algo/config.hpp>
#include <boost/detail/iterator.hpp>

namespace boost {
    namespace string_algo {
        namespace detail {

//  trim iterator helper -----------------------------------------------//

            // Search for first non matching character from the beginning of the sequence
            template< typename ForwardIteratorT, typename PredicateT >
            inline ForwardIteratorT trim_begin( 
                ForwardIteratorT InBegin, 
                ForwardIteratorT InEnd, 
                PredicateT IsSpace )
            {
                ForwardIteratorT It=InBegin;
                for(; It!=InEnd; It++ )
                {
                    if (!IsSpace(*It))
                        return It;
                }

                return It;
            }

            // Search for first non matching character from the end of the sequence
            template< typename ForwardIteratorT, typename PredicateT >
            inline ForwardIteratorT trim_end( 
                ForwardIteratorT InBegin, 
                ForwardIteratorT InEnd, 
                PredicateT IsSpace )
            {
                typedef BOOST_STRING_TYPENAME boost::detail::
                    iterator_traits<ForwardIteratorT>::iterator_category category;

                return trim_end_iter_select( InBegin, InEnd, IsSpace, category() );
            }

            template< typename ForwardIteratorT, typename PredicateT >
            inline ForwardIteratorT trim_end_iter_select( 
                ForwardIteratorT InBegin, 
                ForwardIteratorT InEnd, 
                PredicateT IsSpace,
                std::forward_iterator_tag )
            {
                ForwardIteratorT TrimIt=InBegin;

                for( ForwardIteratorT It=InBegin; It!=InEnd; It++ )
                {
                    if ( !IsSpace(*It) ) 
                    {
                        TrimIt=It;
                        TrimIt++;
                    }
                }

                return TrimIt;
            }

            template< typename ForwardIteratorT, typename PredicateT >
            inline ForwardIteratorT trim_end_iter_select( 
                ForwardIteratorT InBegin, 
                ForwardIteratorT InEnd, 
                PredicateT IsSpace,
                std::bidirectional_iterator_tag )
            {
                for( ForwardIteratorT It=InEnd; It!=InBegin;  )
                {
                    if ( !IsSpace(*(--It)) )
                        return ++It;
                }

                return InBegin;
            }

        } // namespace detail
    } // namespace string_algo
} // namespace boost


#endif  // BOOST_STRING_TRIM_DETAIL_HPP

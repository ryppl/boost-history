//  Boost string_algo library find_impl.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND_IMPL_HPP
#define BOOST_STRING_FIND_IMPL_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/find.hpp>

namespace boost {
    namespace string_algo {

//  Finder generators ------------------------------------------//
        
        // first_finder
        /*
            Construct first_finder. The finders searches for the first
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.
        */
        template<typename ForwardIteratorT>
        inline detail::first_finderF<ForwardIteratorT>
        first_finder( 
            ForwardIteratorT SearchBegin, 
            ForwardIteratorT SearchEnd )
        {
            return detail::first_finderF<ForwardIteratorT>( SearchBegin, SearchEnd );
        }
        
        // first_finder
        /*
            Construct first_finder. The finders searches for the first
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.
        */
        template<typename ContainerT>
        inline detail::first_finderF<
            BOOST_STRING_TYPENAME container_traits<ContainerT>::const_iterator >
        first_finder( const ContainerT& Search )
        {
            return detail::first_finderF<
                BOOST_STRING_TYPENAME 
                    container_traits<ContainerT>::const_iterator>( Search );
        }

        // last_finder
        /*
            Construct last_finder. The finders searches for the last
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.
        */
        template<typename ForwardIteratorT>
        inline detail::last_finderF<ForwardIteratorT>
        last_finder( 
            ForwardIteratorT SearchBegin, 
            ForwardIteratorT SearchEnd )
        {
            return detail::last_finderF<ForwardIteratorT>( SearchBegin, SearchEnd );
        }
        
        // last_finder
        /*
            Construct last_finder. The finders searches for the last
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.
        */
        template<typename ContainerT>
        inline detail::last_finderF<
            BOOST_STRING_TYPENAME container_traits<ContainerT>::const_iterator >
        last_finder( const ContainerT& Search )
        {
            return detail::last_finderF<
                BOOST_STRING_TYPENAME 
                    container_traits<ContainerT>::const_iterator>( Search );
        }

        // nth_finder
        /*
            Construct nth_finder. The finders searches for the n-th
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.
        */
        template<typename ForwardIteratorT>
        inline detail::nth_finderF<ForwardIteratorT>
        nth_finder( 
            ForwardIteratorT SearchBegin, 
            ForwardIteratorT SearchEnd,
            unsigned int Nth )
        {
            return detail::nth_finderF<ForwardIteratorT>( SearchBegin, SearchEnd, Nth );
        }
        
        // nth_finder
        /*
            Construct nth_finder. The finders searches for the n-th
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.
        */
        template<typename ContainerT>
        inline detail::nth_finderF<
            BOOST_STRING_TYPENAME container_traits<ContainerT>::const_iterator >
        nth_finder( const ContainerT& Search, unsigned int Nth )
        {
            return detail::nth_finderF<
                BOOST_STRING_TYPENAME 
                    container_traits<ContainerT>::const_iterator>( Search, Nth );
        }

        // head_finder
        /*
            Construct head_finder. The finder returns a head of a given
            input. Head is a prefix of a sequene up to n elemets in
            size. It an input has less then n elements, whole input is 
            considered a head.
        */
        inline detail::head_finderF 
        head_finder( unsigned int N )
        {
            return detail::head_finderF( N );
        }
        
        // tail_finder
        /*
            Construct tail_finder. The finder returns a tail of a given
            input. Tail is a suffix of the sequene up to n elemets in
            size. It an input has less then n elements, whole input is 
            considered a head.
        */
        inline detail::tail_finderF 
        tail_finder( unsigned int N )
        {
            return detail::tail_finderF( N );
        }

        // token finder
        /*
            Construct token_finder. The finder searches for a token 
            specified by a predicate. It is similar to std::find_if 
            algorith, with an exception that it return a range of
            instead of a single iterator.

            If bCompress is set to true, adjacent matching tokens are 
            concatenated into one match. Thus the finder can be used to 
            search for continous segments of elements satisfying the 
            given predicate.
        */
        template< typename PredicateT >
        inline detail::token_finderF<PredicateT>
        token_finder( PredicateT Pred, bool bCompress=true )
        {
            return detail::token_finderF<PredicateT>( Pred, bCompress );
        }

        // range finder
        /*
            Construct range_finder. The finder does not perform 
            any operation. It simply returns the given range for 
            any input. 
        */
        template< typename ForwardIteratorT >
        inline detail::range_finderF<ForwardIteratorT>
        range_finder(
            ForwardIteratorT Begin,
            ForwardIteratorT End )
        {
            return detail::range_finderF<ForwardIteratorT>( Begin, End );
        }

        // range finder
        /*
            Construct range_finder. The finder does not perform 
            any operation. It simply returns the given range for 
            any input. 
        */
        template< typename ForwardIteratorT >
        inline detail::range_finderF<ForwardIteratorT>
        range_finder( iterator_range<ForwardIteratorT> Range )
        {
            return detail::range_finderF<ForwardIteratorT>( Range );
        }

    } // namespace string_algo
} // namespace boost


#endif  // BOOST_STRING_FIND_IMPL_HPP

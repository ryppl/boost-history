//  Boost string_algo library find2.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND2_HPP
#define BOOST_STRING_FIND2_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/find.hpp>
#include <boost/string_algo/compare.hpp>

/*! \file
	Defines finder generators. Finder is a functor which is able to 
	find a subsequence matching a specific criteria in an input
	sequence. 
	Finders are used by as a plugable components for replace, find 
	and split facilities. This header contains generator functions 
	for finders provided in this library.
*/

namespace boost {
    namespace string_algo {

//  Generic find -----------------------------------------------//

	//! Generic find algorithm
	/*!
		Search the input using the given finder.

		\param Input A container which will be searched.
		\param FindF Finder used for searching.
		\return 
			An iterator_range containing iterators delimiting the match. 
			Returned iterator is either InputContainerT::iterator or 
			InputContainerT::const_iterator, depending on the constness of 
			the input parameter.
	*/
	template< typename InputContainerT, typename FindFT >
    inline iterator_range< 
        BOOST_STRING_TYPENAME string_algo::container_result_iterator<InputContainerT>::type >
    find( 
        InputContainerT& Input, 
		FindFT FindF )
    {
		return FindF(begin(Input),end(Input));
    }

//  Finder generators ------------------------------------------//
        
        //! "First" finder 
        /*!
            Construct first_finder. The finders searches for the first
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

			\param Search A seqeunce to be searched for.
			\return first_finder functor
        */
        template<typename ContainerT>
        inline detail::first_finderF<
            BOOST_STRING_TYPENAME container_const_iterator<ContainerT>::type,
            is_equal>
        first_finder( const ContainerT& Search )
        {
            return 
				detail::first_finderF<
					BOOST_STRING_TYPENAME 
		                container_const_iterator<ContainerT>::type,
					is_equal>( Search, is_equal() ) ;
        }

        //! "First" finder
        /*!
            Construct first_finder. The finders searches for the first
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

            Elements are compared using given predicate.

			\param Search A seqeunce to be searched for.
			\param Comp An element comparison predicate
			\return first_finder functor
        */
        template<typename ContainerT,typename PredicateT>
        inline detail::first_finderF<
            BOOST_STRING_TYPENAME container_const_iterator<ContainerT>::type,
            PredicateT>
        first_finder( 
            const ContainerT& Search, PredicateT Comp )
        {
            return 
				detail::first_finderF<
					BOOST_STRING_TYPENAME 
						container_const_iterator<ContainerT>::type,
					PredicateT>( Search, Comp );
        }

        //! "Last" finder
        /*!
            Construct last_finder. The finders searches for the last
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

			\param Search A seqeunce to be searched for.
			\return last_finder functor
        */
        template<typename ContainerT>
        inline detail::last_finderF<
            BOOST_STRING_TYPENAME container_const_iterator<ContainerT>::type,
            is_equal>
        last_finder( const ContainerT& Search )
        {
			return 
				detail::last_finderF<
					BOOST_STRING_TYPENAME 
						container_const_iterator<ContainerT>::type,
					is_equal>( Search, is_equal() );
        }
        //! "Last" finder
        /*!
            Construct last_finder. The finders searches for the last
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

            Elements are compared using given predicate.

			\param Search A seqeunce to be searched for.
			\param Comp An element comparison predicate
			\return last_finder functor
        */
        template<typename ContainerT, typename PredicateT>
        inline detail::last_finderF<
            BOOST_STRING_TYPENAME container_const_iterator<ContainerT>::type,
            PredicateT>
        last_finder( const ContainerT& Search, PredicateT Comp )
        {
            return 
				detail::last_finderF<
					BOOST_STRING_TYPENAME 
						container_const_iterator<ContainerT>::type,
					PredicateT>( Search, Comp ) ;
        }

        //! "Nth" finder
        /*!
            Construct nth_finder. The finders searches for the n-th
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

			\param Search A seqeunce to be searched for.
			\param Nth An index of the match to be find
			\return nth_finder functor
        */
        template<typename ContainerT>
        inline detail::nth_finderF<
            BOOST_STRING_TYPENAME container_const_iterator<ContainerT>::type,
            is_equal>
        nth_finder( 
            const ContainerT& Search, 
            unsigned int Nth)
        {
            return 
				detail::nth_finderF<
					BOOST_STRING_TYPENAME 
						container_const_iterator<ContainerT>::type,
					is_equal>( Search, Nth, is_equal() ) ;
        }
        //! "Nth" finder
        /*!
            Construct nth_finder. The finders searches for the n-th
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

            Elements are compared using given predicate.

			\param Search A seqeunce to be searched for.
			\param Nth An index of the match to be find
			\param Comp An element comparison predicate
			\return nth_finder functor
        */
        template<typename ContainerT, typename PredicateT>
        inline detail::nth_finderF<
            BOOST_STRING_TYPENAME container_const_iterator<ContainerT>::type,
            PredicateT>
        nth_finder( 
            const ContainerT& Search, 
            unsigned int Nth, 
            PredicateT Comp )
        {
            return 
				detail::nth_finderF<
					BOOST_STRING_TYPENAME 
	                    container_const_iterator<ContainerT>::type,
					PredicateT>( Search, Nth, Comp );
        }

        //! "Head" finder
        /*!
            Construct head_finder. The finder returns a head of a given
            input. Head is a prefix of a sequene up to n elemets in
            size. It an input has less then n elements, whole input is 
            considered a head.

			\param N The size of the head
			\return head_finder functor
        */
        inline detail::head_finderF
        head_finder( unsigned int N )
        {
            return detail::head_finderF(N);
        }
        
        //! "Tail" finder
        /*!
            Construct tail_finder. The finder returns a tail of a given
            input. Tail is a suffix of the sequene up to n elemets in
            size. It an input has less then n elements, whole input is 
            considered a head.

			\param N The size of the head
			\return tail_finder functor
        */
        inline detail::tail_finderF
        tail_finder( unsigned int N )
        {
            return detail::tail_finderF(N);
        }

        //! "Token" finder
        /*!
            Construct token_finder. The finder searches for a token 
            specified by a predicate. It is similar to std::find_if 
            algorith, with an exception that it return a range of
            instead of a single iterator.

            If bCompress is set to true, adjacent matching tokens are 
            concatenated into one match. Thus the finder can be used to 
            search for continous segments of elements satisfying the 
            given predicate.

			\param Pred An element selection predicate
			\param bCompress Compress flag
			\return token_finder functor
        */
        template< typename PredicateT >
        inline detail::token_finderF<PredicateT>
        token_finder( PredicateT Pred, bool bCompress=true )
        {
            return detail::token_finderF<PredicateT>( Pred, bCompress );
        }

        //! "Range" finder
        /*!
            Construct range_finder. The finder does not perform 
            any operation. It simply returns the given range for 
            any input. 

			\param Begin Beginning of the range
			\param End End of the range
			\return range_finger functor
        */
        template< typename ForwardIteratorT >
        inline detail::range_finderF<ForwardIteratorT>
        range_finder(
            ForwardIteratorT Begin,
            ForwardIteratorT End )
        {
            return detail::range_finderF<ForwardIteratorT>( Begin, End );
        }

        //! "Range" finder
        /*!
            Construct range_finder. The finder does not perform 
            any operation. It simply returns the given range for 
            any input. 

			\param Range The range.
			\return range_finger functor
        */
        template< typename ForwardIteratorT >
        inline detail::range_finderF<ForwardIteratorT>
        range_finder( iterator_range<ForwardIteratorT> Range )
        {
            return detail::range_finderF<ForwardIteratorT>( Range );
        }

    } // namespace string_algo
} // namespace boost


#endif  // BOOST_STRING_FIND2_HPP
//  Boost string_algo library find2.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FIND2_HPP
#define BOOST_STRING_FIND2_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/find.hpp>
#include <boost/string_algo/compare.hpp>

/*! \file
	Defines finder generators. Finder is a functor which is able to 
	find a subsequence matching a specific criteria in an input
	sequence. 
	Finders are used by as a plugable components for replace, find 
	and split facilities. This header contains generator functions 
	for finders provided in this library.
*/

namespace boost {
    namespace string_algo {

//  Finder generators ------------------------------------------//
        
        //! "First" finder 
        /*!
            Construct first_finder. The finders searches for the first
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

			\param Search A seqeunce to be searched for.
			\return first_finder functor
        */
        template<typename ContainerT>
        inline detail::first_finderF<
            BOOST_STRING_TYPENAME container_const_iterator<ContainerT>::type,
            is_equal>
        first_finder( const ContainerT& Search )
        {
            return 
				detail::first_finderF<
					BOOST_STRING_TYPENAME 
		                container_const_iterator<ContainerT>::type,
					is_equal>( Search, is_equal() ) ;
        }

        //! "First" finder
        /*!
            Construct first_finder. The finders searches for the first
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

            Elements are compared using given predicate.

			\param Search A seqeunce to be searched for.
			\param Comp An element comparison predicate
			\return first_finder functor
        */
        template<typename ContainerT,typename PredicateT>
        inline detail::first_finderF<
            BOOST_STRING_TYPENAME container_const_iterator<ContainerT>::type,
            PredicateT>
        first_finder( 
            const ContainerT& Search, PredicateT Comp )
        {
            return 
				detail::first_finderF<
					BOOST_STRING_TYPENAME 
						container_const_iterator<ContainerT>::type,
					PredicateT>( Search, Comp );
        }

        //! "Last" finder
        /*!
            Construct last_finder. The finders searches for the last
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

			\param Search A seqeunce to be searched for.
			\return last_finder functor
        */
        template<typename ContainerT>
        inline detail::last_finderF<
            BOOST_STRING_TYPENAME container_const_iterator<ContainerT>::type,
            is_equal>
        last_finder( const ContainerT& Search )
        {
			return 
				detail::last_finderF<
					BOOST_STRING_TYPENAME 
						container_const_iterator<ContainerT>::type,
					is_equal>( Search, is_equal() );
        }
        //! "Last" finder
        /*!
            Construct last_finder. The finders searches for the last
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

            Elements are compared using given predicate.

			\param Search A seqeunce to be searched for.
			\param Comp An element comparison predicate
			\return last_finder functor
        */
        template<typename ContainerT, typename PredicateT>
        inline detail::last_finderF<
            BOOST_STRING_TYPENAME container_const_iterator<ContainerT>::type,
            PredicateT>
        last_finder( const ContainerT& Search, PredicateT Comp )
        {
            return 
				detail::last_finderF<
					BOOST_STRING_TYPENAME 
						container_const_iterator<ContainerT>::type,
					PredicateT>( Search, Comp ) ;
        }

        //! "Nth" finder
        /*!
            Construct nth_finder. The finders searches for the n-th
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

			\param Search A seqeunce to be searched for.
			\param Nth An index of the match to be find
			\return nth_finder functor
        */
        template<typename ContainerT>
        inline detail::nth_finderF<
            BOOST_STRING_TYPENAME container_const_iterator<ContainerT>::type,
            is_equal>
        nth_finder( 
            const ContainerT& Search, 
            unsigned int Nth)
        {
            return 
				detail::nth_finderF<
					BOOST_STRING_TYPENAME 
						container_const_iterator<ContainerT>::type,
					is_equal>( Search, Nth, is_equal() ) ;
        }
        //! "Nth" finder
        /*!
            Construct nth_finder. The finders searches for the n-th
            occurence of the search sequence in a given input.
            Result is given as a range of iterator delimiting the match.

            Elements are compared using given predicate.

			\param Search A seqeunce to be searched for.
			\param Nth An index of the match to be find
			\param Comp An element comparison predicate
			\return nth_finder functor
        */
        template<typename ContainerT, typename PredicateT>
        inline detail::nth_finderF<
            BOOST_STRING_TYPENAME container_const_iterator<ContainerT>::type,
            PredicateT>
        nth_finder( 
            const ContainerT& Search, 
            unsigned int Nth, 
            PredicateT Comp )
        {
            return 
				detail::nth_finderF<
					BOOST_STRING_TYPENAME 
	                    container_const_iterator<ContainerT>::type,
					PredicateT>( Search, Nth, Comp );
        }

        //! "Head" finder
        /*!
            Construct head_finder. The finder returns a head of a given
            input. Head is a prefix of a sequene up to n elemets in
            size. It an input has less then n elements, whole input is 
            considered a head.

			\param N The size of the head
			\return head_finder functor
        */
        inline detail::head_finderF
        head_finder( unsigned int N )
        {
            return detail::head_finderF(N);
        }
        
        //! "Tail" finder
        /*!
            Construct tail_finder. The finder returns a tail of a given
            input. Tail is a suffix of the sequene up to n elemets in
            size. It an input has less then n elements, whole input is 
            considered a head.

			\param N The size of the head
			\return tail_finder functor
        */
        inline detail::tail_finderF
        tail_finder( unsigned int N )
        {
            return detail::tail_finderF(N);
        }

        //! "Token" finder
        /*!
            Construct token_finder. The finder searches for a token 
            specified by a predicate. It is similar to std::find_if 
            algorith, with an exception that it return a range of
            instead of a single iterator.

            If bCompress is set to true, adjacent matching tokens are 
            concatenated into one match. Thus the finder can be used to 
            search for continous segments of elements satisfying the 
            given predicate.

			\param Pred An element selection predicate
			\param bCompress Compress flag
			\return token_finder functor
        */
        template< typename PredicateT >
        inline detail::token_finderF<PredicateT>
        token_finder( PredicateT Pred, bool bCompress=true )
        {
            return detail::token_finderF<PredicateT>( Pred, bCompress );
        }

        //! "Range" finder
        /*!
            Construct range_finder. The finder does not perform 
            any operation. It simply returns the given range for 
            any input. 

			\param Begin Beginning of the range
			\param End End of the range
			\return range_finger functor
        */
        template< typename ForwardIteratorT >
        inline detail::range_finderF<ForwardIteratorT>
        range_finder(
            ForwardIteratorT Begin,
            ForwardIteratorT End )
        {
            return detail::range_finderF<ForwardIteratorT>( Begin, End );
        }

        //! "Range" finder
        /*!
            Construct range_finder. The finder does not perform 
            any operation. It simply returns the given range for 
            any input. 

			\param Range The range.
			\return range_finger functor
        */
        template< typename ForwardIteratorT >
        inline detail::range_finderF<ForwardIteratorT>
        range_finder( iterator_range<ForwardIteratorT> Range )
        {
            return detail::range_finderF<ForwardIteratorT>( Range );
        }

    } // namespace string_algo
} // namespace boost


#endif  // BOOST_STRING_FIND2_HPP

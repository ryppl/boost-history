//  Boost string_algo library find_iterator.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_SPLIT2_HPP
#define BOOST_STRING_SPLIT2_HPP

#include <boost/string_algo/config.hpp>
#include <algorithm>
#include <iterator>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/concept.hpp>
#include <boost/string_algo/detail/find_iterator.hpp>
#include <boost/string_algo/detail/util.hpp>

/*! \file
    Defines generic split algorithms. Split algorithms can be 
    used to divide a sequence into several part accorfing 
    to a given criteria. Result is given as a 'container 
    of containers' where elements are copies or references 
    to extracted parts.

    There are two algorithms provided. One iterated through matching
    substring, the other one through gasps between theese matches.
*/

namespace boost {
    namespace string_algo {

//  iterate find ---------------------------------------------------//

        //! Iter find algorithm
        /*!
            This algorithm executes a given finder in iteration on the input,
            until the end of input is reached, or no match is found.
            Iteration is done using built-in find_iterator, so the real 
            searching is performed only when needed.
            In each iteration new match is found and added to the result.

            \param Result A 'container container' to container the result of search.
                Both outher and inner container must have constructor taking a pair
                of iterators as an argument.
                Typical type of the result is 
                    std::vector< boost::iterator_range<iterator> >.
                ( each element of such a vector will container a range delimiting 
                a match )
            \param Input A container which will be searched.
            \param Finder A Finder object used for searching
            \return A reference the result

            \note Prior content of the result will be overriden.
        */
        template< 
            typename ResultT,
            typename InputT,
            typename FinderT >
        inline ResultT&
        iter_find(
            ResultT& Result,
            InputT& Input,
            FinderT Finder )
        {
            function_requires< 
                FinderConcept<FinderT,
                BOOST_STRING_TYPENAME container_result_iterator<InputT>::type> >();

            typedef BOOST_STRING_TYPENAME 
                container_result_iterator<InputT>::type input_iterator_type;
            typedef detail::find_iterator<
                input_iterator_type,
                FinderT > find_iterator_type;
            typedef detail::copy_rangeF<
                BOOST_STRING_TYPENAME 
                    container_value_type<ResultT>::type,
                input_iterator_type> copy_range_type;
            
            input_iterator_type InputEnd=end(Input);

            ResultT Tmp(
                make_transform_iterator( 
                    find_iterator_type( begin(Input), InputEnd, Finder ),
                    copy_range_type() ),
                make_transform_iterator( 
                    find_iterator_type( InputEnd, InputEnd, Finder ),
                    copy_range_type() ) );

            Result.swap(Tmp);
            return Result;
        };

//  iterate split ---------------------------------------------------//

        //! Split find algorithm
        /*!
            This algorithm executes a given finder in iteration on the input,
            until the end of input is reached, or no match is found.
            Iteration is done using built-in find_iterator, so the real 
            searching is performed only when needed.
            Each match is used as a separator of segments. These segments are then
            returned in the result.

            \param Result A 'container container' to container the result of search.
                Both outher and inner container must have constructor taking a pair
                of iterators as an argument.
                Typical type of the result is 
                    std::vector< boost::iterator_range<iterator> >.
                ( each element of such a vector will container a range delimiting 
                a match )
            \param Input A container which will be searched.
            \param Finder A finder object used for searching
            \return A reference the result

            \note Prior content of the result will be overriden.
        */
        template< 
            typename ResultT,
            typename InputT,
            typename FinderT >
        inline ResultT&
        iter_split(
            ResultT& Result,
            InputT& Input,
            FinderT Finder )
        {
            function_requires< 
                FinderConcept<FinderT,
                BOOST_STRING_TYPENAME container_result_iterator<InputT>::type> >();

            typedef BOOST_STRING_TYPENAME 
                container_result_iterator<InputT>::type input_iterator_type;
            typedef detail::find_iterator<
                input_iterator_type,
                FinderT,
                detail::split_find_policy > find_iterator_type;
            typedef detail::copy_rangeF<
                BOOST_STRING_TYPENAME 
                    container_value_type<ResultT>::type,
                input_iterator_type> copy_range_type;
            
            input_iterator_type InputEnd=end(Input);

            ResultT Tmp(
                make_transform_iterator( 
                    find_iterator_type( begin(Input), InputEnd, Finder ),
                    copy_range_type() ),
                make_transform_iterator( 
                    find_iterator_type( InputEnd, InputEnd, Finder ),
                    copy_range_type() ) );
            
            Result.swap(Tmp);
            return Result;
        };

    } // namespace string_algo
} // namespace boost


#endif  // BOOST_STRING_SPLIT2_HPP

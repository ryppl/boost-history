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
#include <boost/iterator_adaptors.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>
#include <boost/string_algo/detail/find_iterator.hpp>

namespace boost {
    namespace string_algo {

//  iterate find ---------------------------------------------------//

        // iter find
        /*
            
        */
        template< 
            typename ResultT,
            typename InputT,
            typename FindFT >
        inline ResultT&
        iter_find(
            ResultT& Result,
            const InputT& Input,
            FindFT FindF )
        {
            typedef BOOST_STRING_TYPENAME 
                container_traits<InputT>::const_iterator input_iterator_type;
            typedef detail::find_iterator<
                input_iterator_type,
                FindFT > find_iterator_type;
            typedef copy_rangeF<
                BOOST_STRING_TYPENAME 
                    container_traits<ResultT>::value_type,
                input_iterator_type> copy_range_type;
            
            input_iterator_type InputEnd=end(Input);

            ResultT Tmp(
                make_transform_iterator( 
                    find_iterator_type( begin(Input), InputEnd, FindF ),
                    copy_range_type() ),
                make_transform_iterator( 
                    find_iterator_type( InputEnd, InputEnd, FindF ),
                    copy_range_type() ) );

            Result.swap(Tmp);
            return Result;
        };

//  iterate split ---------------------------------------------------//

        // split find
        /*
            
        */
        template< 
            typename ResultT,
            typename InputT,
            typename FindFT >
        inline ResultT&
        iter_split(
            ResultT& Result,
            const InputT& Input,
            FindFT FindF )
        {
            typedef BOOST_STRING_TYPENAME 
                container_traits<InputT>::const_iterator input_iterator_type;
            typedef detail::find_iterator<
                input_iterator_type,
                FindFT,
                detail::split_find_policy > find_iterator_type;
            typedef copy_rangeF<
                BOOST_STRING_TYPENAME 
                    container_traits<ResultT>::value_type,
                input_iterator_type> copy_range_type;
            
            input_iterator_type InputEnd=end(Input);

            ResultT Tmp(
                make_transform_iterator( 
                    find_iterator_type( begin(Input), InputEnd, FindF ),
                    copy_range_type() ),
                make_transform_iterator( 
                    find_iterator_type( InputEnd, InputEnd, FindF ),
                    copy_range_type() ) );
            
            Result.swap(Tmp);
            return Result;
        };

    } // namespace string_algo
} // namespace boost


#endif  // BOOST_STRING_SPLIT2_HPP

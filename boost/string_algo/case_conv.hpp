//  Boost string_algo library case_conv.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_CASE_CONV_HPP
#define BOOST_STRING_CASE_CONV_HPP

#include <boost/string_algo/config.hpp>
#include <algorithm>
#include <locale>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/detail/case_conv.hpp>

namespace boost {

//  to_lower  -----------------------------------------------//

    // convert to lower case
    /*
        Each element of the input sequence is converted to lower
        case.
        Result is copied to the given output iterator.
    */
    template< typename OutputIteratorT, typename SeqT >
    inline OutputIteratorT 
    to_lower_copy(
        OutputIteratorT Output,
        const SeqT& Input,
        const std::locale& Loc=std::locale() )
    {
        return std::transform( 
            string_algo::begin(Input), 
            string_algo::end(Input), 
            Output,
            string_algo::detail::to_lowerF(Loc) );
    }

    // convert to lower case
    /*
        Each element of the input sequence is converted to lower
        case.
        Result is given as a converted copy of the input sequence.
    */
    template< typename SeqT >
    inline SeqT to_lower_copy( const SeqT& Input, const std::locale& Loc=std::locale() )
    {
        SeqT Output;
        std::transform( 
            string_algo::begin(Input), 
            string_algo::end(Input), 
            std::back_inserter<SeqT>( Output ),
            string_algo::detail::to_lowerF(Loc) );

        return Output;
    }

    // convert to lower case
    /*
        Each element of the input sequence is converted to lower
        case.
        Input sequence is modified in-place and the reference to it is as
        a result.
    */
    template< typename SeqT >
    inline SeqT& to_lower( SeqT& Input, const std::locale& Loc=std::locale() )
    {
        std::transform( 
            string_algo::begin(Input), 
            string_algo::end(Input), 
            string_algo::begin(Input), 
            string_algo::detail::to_lowerF(Loc) );
        
        return Input;
    }
    
//  to_upper  -----------------------------------------------//

    // convert to upper case
    /*
        Each element of the input sequence is converted to upper
        case.
        Result is copied to the given output iterator.
    */
    template< typename OutputIteratorT, typename SeqT >
    inline OutputIteratorT 
    to_upper_copy(
        OutputIteratorT Output,
        const SeqT& Input,
        const std::locale& Loc=std::locale() )
    {
        return std::transform( 
            string_algo::begin(Input), 
            string_algo::end(Input), 
            Output,
            string_algo::detail::to_upperF(Loc) );
    }

    // convert to upper case
    /*
        Each element of the input sequence is converted to upper
        case.
        Result is given as a converted copy of the input sequence.
    */
    template< typename SeqT >
    inline SeqT to_upper_copy( const SeqT& Input, const std::locale& Loc=std::locale() )
    {
        SeqT Output;
        std::transform( 
            string_algo::begin(Input), 
            string_algo::end(Input), 
            std::back_inserter<SeqT>( Output ),
            string_algo::detail::to_upperF(Loc) );

        return Output;
    }

    // convert to upper case
    /*
        Each element of the input sequence is converted to upper
        case.
        Input sequence is modified in-place and the reference to it is as
        a result.
    */
    template< typename SeqT >
    inline SeqT& to_upper( SeqT& Input, const std::locale& Loc=std::locale() )
    {
        std::transform( 
            string_algo::begin(Input), 
            string_algo::end(Input), 
            string_algo::begin(Input), 
            string_algo::detail::to_upperF(Loc) );
        
        return Input;
    }

} // namespace boost


#endif  // BOOST_STRING_CASE_CONV_HPP

//  Boost string_algo library string_conv.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_CONV_HPP
#define BOOST_STRING_CONV_HPP

#include <algorithm>
#include <functional>
#include <locale>

#include "string_funct.hpp"

namespace boost {

//  tolower  -----------------------------------------------//

    // const version of tolower
    template< typename Seq >
    inline Seq tolower_copy( const Seq& Input, const std::locale& Loc=std::locale() )
    {
        Seq Output;
        std::transform( 
            Input.begin(), 
            Input.end(), 
            std::back_inserter<Seq>( Output ),
            std::bind2nd( string_util_impl::tolowerF<typename Seq::value_type>(), Loc ) );

        return Output;
    }

    // in-place version of tolower
    template< typename Seq >
    inline Seq& tolower( Seq& Input, const std::locale& Loc=std::locale() )
    {
        std::transform( 
            Input.begin(), 
            Input.end(), 
            Input.begin(),
            std::bind2nd( string_util_impl::tolowerF<typename Seq::value_type>(), Loc ) );
        
        return Input;
    }
    
//  toupper  -----------------------------------------------//

    // const version of toupper
    template< typename Seq >
    inline Seq toupper_copy( const Seq& Input, const std::locale& Loc=std::locale() )
    {
        Seq Output;
        std::transform( 
            Input.begin(), 
            Input.end(), 
            std::back_inserter<Seq>( Output ),
            std::bind2nd( string_util_impl::toupperF<typename Seq::value_type>(), Loc ) );

        return Output;
    }

    // in-place version of toupper
    template< typename Seq >
    inline Seq& toupper( Seq& Input, const std::locale& Loc=std::locale() )
    {
        std::transform( 
            Input.begin(), 
            Input.end(), 
            Input.begin(),
            std::bind2nd( string_util_impl::toupperF<typename Seq::value_type>(), Loc ) );
        
        return Input;
    }
    
} // namespace boost


#endif  // BOOST_STRING_CONV_HPP

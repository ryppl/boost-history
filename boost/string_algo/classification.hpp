//  Boost string_algo library classification.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_CLASSIFICATION_HPP
#define BOOST_STRING_CLASSIFICATION_HPP

#include <algorithm>
#include <locale>
#include <boost/string_algo/detail/classification.hpp>

namespace boost {
    
//  classification functor generator -------------------------------------//

    // Construct an is_classified functor
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT>
    is_classified( std::ctype_base::mask Type, const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( Type, Loc );
    }

    // Construct an is_space functor
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_space( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::space, Loc );
    }

    // Construct an is_alnum functor
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_alnum( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::alnum, Loc );
    }

    // Construct an is_alpha functor
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_alpha( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::alpha, Loc );
    }

    // Construct an is_cntrl functor
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_cntrl( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::cntrl, Loc );
    }

    // Construct an is_digit functor
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_digit( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::digit, Loc );
    }

    // Construct an is_graph functor
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_graph( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::graph, Loc );
    }

    // Construct an is_lower functor
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_lower( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::lower, Loc );
    }

    // Construct an is_print functor
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_print( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::print, Loc );
    }

    // Construct an is_punct functor
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_punct( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::punct, Loc );
    }

    // Construct an is_upper functor
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_upper( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::upper, Loc );
    }

    // Construct an is_xdigit functor
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_xdigit( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::xdigit, Loc );
    }

    // Construct an is_of functor 
    template< typename CharT, typename SeqT >
    inline string_algo::detail::is_ofF<CharT> is_of( const SeqT& Seq )
    {
        return string_algo::detail::is_ofF<CharT>(Seq); 
    }

} // namespace boost


#endif  // BOOST_STRING_PREDICATE_HPP

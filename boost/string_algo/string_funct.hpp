//  Boost string_algo library string_funct.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_FUNCT_HPP
#define BOOST_STRING_FUNCT_HPP

#include <functional>
#include <locale>

namespace boost {

// -----------------------------------------------------------------------------
//  various utility functors used by string_algo functions as default arguments 

namespace string_util_impl {

    // isspace implementation
    /*
        this forward is provided here to allow non standard isspace implementations.
        f.e. non character base overrides
    */
    template< typename CharT >
    bool isspace_impl( CharT Ch, const std::locale& Loc )
    {
        return std::isspace( Ch, Loc );
    }

    // an isspace functor 
    template< typename CharT >
        struct isspaceF : public std::binary_function< CharT, std::locale, bool > 
    {
        typedef typename std::binary_function< CharT, std::locale, bool >::result_type result_type;
        typedef typename std::binary_function< CharT, std::locale, bool >::first_argument_type first_argument_type;
        typedef typename std::binary_function< CharT, std::locale, bool >::second_argument_type second_argument_type;

        result_type operator ()( first_argument_type Ch, const second_argument_type& Loc ) const
        {
            return isspace_impl( Ch, Loc );
        }
    };

    // a tolower functor 
    template< typename CharT >
        struct tolowerF : public std::binary_function< CharT, std::locale, CharT > 
    {
        typedef typename std::binary_function< CharT, std::locale, CharT >::result_type result_type;
        typedef typename std::binary_function< CharT, std::locale, CharT >::first_argument_type first_argument_type;
        typedef typename std::binary_function< CharT, std::locale, CharT >::second_argument_type second_argument_type;

        result_type operator ()( first_argument_type Ch, const second_argument_type& Loc ) const
        {
            return std::tolower( Ch, Loc );
        }
    };

    // a toupper functor 
    template< typename CharT >
        struct toupperF : public std::binary_function< CharT, std::locale, CharT > 
    {
        typedef typename std::binary_function< CharT, std::locale, CharT >::result_type result_type;
        typedef typename std::binary_function< CharT, std::locale, CharT >::first_argument_type first_argument_type;
        typedef typename std::binary_function< CharT, std::locale, CharT >::second_argument_type second_argument_type;

        result_type operator ()( first_argument_type Ch, const second_argument_type& Loc ) const
        {
            return std::toupper( Ch, Loc );
        }
    };

    // equal_to functor reimplementation
    /*
        standard STL implementation does handles only comparison between types
        of the same value. This is less restrictive version which wraps == operator.
    */
    template< typename Type1, typename Type2 >
        struct equal_toF : public std::binary_function< Type1, Type2, bool > 
    {
        typedef typename std::binary_function< Type1, Type2, bool >::result_type result_type;
        typedef typename std::binary_function< Type1, Type2, bool >::first_argument_type first_argument_type;
        typedef typename std::binary_function< Type1, Type2, bool >::second_argument_type second_argument_type;

        result_type operator ()( first_argument_type Ch, const second_argument_type& Loc ) const
        {
            return Ch==Loc;
        }
    };

} // namespace string_util_impl

} // namespace boost


#endif  // BOOST_STRING_FUNCT_HPP

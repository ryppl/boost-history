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

/*! \file
    Classification predicates are included in the library to give 
    some more convinience when using algorithms like \c trim() and \c all(). 
    In general they wrap funcionality of STL classification functions (e.g. \c std::isspace() )
    into generic functors. 
    
    All predicates support unary_function concept interface, so they can be used together 
    with stl logical predicates.
*/

namespace boost {
    
//  classification functor generator -------------------------------------//

    //! is_classified predicate
    /*!
        Construct is_classified predicate. This predicate holds, if an input is
        of specified ctype category.
        The predicate is compatible with AdaptableUnaryFunction. CharT has to
        be explititly specified.

        \param Type ctype category
        \param Loc A locale used for classification
        \return An instance of is_classified predicated 
    */
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT>
    is_classified( std::ctype_base::mask Type, const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( Type, Loc );
    }

    //! is_space predicate
    /*!
        Construct is_classified predicate for ctype_base::space category.   

        \param Loc A locale used for classification
        \return An instance of is_classified predicated 
    */
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_space( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::space, Loc );
    }

    //! is_alnum predicate
    /*!
        Construct is_classified predicate for ctype_base::alnum category.   

        \param Loc A locale used for classification
        \return An instance of is_classified predicated 
    */
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_alnum( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::alnum, Loc );
    }

    //! is_alpha predicate
    /*!
        Construct is_classified predicate for ctype_base::alpha category.   

        \param Loc A locale used for classification
        \return An instance of is_classified predicated 
    */
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_alpha( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::alpha, Loc );
    }

    //! is_cntrl predicate
    /*!
        Construct is_classified predicate for ctype_base::cntrl category.   

        \param Loc A locale used for classification
        \return An instance of is_classified predicated 
    */
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_cntrl( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::cntrl, Loc );
    }

    //! is_digit predicate
    /*!
        Construct is_classified predicate for ctype_base::digit category.   

        \param Loc A locale used for classification
        \return An instance of is_classified predicated 
    */
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_digit( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::digit, Loc );
    }

    //! is_graph predicate
    /*!
        Construct is_classified predicate for ctype_base::graph category.   

        \param Loc A locale used for classification
        \return An instance of is_classified predicated 
    */
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_graph( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::graph, Loc );
    }

    //! is_lower predicate
    /*!
        Construct is_classified predicate for ctype_base::lower category.   

        \param Loc A locale used for classification
        \return An instance of is_classified predicated 
    */
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_lower( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::lower, Loc );
    }

    //! is_print predicate
    /*!
        Construct is_classified predicate for ctype_base::print category.   

        \param Loc A locale used for classification
        \return An instance of is_classified predicated 
    */
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_print( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::print, Loc );
    }

    //! is_punct predicate
    /*!
        Construct is_classified predicate for ctype_base::punct category.   

        \param Loc A locale used for classification
        \return An instance of is_classified predicated 
    */
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_punct( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::punct, Loc );
    }

    //! is_upper predicate
    /*!
        Construct is_classified predicate for ctype_base::upper category.   

        \param Loc A locale used for classification
        \return An instance of is_classified predicated 
    */
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_upper( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::upper, Loc );
    }

    //! is_xdigit predicate
    /*!
        Construct is_classified predicate for ctype_base::xdigit category.  

        \param Loc A locale used for classification
        \return An instance of is_classified predicated 
    */
    template< typename CharT >
    inline string_algo::detail::is_classifiedF<CharT> 
    is_xdigit( const std::locale& Loc=std::locale() )
    {
        return string_algo::detail::is_classifiedF<CharT>( std::ctype_base::xdigit, Loc );
    }

    //! is_any_of predicate
    /*!
        Construct is_any_of predicate. The predicate holds, if an input
        is included in the specified set of characters.
        The predicate is compatible with AdaptableUnaryFunction. CharT has to
        be explititly specified.

        \param Set A set of characters to be recognized
        \return An instance of is_any_of predicated 
    */
    template< typename CharT, typename ContainerT >
    inline string_algo::detail::is_any_ofF<CharT> is_any_of( const ContainerT& Set )
    {
        return string_algo::detail::is_any_ofF<CharT>(Set); 
    }

} // namespace boost


#endif  // BOOST_STRING_PREDICATE_HPP

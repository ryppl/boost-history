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
//  various utility functors used by string_algo functions

    namespace detail {

        namespace string_algo {

            // isclassified functor
            /*
                This functor was suggested by Gennaro Prota
            */
            template< typename CharT >
            struct isclassifiedF : public std::unary_function< CharT, bool >
            {
                typedef typename std::unary_function< CharT, bool >::result_type result_type;
                typedef typename std::unary_function< CharT, bool >::argument_type argument_type;

                // Constructor from a ctype
                isclassifiedF(std::ctype_base::mask Type, std::ctype<CharT> & Ct) : 
                    m_Type(Type), m_CType(Ct) {}

                // Constructor from a locale 
                isclassifiedF(std::ctype_base::mask Type, std::locale const & Loc = std::locale())
                    : m_Type(Type), m_CType(std::use_facet< std::ctype<CharT> >(Loc)) {}

                // Operation
                result_type operator ()( argument_type Ch ) const
                {
                    return m_CType.is(m_Type, Ch);
                }

            private:
                const std::ctype<CharT>& m_CType;
                const std::ctype_base::mask m_Type;
            };

            // an isfrom functor 
            /* 
                returns true if the value is from specitied range
            */
            template< typename SeqT >
            struct isfromF : public std::unary_function< typename SeqT::value_type, bool > 
            {
                typedef typename SeqT::value_type CharT;
                typedef typename std::unary_function< CharT, bool >::result_type result_type;
                typedef typename std::unary_function< CharT, bool >::argument_type argument_type;

                // Constructor 
                isfromF( const SeqT& Seq ) : m_Seq( Seq ) {}
                
                // Operation
                result_type operator ()( argument_type Ch ) const
                {
                    return std::find( m_Seq.begin(), m_Seq.end(), Ch )!=m_Seq.end();
                }
            
            private:
                const SeqT& m_Seq;
            };

            // a tolower functor 
            template< typename CharT >
            struct tolowerF : public std::unary_function< CharT, CharT > 
            {
                typedef typename std::unary_function< CharT, CharT >::result_type result_type;
                typedef typename std::unary_function< CharT, CharT >::argument_type argument_type;

                // Constructor
                tolowerF( const std::locale& Loc ) : m_Loc( Loc ) {}

                // Operation
                result_type operator ()( argument_type Ch ) const
                {
                    return std::tolower( Ch, m_Loc );
                }
            private:
                const std::locale& m_Loc;
            };

            // a toupper functor 
            template< typename CharT >
            struct toupperF : public std::unary_function< CharT, CharT > 
            {
                typedef typename std::unary_function< CharT, CharT >::result_type result_type;
                typedef typename std::unary_function< CharT, CharT >::argument_type argument_type;

                // Constructor
                toupperF( const std::locale& Loc ) : m_Loc( Loc ) {}

                // Operation
                result_type operator ()( argument_type Ch ) const
                {
                    return std::toupper( Ch, m_Loc );
                }
            private:
                const std::locale& m_Loc;
            };

            // equal_to functor reimplementation
            /*
                standard STL implementation does handle only comparison between types
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

        } // namespace string_algo

    } // namespace detail

    //  trim specific predicates -------------------------------------//

    // Construct isclassified functor
    template< typename CharT >
    detail::string_algo::isclassifiedF<CharT> 
    if_isclassified( std::ctype_base::mask Type, const std::locale& Loc=std::locale() )
    {
        return detail::string_algo::isclassifiedF<CharT>( Type, Loc );
    }

    // Construct isspace functor to use with trim
    template< typename CharT >
    detail::string_algo::isclassifiedF<CharT> 
    if_isspace( const std::locale& Loc=std::locale() )
    {
        return detail::string_algo::isclassifiedF<CharT>( std::ctype_base::space, Loc );
    }

    // Construct isfrom functor 
    template< typename SeqT >
    detail::string_algo::isfromF<SeqT> 
    if_isfrom( const SeqT& Seq )
    {
        return detail::string_algo::isfromF<SeqT>(Seq); 
    }


} // namespace boost


#endif  // BOOST_STRING_FUNCT_HPP

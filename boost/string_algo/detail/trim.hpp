//  Boost string_algo library trim.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_TRIM_DETAIL_HPP
#define BOOST_STRING_TRIM_DETAIL_HPP

#include <functional>
#include <locale>
#include <set>

namespace boost {

    namespace string_algo {

        namespace detail {

//  trim iterator helper -----------------------------------------------//

            // Search for first non matching character from the beginning of the sequence
            template< typename Iterator, typename Predicate >
            inline Iterator trim_begin_if( Iterator InBegin, Iterator InEnd, Predicate IsSpace )
            {
                return std::find_if( 
                    InBegin, 
                    InEnd, 
                    std::not1(IsSpace));
            }

            // Search for first non matching character from the end of the sequence
            template< typename Iterator, typename Predicate >
            inline Iterator trim_end_if( Iterator InBegin, Iterator InEnd, Predicate IsSpace )
            {
                for( Iterator It=InEnd; It!=InBegin;  )
                {
                    if ( !IsSpace(*(--It)) )
                        return ++It;
                }

                return InBegin;
            }

//  trim functors -----------------------------------------------//

            // isclassified functor
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
                isfromF( const SeqT& Seq ) : m_Set( Seq.begin(), Seq.end() ) {}
                
                // Operation
                result_type operator ()( argument_type Ch ) const
                {
					return m_Set.find( Ch )!=m_Set.end();
                }
            
            private:
				std::set<CharT> m_Set;                
            };

        } // namespace detail

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_TRIM_DETAIL_HPP

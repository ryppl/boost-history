//  Boost string_algo library find_format_store.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_REPLACE_STORE_DETAIL_HPP
#define BOOST_STRING_REPLACE_STORE_DETAIL_HPP

#include <boost/string_algo/config.hpp>
#include <boost/string_algo/container_traits.hpp>
#include <boost/string_algo/iterator_range.hpp>

namespace boost {
    namespace string_algo {
        namespace detail {

//  temporary format and find result storage --------------------------------//

            template< 
                typename ForwardIteratorT,
                typename FormatterT,
                typename FormatResultT >
            class find_format_store : 
                public iterator_range<ForwardIteratorT>
            {
            public:
                // typedefs
                typedef iterator_range<ForwardIteratorT> base_type;
                typedef FormatterT  formatter_type;
                typedef FormatResultT format_result_type;
                
            public:
                // Construction
                find_format_store( 
                        const base_type FindResult,
                        const format_result_type& FormatResult,
                        const formatter_type& Formatter ) :
                    base_type(FindResult),
                    m_FormatResult(FormatResult),
                    m_Formatter(Formatter) {}

                // Assignment
                template< typename FindResultT >
                find_format_store& operator=( FindResultT FindResult )
                {
                    iterator_range<ForwardIteratorT>::operator=(FindResult);
                    m_FormatResult=m_Formatter(FindResult);
                    
                    return *this;
                }

                // Retrive format result
                const format_result_type& format_result()
                {   
                    return m_FormatResult;
                }

            private:
                format_result_type m_FormatResult;
                const formatter_type& m_Formatter;
            };

        } // namespace detail
    } // namespace string_algo
} // namespace boost

#endif  // BOOST_STRING_REPLACE_STORE_DETAIL_HPP

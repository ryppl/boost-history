//  Boost string_algo library container_basic_string.hpp header file  ------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_DETAIL_CONTAINER_BASIC_STRING_HPP
#define BOOST_STRING_DETAIL_CONTAINER_BASIC_STRING_HPP

#include <boost/config.hpp>
#include <string>

namespace boost {

    namespace string_algo {

        namespace detail {

//  replace functor  -------------------------------------------//
        
#ifdef BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

			template<> 
			struct replaceF<std::string>
            {
				typedef std::string input_type;
                typedef input_type::iterator input_iterator_type;
                typedef input_type& input_reference_type;

                // Sequence variant
				template< typename InsertT >
                void operator()(
                    input_reference_type Input,
                    input_iterator_type From,
                    input_iterator_type To,
					const InsertT& Insert )
                {
					Input.replace( From, To, Insert.begin(), Insert.end() );
				}

                // Iterator variant
				template< typename InsertIteratorT >
                void operator()(
                    input_reference_type Input,
                    input_iterator_type From,
                    input_iterator_type To,
					InsertIteratorT Begin,
                    InsertIteratorT End )
                {
					Input.replace( From, To, Begin, End );
                }
			};

			template<> 
			struct replaceF<std::wstring>
            {
				typedef std::wstring input_type;
                typedef input_type::iterator input_iterator_type;
                typedef input_type& input_reference_type;

                // Sequence variant
				template< typename InsertT >
                void operator()(
                    input_reference_type Input,
                    input_iterator_type From,
                    input_iterator_type To,
					const InsertT& Insert )
                {
					Input.replace( From, To, Insert.begin(), Insert.end() );
				}

                // Iterator variant
				template< typename InsertIteratorT >
                void operator()(
                    input_reference_type Input,
                    input_iterator_type From,
                    input_iterator_type To,
					InsertIteratorT Begin,
                    InsertIteratorT End )
                {
					Input.replace( From, To, Begin, End );
                }
			};

#else
			template< typename CharT, typename Traits, typename Allocator > 
			struct replaceF< std::basic_string<CharT, Traits, Allocator> >
            {
                typedef std::basic_string<CharT, Traits, Allocator> input_type;
                typedef typename input_type::iterator input_iterator_type;
                typedef input_type& input_reference_type;

                // Sequence variant
				template< typename InsertT >
                void operator()(
                    input_reference_type Input,
                    input_iterator_type From,
                    input_iterator_type To,
					const InsertT& Insert )
                {
					Input.replace( From, To, Insert.begin(), Insert.end() );
				}

                // Iterator variant
				template< typename InsertIteratorT >
                void operator()(
                    input_reference_type Input,
                    input_iterator_type From,
                    input_iterator_type To,
					InsertIteratorT Begin,
                    InsertIteratorT End )
                {
					Input.replace( From, To, Begin, End );
                }
			};

#endif // BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION

		} // namespace detail

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_DETAIL_CONTAINER_BASIC_STRING_HPP

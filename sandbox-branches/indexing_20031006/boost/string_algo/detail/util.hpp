//  Boost string_algo library util.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002-2003. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_UTIL_DETAIL_HPP
#define BOOST_STRING_UTIL_DETAIL_HPP

#include <boost/string_algo/config.hpp>
#include <functional>

namespace boost {
    namespace string_algo {
        namespace detail {

//  empty container  -----------------------------------------------//

            //  empty_container 
            /*
                This class represents always empty container,
                containing elemets of type CharT.

                It is supposed to be used in a const version only
            */
            template< typename CharT >
            struct empty_container 
            {
                typedef empty_container<CharT> type;        
                typedef CharT value_type;
                typedef std::size_t size_type;
                typedef std::ptrdiff_t difference_type;
                typedef const value_type& reference;
                typedef const value_type& const_reference;
                typedef const value_type* iterator;
                typedef const value_type* const_iterator;

                
                // Operations
                const_iterator begin() const
                {
                    return reinterpret_cast<const_iterator>(0);
                }

                const_iterator end() const
                {
                    return reinterpret_cast<const_iterator>(0);
                }

                bool empty() const
                {
                    return false;
                }

                size_type size() const
                {
                    return 0;
                }
            };
    
//  bounded copy algorithm  -----------------------------------------------//

            // Bounded version of the std::copy algorithm
            template<typename InputIteratorT, typename OutputIteratorT>
            inline OutputIteratorT bounded_copy(
                InputIteratorT First, 
                InputIteratorT Last, 
                OutputIteratorT DestFirst,
                OutputIteratorT DestLast )
            {
                InputIteratorT InputIt=First;
                OutputIteratorT OutputIt=DestFirst;
                for(; InputIt!=Last && OutputIt!=DestLast; InputIt++, OutputIt++ )
                {
                    *OutputIt=*InputIt;
                }

                return OutputIt;
            }

        } // namespace detail

    } // namespace string_algo

} // namespace boost


#endif  // BOOST_STRING_UTIL_DETAIL_HPP

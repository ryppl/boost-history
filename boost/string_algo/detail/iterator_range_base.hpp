//  Boost string_algo library iterator.hpp header file  ---------------------------//

//  (C) Copyright Pavol Droba 2002. Permission to copy, use, modify, sell and
//  distribute this software is granted provided this copyright notice appears
//  in all copies. This software is provided "as is" without express or implied
//  warranty, and with no claim as to its suitability for any purpose.

//  See http://www.boost.org for updates, documentation, and revision history.

#ifndef BOOST_STRING_DETAIL_ITERATOR_RANGE_HPP
#define BOOST_STRING_DETAIL_ITERATOR_RANGE_HPP

#include <iterator>
#include <boost/detail/iterator.hpp>

namespace boost {

    namespace string_algo {

        namespace detail {

//  iterator range base  -----------------------------------------------//

            // iterator_range_base
            /*
                A base class for the iterator_range
            */
            template<typename IteratorT> class iterator_range_base
            {
            public:
                typedef iterator_range_base<IteratorT> type;
                typedef typename boost::detail::
                    iterator_traits<IteratorT>::value_type value_type;
                typedef typename boost::detail::
                    iterator_traits<IteratorT>::reference reference;
                typedef typename boost::detail::
                    iterator_traits<IteratorT>::difference_type difference_type;
                typedef IteratorT const_iterator;
                typedef IteratorT iterator;

                // Constructors
                iterator_range_base() {}

                iterator_range_base( iterator Begin, iterator End ) : 
                    m_Begin( Begin ), m_End( End ) {}

                iterator_range_base( const iterator_range_base& Other ) :
                    m_Begin( Other.begin(), Other.end() ) {} 

                // Assignment
                iterator_range_base& operator=( const iterator_range_base& Other )
                {
                    m_Begin=Other.begin(); m_End=Other.end();
                    return *this;
                }

                template< typename OtherItT >
                iterator_range_base& operator=( const iterator_range_base<OtherItT>& Other )
                {
                    m_Begin=Other.begin(); m_End=Other.end();
                    return *this;
                }

                // Comparison
                template< typename OtherItT > 
                bool operator==( const iterator_range_base<OtherItT>& Other )
                {
                    return m_Begin==Other.begin() && m_End==Other.end();
                }

                template< typename OtherItT > 
                bool operator!=( const iterator_range_base<OtherItT>& Other )
                {
                    return m_Begin!=Other.begin() || m_End!=Other.end();
                }

                // Operations
                IteratorT begin() const 
                { 
                    return m_Begin; 
                }

                IteratorT end() const 
                { 
                    return m_End; 
                } 

                bool empty() const 
                { 
                    return m_Begin==m_End; 
                }
                
                difference_type size() const
                { 
                    return std::distance( m_Begin, m_End ); 
                }

            private:
                // begin and end iterators
                IteratorT m_Begin;
                IteratorT m_End;
            };

        } // namespace detail

    } // namespace string_algo

} // namespace boost

#endif  // BOOST_STRING_DETAIL_ITERATOR_RANGE_HPP

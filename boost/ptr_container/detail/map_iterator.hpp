/**
 * Pointer-Container Library
 * (C) Thorsten Ottosen 2003-2004
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/ptr_container for documentation.
 *
 */

#ifndef BOOST_PTR_CONTAINER_MAP_ITERATOR_HPP
#define BOOST_PTR_CONTAINER_MAP_ITERATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/config.hpp>
#include <boost/operators.hpp>
#include <boost/detail/iterator.hpp>
#include <cassert>

namespace boost
{
    namespace ptr_container
    {
        namespace detail
        {
            template< typename I, typename V > // I = original iterator, V = return value type of operator*()
            class map_iterator : bidirectional_iterator_helper< map_iterator<I,V>, typename boost::detail::iterator_traits<I>::value_type, 
                                                                std::ptrdiff_t, typename boost::detail::iterator_traits<I>::value_type*, 
                                                                V& >     
            {
                I iter_;
                typedef typename boost::detail::iterator_traits<I>::value_type  pair_t;
                typedef typename pair_t::first_type                             key_type;
                
            public:
                typedef std::ptrdiff_t difference_type;
                typedef V              value_type;
                typedef V*             pointer;
                typedef V&             reference;
                typedef                std::bidirectional_iterator_tag  iterator_category;        
                
            public:
                explicit map_iterator( const I& i ) : iter_( i )         {}
                
                map_iterator( const map_iterator& r ) : iter_( r.iter_ ) {}
                
                map_iterator& operator=( const map_iterator& r ) 
                {
                    iter_ = r.iter_;
                    return *this;
                }
                
                V& operator*() const
                {
                    return *iter_->second;
                }
                
                map_iterator& operator++()
                {
                    ++iter_;
                    return *this;
                }
                
                map_iterator& operator--()
                {
                    --iter_;
                    return *this;
                }

                bool operator==( const map_iterator& r ) const
                {
                    return iter_ == r.iter_;
                }
                
                I base() const
                {
                    return iter_;
                }
                
                key_type key() const
                {
                    return iter_->first;
                }
                
            }; // class 'map_iterator'
        } // namespace 'detail'
    } // nameespace 'ptr_container'
    
}

#endif

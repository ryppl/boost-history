/**
 * Pointer-Container Library
 * (C) Thorsten Ottosen 2003
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

#ifndef BOOST_PTR_CONTAINER_SWAP_ITERATOR_HPP
#define BOOST_PTR_CONTAINER_SWAP_ITERATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>
#include <boost/utility.hpp>
#include <boost/operators.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/detail/iterator.hpp>
#include <iosfwd>
#include <cassert>
#include <iterator>
#include <vector>

namespace boost
{
    namespace ptr_container
    {
        namespace detail
        {
            template< typename I >
            class swapper
            {
                typedef typename std::iterator_traits<I>::value_type T;
                //T* i_;
                I& i_;
                
                void swap( swapper& r ) // swap pointers
                {
                    T t   = *i_;
                    *i_   = *r.i_;
                    *r.i_ = t;
                }
                
                public:
             
                swapper( I& i ) : i_( i )
                { }
                
                swapper( const swapper& r ) : i_( r.i_ ) 
                {}
                
                void operator=( swapper& r )
                { swap( r ); }
                
                void operator=( T t ) // T is a pointer
                {
                    *i_ = t;
                }
                
                friend inline bool operator<( const swapper& l, const swapper& r )
                {
                    return *l.i_ < *r.i_;
                }
                
                I iter() const
                {
                    return i_;
                }
                
                operator T() const // T is a pointer
                {
                    return *i_;
                }
            };
            
            
            template< typename I >
            class swap_iterator 
            {
                I          iter_;
                swapper<I> si_;
                
            public:
                typedef std::random_access_iterator_tag               iterator_category;
                typedef typename std::iterator_traits<I>::value_type  value_type;
                typedef std::ptrdiff_t                                difference_type;
                typedef swapper<I>                                    reference;
                typedef value_type*                                   pointer;
                     
                swap_iterator() : iter_( I() ), si_( iter_ )                              {}
                explicit swap_iterator( const I& i ) : iter_( i ), si_( iter_ )           {}
//                explicit swap_iterator( swapper<I>& s ) : iter_( s.iter() ), si_( iter_ ) {}
                
                swap_iterator( const swap_iterator& r ) : iter_( r.iter_ ), si_( iter_ ) {}
                
                swap_iterator& operator=( const swap_iterator& r ) 
                {
                    iter_ = r.iter_;
                    return *this;
                }
                
                swapper<I>& operator*()
                {
                    return si_;
                }
                
                
                const swapper<I>& operator*() const
                {
                    return si_;
                }
                
                
                swap_iterator& operator++()
                {
                    ++iter_;
                    return *this;
                }
                
                swap_iterator& operator--()
                {
                    --iter_;
                    return *this;
                }
                
                swap_iterator operator++( int )
                {
                    swap_iterator tmp( iter_ );
                    ++tmp;
                    return tmp;
                }
                
                swap_iterator operator--( int )
                {
                    swap_iterator tmp( iter_ );
                    --tmp;
                    return tmp;
                }

                swap_iterator& operator+=( std::ptrdiff_t n )
                {
                    iter_ += n;
                    return *this;
                }
                
                swap_iterator& operator-=( std::ptrdiff_t n )
                {
                    iter_ -= n;
                    return *this;
                }


                friend inline swap_iterator operator+( std::ptrdiff_t n, const swap_iterator& r )
                {
                    swap_iterator res( r );
                    res += n;
                    return res;
                }
                
                
                friend inline swap_iterator operator+( const swap_iterator& r, std::ptrdiff_t n )
                {
                    swap_iterator res( r );
                    res += n;
                    return res;
                }

                
                friend inline swap_iterator operator-( std::ptrdiff_t n, const swap_iterator& r )
                {
                    swap_iterator res( r );
                    res -= n;
                    return res;
                }
                
                
                friend inline swap_iterator operator-( const swap_iterator& r, std::ptrdiff_t n )
                {
                    swap_iterator res( r );
                    res -= n;
                    return res;
                }
                

                friend inline bool operator==( const swap_iterator& l, const swap_iterator& r ) 
                {
                    return l.iter_ == r.iter_;
                }
                
                friend inline bool operator!=( const swap_iterator& l, const swap_iterator& r ) 
                {
                    return l.iter_ != r.iter_;
                }

                friend inline bool operator<( const swap_iterator& l, const swap_iterator& r ) 
                {
                    return l.iter_ < r.iter_;
                }
                
                friend inline bool operator>( const swap_iterator& l, const swap_iterator& r ) 
                {
                    return l.iter_ > r.iter_;
                }
                
                friend inline bool operator<=( const swap_iterator& l, const swap_iterator& r ) 
                {
                    return l.iter_ <= r.iter_;
                }
                
                friend inline bool operator>=( const swap_iterator& l, const swap_iterator& r ) 
                {
                    return l.iter_ >= r.iter_;
                }
                
                
                swapper<I> operator[]( unsigned n )
                {
                    swap_iterator tmp( *this );
                    tmp += n; 
                    return tmp.operator*();
                }
                
                friend inline std::ptrdiff_t operator-( const swap_iterator& x, const swap_iterator& y )
                {
                    std::ptrdiff_t n = std::distance( y, x );
                    assert( n > 0 );
                    return n;
                }
                
                I base() const
                {
                    return iter_;
                }
                
            }; // class 'swap_iterator'
        } // namespace 'detail'
    } // nameespace 'ptr_container'
    
//    template<typename I, typename R>
//    inline ptr_container::detail::swap_iterator<I,R> make_indirection_iterator( ptr_container::detail::swap_iterator<I,R>& i )
//    {
//        return ptr_container::detail::swap_iterator<I,R>( i );
//    }
    
    class my_ptr_vector
    {
        std::vector<int*> v_;
        
    public:
        ~my_ptr_vector()
        {
            for( unsigned i = 0; i < v_.size(); ++ i )
                delete v_[i]; 
        }
        
        void push_back( int* p )
        {
            v_.push_back( p );
        }
        
        typedef ptr_container::detail::swap_iterator< std::vector<int*>::iterator > swap_iterator;
        
        swap_iterator begin() { return swap_iterator( v_.begin() ); }
        swap_iterator end()   { return swap_iterator( v_.end() ); }
        
        std::size_t size() const { return v_.size(); }
    };
    
} // namespace 'boost'

#endif

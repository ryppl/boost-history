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
#include <boost/concept_check.hpp>

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
                typedef typename boost::remove_pointer<T>::type               Obj_t;
                    I i_;
                
                void swap( swapper& r ) // swap pointers
                {
                    // swap 
                    
                    T t   = *i_;
                    assert( t != 0 );
                    *i_   = *r.i_;
                    assert( *i_ != 0 );
                    *r.i_ = t; //*/
                    // copy -> crash?
                    //*i_ = *r.i_;
                }
                
                swapper();
                
            public:
             
                swapper( I& i ) : i_( i )
                {
                    //assert( *i_ != 0 ); 
                }
                
                swapper( const swapper& r ) : i_( r.i_ ) 
                {
                    //assert( *i_ != 0 ); 
                }
                
                void operator=( const swapper& r )
                {
                    *i_ = *r.i_; 
                }
                
                void operator=( Obj_t& o )
                {
                    *i_ = &o;
                }
 
                operator const Obj_t&() const
                {
                    assert( *i_ != 0 );
                    return **i_;
                }
                /*
                operator T()
                {
                    assert( *i_ != 0 );
                    return *i_;
                }
                */
                friend inline bool operator==( const swapper& l, const swapper& r )
                {
                    assert( *l.i_ != 0 );
                    assert( *r.i_ != 0 );
                    return *l.i_ == *r.i_; 
                }
                
                T address() const
                {
                    assert( *i_ != 0 );
                    return *i_;
                }
            };
            

            
            template< typename I >
            class swap_iterator 
            {
                I          iter_;
                swapper<I> si_;
                
                swap_iterator();
                
            public:
                typedef std::random_access_iterator_tag               iterator_category;
                typedef typename std::iterator_traits<I>::value_type  orig_value_type;
                typedef typename boost::remove_pointer< orig_value_type >::type  value_type;
                //typedef swapper<I>                                    value_type;
                typedef std::ptrdiff_t                                difference_type;
                typedef swapper<I>&                                   reference;
                typedef value_type*                                   pointer;
                     
                explicit swap_iterator( const I& i ) : iter_( i ), si_( iter_ )           
                {
                    //assert( *iter_ != 0 );
                }
                
                swap_iterator( const swap_iterator& r ) : iter_( r.iter_ ), si_( iter_ ) 
                {
                    //assert( *iter_ != 0 );
                }
                
                swap_iterator& operator=( const swap_iterator& r ) 
                {
                    iter_ = r.iter_;
                    si_   = iter_;
                    assert( *iter_ != 0 );
                    return *this;
                }
                
                swapper<I>& operator*()
                {
                    assert( *iter_ != 0 );
                    return si_;
                }
                
                const swapper<I>& operator*() const
                {
                    assert( *iter_ != 0 );
                    return si_;
                }
                
                orig_value_type operator->()
                {
                    assert( *iter_ != 0 );
                    return *iter_;
                }
                
                
                const orig_value_type operator->() const
                {
                    assert( *iter_ != 0 );
                    return *iter_;
                }
                
                swap_iterator& operator++()
                {
                    assert( *iter_ != 0 );
                    ++iter_;
                    si_ = iter_;
                    // no assert here...could be end();
                    return *this;
                }
                
                swap_iterator& operator--()
                {
                    assert( *iter_ != 0 );
                    --iter_;
                    si_ = iter_;
                    assert( *iter_ != 0 );
                    return *this;
                }
                
                swap_iterator operator++( int )
                {
                    swap_iterator tmp( iter_ );
                    ++*this;
                    // ++tmp; doh!
                    assert( *iter_ != 0 );
                    return tmp;
                }
                
                swap_iterator operator--( int )
                {
                    swap_iterator tmp( iter_ );
                    --*this;
                    //--tmp; doh!
                    assert( *iter_ != 0 );
                    return tmp;
                }

                swap_iterator& operator+=( std::ptrdiff_t n )
                {
                    iter_ += n;
                    si_ = iter_;
                    assert( *iter_ != 0 );
                    return *this;
                }
                
                swap_iterator& operator-=( std::ptrdiff_t n )
                {
                    iter_ -= n;
                    si_   = iter_;
                    assert( *iter_ != 0 );
                    return *this;
                }

                friend inline swap_iterator operator+( std::ptrdiff_t n, const swap_iterator& r )
                {
                    swap_iterator res( r );
                    res += n;
                    assert( *res.iter_ != 0 );
                    return res;
                }
                
                friend inline swap_iterator operator+( const swap_iterator& r, std::ptrdiff_t n )
                {
                    swap_iterator res( r );
                    res += n;
                    assert( *res.iter_ != 0 );
                    return res;
                }

                friend inline swap_iterator operator-( const swap_iterator& r, std::ptrdiff_t n )
                {
                    swap_iterator res( r );
                    res -= n;
                    assert( *res.iter_ != 0 );
                    return res;
                }

                friend inline std::ptrdiff_t operator-( const swap_iterator& x, const swap_iterator& y )
                {
                    assert( x.iter_ - y.iter_ >= 0 );
                    //assert( *x.iter_ != 0 ); could be end()
                    assert( *y.iter_ != 0 );
                    return x.iter_ - y.iter_;
                }
                               
                swapper<I> operator[]( std::ptrdiff_t n )
                {
                    swap_iterator s = *this + n;
                    assert( *s.iter_ != 0 );
                    return s.si_;
                }

                friend inline bool operator==( const swap_iterator& l, const swap_iterator& r ) 
                {
                    assert( *l.iter_ != 0 );
                    // assert( *r.iter_ != 0 ); could be end()
                    return l.iter_ == r.iter_;
                }
                
                friend inline bool operator!=( const swap_iterator& l, const swap_iterator& r ) 
                {
                    //assert( *l.iter_ != 0 );
                    //assert( *r.iter_ != 0 ); // could be end() 
                    return l.iter_ != r.iter_;
                }

                friend inline bool operator<( const swap_iterator& l, const swap_iterator& r ) 
                {
                    assert( *l.iter_ != 0 );
                    assert( *r.iter_ != 0 );
                    return l.iter_ < r.iter_;
                }
                
                friend inline bool operator>( const swap_iterator& l, const swap_iterator& r ) 
                {
                    assert( *l.iter_ != 0 );
                    assert( *r.iter_ != 0 );
                    return l.iter_ > r.iter_;
                }
                
                friend inline bool operator<=( const swap_iterator& l, const swap_iterator& r ) 
                {
                    assert( *l.iter_ != 0 );
                    assert( *r.iter_ != 0 );
                    return l.iter_ <= r.iter_;
                }
                
                friend inline bool operator>=( const swap_iterator& l, const swap_iterator& r ) 
                {
                    assert( *l.iter_ != 0 );
                    assert( *r.iter_ != 0 );
                    return l.iter_ >= r.iter_;
                }
                
                
                /*
                I base() const
                {
                    return iter_;
                }
                */
            }; // class 'swap_iterator'
        } // namespace 'detail'
    } // nameespace 'ptr_container'
    
//    template<typename I, typename R>
//    inline ptr_container::detail::swap_iterator<I,R> make_indirection_iterator( ptr_container::detail::swap_iterator<I,R>& i )
//    {
//        return ptr_container::detail::swap_iterator<I,R>( i );
//    }
    
    template< typename T >
    class my_ptr_vector
    {
        std::vector<T*> v_;
        
    public:
        ~my_ptr_vector()
        {
            for( unsigned i = 0; i < v_.size(); ++i )
                delete v_[i]; 
        }
        
        void push_back( T* p )
        {
            v_.push_back( p );
        }
        
        typedef ptr_container::detail::swap_iterator< typename std::vector<T*>::iterator > swap_iterator;
        
        swap_iterator begin() { return swap_iterator( v_.begin() ); }
        swap_iterator end()   { return swap_iterator( v_.end() ); }
        
        std::size_t size() const { return v_.size(); }
    };
    
} // namespace 'boost'

#endif

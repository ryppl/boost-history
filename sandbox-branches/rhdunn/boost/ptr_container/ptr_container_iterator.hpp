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

#ifndef BOOST_PTR_CONTAINER_PTR_CONTAINER_ITERATOR_HPP
#define BOOST_PTR_CONTAINER_PTR_CONTAINER_ITERATOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/config.hpp>
#include <boost/utility.hpp>
#include <boost/operators.hpp>
#include <boost/type_traits/remove_pointer.hpp>
#include <boost/detail/iterator.hpp>
#include <boost/iterator/indirect_iterator.hpp>
#include <iosfwd>
#include <iostream>
#include <cassert>

using namespace std;

namespace boost
{   
    template< typename ValueType, typename PtrIterator >
    class iterator_proxy
    {
        typedef BOOST_DEDUCED_TYPENAME ::boost::remove_pointer<ValueType>::type T;
        typedef iterator_proxy<ValueType,PtrIterator>                           this_type;
            
        PtrIterator& i_;
        
        void shallow_copy( PtrIterator to, PtrIterator from )
        {
            static int i = 0;
            ++i;
            cout << i << " ";
            *to = *from; 
        }
        
    public:
        explicit iterator_proxy( PtrIterator& i ) : i_( i ) 
        {
            //cout << " (i)cons " ; 
        }
        
        
        
        iterator_proxy( const iterator_proxy& r ) : i_( r.i_ ) 
        {
            //cout << " (i)ccons " ; 
        }
        
        
        
        iterator_proxy& operator=( const iterator_proxy& r )
        {
            // this is wrong! copy the iterator aftawards!!
            //cout << " (i)sc ";
            shallow_copy( i_, r.i_ );
            i_ = r.i_;
            return *this;
        }
        
        
/*
        void operator=( const ValueType& r )
        {
            //cout << " (i)= VT sc ";
            assert( r != (void*)0xfeeefeee );
            *i_ = r;
        }
*/
        
/*        
        indirected_value_type& operator*()
        {
        //    cout << " (i)*() ";
            return **i_;
        }
        
        
        const indirected_value_type& operator*() const
        {
      //      cout << " (i)*() const ";
            return **i_;
        }
 */     
        bool operator==( const this_type& r ) const
        {
            return **i_ == **r.i_;
        }
        
        bool operator<( const this_type& r ) const
        {
            return **i_ < **r.i_;
        }

        
        
   /*     
        bool operator==( const T& r ) const
        {
    //        cout << " (i)== ";
            return **i_ == r;
        }
        
        
        
        bool operator<( const T& r ) const
        {
  //          cout << " (i)< ";
            return **i_ < r;
        }*/
      /*
        operator const indirected_value_type&()  // for assignment, consider reference parameter + overload
        {
//            cout << " (i)VT const ";
            return **i_;
        }
       */
        /*
        operator const ValueType() const
        {
            **i_;
            assert( *i_ != (void*)0x00375A38 );
            return *i_;
        }
        
        */
        
    }; // class 'indirect_type';
    
    template< typename PI > // pointer iterator
    class ptr_container_iterator 
    {
    public:
        typedef PI                                                                       ptr_iterator;                                                      
        typedef BOOST_DEDUCED_TYPENAME detail::iterator_traits<ptr_iterator>::value_type value_type_t;
        typedef BOOST_DEDUCED_TYPENAME ::boost::remove_pointer<value_type_t>::type         T;
        typedef T*                                                                       pointer;
        typedef iterator_proxy<T*,ptr_iterator>                                          proxy_type;
        typedef                                        proxy_type                        value_type;
       // typedef proxy_type                                                               value_type;
        typedef proxy_type&                                                              reference;
        //typedef const proxy_type&                                                        const_reference;
        typedef ptr_container_iterator<ptr_iterator>                                     this_type;
        typedef BOOST_DEDUCED_TYPENAME detail::iterator_traits<ptr_iterator>::iterator_category
                                                                                         iterator_category;
        typedef BOOST_DEDUCED_TYPENAME detail::iterator_traits<ptr_iterator>::difference_type 
                                                                                         difference_type;
    public:
       
        
    private:
        ptr_iterator ptr_iterator_;
        proxy_type   proxy_;
                                       
    public:
        
        explicit ptr_container_iterator( ptr_iterator r ) : ptr_iterator_( r ), proxy_( ptr_iterator_ )
        {
        }
        
        ptr_container_iterator( const ptr_container_iterator& r ) 
         : ptr_iterator_( r.ptr_iterator_ ), proxy_( ptr_iterator_ ) // note: very important that reference is forwarded
        {
            //cout << " copy cons "; 
        }
        
        ptr_container_iterator& operator=( const ptr_container_iterator& r )
        {
            ptr_iterator_ = r.ptr_iterator_;
            //proxy_        = proxy_type( ptr_iterator_ );
            return *this;
        }

        proxy_type& operator*()
        {
            //cout << " *() ";
            return proxy_;
        }
        
        const proxy_type& operator*() const
        {
            //cout << " *() const ";
            return proxy_;
        }
        
        pointer operator->()
        {
            return *ptr_iterator_;
        }
        
        const pointer operator->() const
        {
            return *ptr_iterator_;
        }
        
        bool operator<( const this_type& r ) const
        {
            //cout << " < ";
            return ptr_iterator_ < r.ptr_iterator_;
        }
        
        bool operator==( const this_type& r ) const
        {
            //cout << " == ";
            return ptr_iterator_ == r.ptr_iterator_; 
        }
        
        bool operator!=( const this_type& r ) const
        {
            //cout << " == ";
            return !( *this == r );
        }

        
        this_type& operator++()
        {
            //cout << " ++ " << **ptr_iterator_ << endl;
            ++ptr_iterator_;
            return *this;
        }
        
        this_type operator++( int ) 
        {
            this_type res( *this );
            ++*this;
            return res;
        }
        
        this_type& operator--()
        {
            //cout << " -- ";
            --ptr_iterator_;
            return *this;
        }
        
        this_type operator--( int ) 
        {
            this_type res( *this );
            --*this;
            return res;
        }
        
        this_type& operator+=( difference_type n )
        {
            ptr_iterator_ += n;
            return *this;
        }
        
        friend this_type operator+( const this_type& x, int i )
        {
            this_type res( x );
            return res += i;
        }
        
        this_type& operator-=( difference_type n )
        {
            ptr_iterator_ -= n;
            return *this;
        }
        
        friend this_type operator-( const this_type& x, int i )
        {
            this_type res( x );
            return res -= i;
        }

        friend difference_type operator-( const this_type& x, const this_type& y )
        {
            return x.ptr_iterator_ - y.ptr_iterator_;
        }
    };

         
    template< typename PI >
    inline ptr_container_iterator<PI> make_ptr_container_iterator( PI pi )
    {
        return ptr_container_iterator<PI>( pi );
    }

    // ex: ptr_container_iterator< vector<int*>::iterator >
    //
    // copy( c.begin(), c.end(), ostream_operator<T>( cout ) );
    //
}

#endif

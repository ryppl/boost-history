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
using namespace std;

namespace boost
{   
    template< typename ValueType, typename PtrIterator >
    class iterator_proxy
    {
        typedef BOOST_DEDUCED_TYPENAME ::boost::remove_pointer<ValueType>::type indirected_value_type;
        
        PtrIterator& i_;
        
        void shallow_copy( PtrIterator to, PtrIterator from )
        {
            
            *to = *from; 
        }
        
    public:
        iterator_proxy( PtrIterator& i ) : i_( i ) 
        { }
        
        
        
        iterator_proxy( const iterator_proxy& r ) : i_( r.i_ ) 
        { }
        
        
        
        void operator=( const iterator_proxy& r )
        {
//            cout << " (i)sc ";
            shallow_copy( i_, r.i_ );
        }
        
        

        void operator=( const ValueType& r )
        {
//            cout << " (i)= VT ";
            *i_ = r;
        }

        
        
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
        
        
        
        bool operator==( const indirected_value_type& r )
        {
    //        cout << " (i)== ";
            return **i_ == r;
        }
        
        
        
        bool operator<( const indirected_value_type& r )
        {
  //          cout << " (i)< ";
            return **i_ < r;
        }
      
        operator const indirected_value_type&()  // for assignment, consider reference parameter + overload
        {
//            cout << " (i)VT const ";
            return **i_;
        }
       
        operator const ValueType() const
        {
            return *i_;
        }
        
    }; // class 'indirect_type';
    
    template< typename C, typename I >
    inline std::ostream& operator<<( std::ostream& out, const iterator_proxy<C,I>& r )
    {
        return out << *r;
    }
    
    template< typename PtrIterator >
    struct types
    {   
        typedef BOOST_DEDUCED_TYPENAME detail::iterator_traits<PtrIterator>::value_type  ptr_value_type;
        typedef BOOST_DEDUCED_TYPENAME ::boost::remove_pointer<ptr_value_type>::type     VT;
        typedef VT&                                                                      R;
        typedef ptr_value_type                                                           P;
    };
    
    template< typename PI > // pointer iterator
    class ptr_container_iterator : public random_access_iterator_helper<  ptr_container_iterator<PI>,
                                                                          BOOST_DEDUCED_TYPENAME types<PI>::VT,
                                                                          std::ptrdiff_t,
                                                                          BOOST_DEDUCED_TYPENAME types<PI>::P,
                                                                          BOOST_DEDUCED_TYPENAME types<PI>::R >
    {
    public:
        typedef PI                                                                       ptr_iterator;                                              
        typedef BOOST_DEDUCED_TYPENAME detail::iterator_traits<ptr_iterator>::value_type ptr_value_type;
        typedef ptr_value_type                                                           pointer;
        typedef pointer                                                                  value_type;
        typedef BOOST_DEDUCED_TYPENAME ::boost::remove_pointer<ptr_value_type>::type     indirected_value_type;

        typedef value_type&                                                              reference;
        typedef const value_type&                                                        const_reference;
        typedef ptr_container_iterator<ptr_iterator>                                     this_type;
        typedef BOOST_DEDUCED_TYPENAME detail::iterator_traits<ptr_iterator>::iterator_category
                                                                                         iterator_category;
        typedef BOOST_DEDUCED_TYPENAME detail::iterator_traits<ptr_iterator>::difference_type 
                                                                                         difference_type;
    public:
        typedef iterator_proxy<value_type,ptr_iterator>                                  proxy_type;
        
    private:
        ptr_iterator ptr_iterator_;
        proxy_type   proxy_;
                                       
    public:
        
        ptr_container_iterator( ptr_iterator r = ptr_iterator() ) : ptr_iterator_( r ), proxy_( ptr_iterator_ )
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
        
        this_type& operator++()
        {
            //cout << " ++ " << **ptr_iterator_ << endl;
            ++ptr_iterator_;
            return *this;
        }
        
        this_type& operator--()
        {
            //cout << " -- ";
            --ptr_iterator_;
            return *this;
        }
        
        this_type& operator+=( difference_type n )
        {
            ptr_iterator_ += n;
            return *this;
        }
        
        this_type& operator-=( difference_type n )
        {
            ptr_iterator_ -= n;
            return *this;
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

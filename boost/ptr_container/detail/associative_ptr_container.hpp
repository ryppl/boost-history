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

#ifndef BOOST_PTR_CONTAINER_DETAIL_ASSOCIATIVE_PTR_CONTAINER_HPP
#define BOOST_PTR_CONTAINER_DETAIL_ASSOCIATIVE_PTR_CONTAINER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/ptr_container/detail/reversible_ptr_container.hpp>
//#include <vector>

namespace boost
{

namespace detail
{

    template< typename Container, typename T >
    class associative_ptr_container : public reversible_ptr_container<Container,T>
    {
        typedef reversible_ptr_container<Container,T>          Base;

        //static std::pair<iterator,iterator> make_pair( const std::pair<ptr_iter,ptr_iter>& p )
        //{ return std::make_pair( make_indirection_iterator( p.first ), make_indirection_iterator( p.second ) ); }
        
    public: // typedefs
        typedef Container                  C;
        typedef BOOST_DEDUCED_TYPENAME C::key_type          key_type;
        typedef BOOST_DEDUCED_TYPENAME C::key_compare       key_compare;
        typedef BOOST_DEDUCED_TYPENAME C::value_compare     value_compare;
        typedef BOOST_DEDUCED_TYPENAME Base::scoped_deleter scoped_deleter;
        BOOST_FORWARD_TYPEDEF( Base );
    
    protected:
        void     insert_clones_and_release( scoped_deleter& sd ) // strong
        {
            //
            // 'c_.insert' always provides the strong guarantee for T* elements
            // since a copy constructor of a pointer cannot throw
            //
            this->c__().insert( sd.begin(), sd.end() ); 
            sd.release();
        }

    public: // foundation
        
       template< typename Compare >
       explicit associative_ptr_container( const Compare& comp, const allocator_type& alloc = allocator_type() ) : Base( comp, alloc ) 
       { }

       template< typename InputIterator, typename Compare >
       associative_ptr_container( InputIterator first, InputIterator last, const Compare& comp, 
                                 const allocator_type& alloc = allocator_type() ) : Base( comp, alloc )
       { 
           difference_type n = std::distance( first, last ); 
           scoped_deleter sd( n );                  // strong
           this->make_clones( sd, first, last );    // strong
           insert_clones_and_release( sd );         // strong
       }

    public: // associative container interface
        key_compare    key_comp() const { return this->c_().key_comp();}
        value_compare  value_comp() const { return this->c_().value_comp();}
        
        std::pair<iterator,bool> insert( const Key& x ) // strong
        {
            return insert( ::boost::make_clone ( x ) );
        }
        
        std::pair<iterator,bool> insert( const Key* x ) // strong
        {
            auto_ptr<Key> ptr( x );
            std::pair<ptr_iter,bool> res = this->c__().insert( x );
            if( res.second )
                ptr.release();
            return std::make_pair( ::boost::make_indirection_iterator( res.first ), res.second );
        }
        
        template< typename InputIterator >
        void insert( InputIterator first, InputIterator last )
        {
            scoded_deleter sd( std::distance( first, last ) ); // strong
            this->make_clones( sd, first, last );              // strong
            // hm
            this->c__().insert( sd.begin(), sd.end() );
            sd.release();
        }
        
        iterator        find( const key_type& x) { return make_indirection_iterator( this->c_().find( x ) );}
        const_iterator  find( const key_type& x) const { return make_indirection_iterator( this->c_().find( x ) );}
        size_type       count( const key_type& x ) const { return this->c_().count( x );}
        iterator        lower_bound( const key_type& x) { return make_indirection_iterator( this->c_().lower_bound( x ) );} 
        const_iterator  lower_bound( const key_type& x) const { return make_indirection_iterator( this->c_().lower_bound( x ) );} 
        iterator        upper_bound( const key_type& x) { return make_indirection_iterator( this->c_().upper_bound( x ) );}
        const_iterator  upper_bound( const key_type &x) const { return make_indirection_iterator( this->c_().upper_bound( x ) );}
        //std::pair<iterator, iterator>
        //                equal_range( const key_type& x) { return make_pair( this->c_().equal_range( x ) ); }
        //std::pair<const_iterator, const_iterator>
        //                equal_range( const key_type& x) const { return make_pair( this->c_().equal_range( x ) ); }
    };

} // namespace 'detail'

#define BOOST_FORWARD_ASSOC_TYPEDEF( Base ) \
BOOST_FORWARD_TYPEDEF( Base ); \
typedef BOOST_DEDUCED_TYPENAME Base::key_type key_type; \
typedef BOOST_DEDUCED_TYPENAME Base::key_compare key_compare; \
typedef BOOST_DEDUCED_TYPENAME Base::value_compare value_compare

} // namespace 'boost'


#endif

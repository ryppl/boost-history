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

namespace boost
{

namespace detail
{
    template< typename Config >
    class associative_ptr_container : public reversible_ptr_container<Config>
    {
        typedef reversible_ptr_container<Config> Base;

    public: // typedefs
        typedef typename Config::container_type              C;
        typedef BOOST_DEDUCED_TYPENAME C::key_type           key_type;
        typedef BOOST_DEDUCED_TYPENAME C::key_compare        key_compare;
        typedef BOOST_DEDUCED_TYPENAME C::value_compare      value_compare;
        typedef BOOST_DEDUCED_TYPENAME Base::scoped_deleter  scoped_deleter;
        typedef BOOST_DEDUCED_TYPENAME Base::allocator_type  allocator_type;
        typedef BOOST_DEDUCED_TYPENAME Base::object_type     object_type; 
        typedef BOOST_DEDUCED_TYPENAME Base::iterator        iterator; 
        typedef BOOST_DEDUCED_TYPENAME Base::const_iterator  const_iterator;
        typedef BOOST_DEDUCED_TYPENAME Base::size_type       size_type;
        typedef BOOST_DEDUCED_TYPENAME Base::difference_type difference_type;
        //BOOST_FORWARD_TYPEDEF( Base );
    
    protected:
        void insert_clones_and_release( scoped_deleter& sd ) // strong
        {
            //
            // 'c_.insert' always provides the strong guarantee for T* elements
            // since a copy constructor of a pointer cannot throw
            //
            this->c__().insert( sd.begin(), sd.end() ); 
            sd.release();
        }
        
        /*
        template< typename InputIterator >
        void insert_clones_and_release_map( scoped_deleter& sd,
                                            InputIterator first, InputIterator last )
        {
            $this->c__().insert
        }
        */

    public: // foundation
        
       template< typename Compare >
       explicit associative_ptr_container( const Compare& comp, 
                                           const allocator_type& alloc = allocator_type() ) 
       : Base( comp, alloc ) 
       { }

       template< typename PtrContainer >
       associative_ptr_container( std::auto_ptr<PtrContainer> r ) : Base( r )
       { }
       
       template< typename InputIterator, typename Compare >
       associative_ptr_container( InputIterator first, InputIterator last, const Compare& comp, 
                                  const allocator_type& alloc = allocator_type() )
       : Base( comp, alloc )
       { 
           difference_type n = std::distance( first, last ); 
           scoped_deleter sd( n );                  // strong
           this->make_clones( sd, first, last );    // strong
           insert_clones_and_release( sd );         // strong
       }
       
       template< typename PtrContainer >
       void operator=( std::auto_ptr<PtrContainer> r )
       {
           Base::operator=( r );
       }

    public: // associative container interface
        key_compare key_comp() const 
        {
            return this->c__().key_comp();
        }
        
        value_compare value_comp() const
        {
            return this->c__().value_comp();
        }
        
        using Base::insert;
        
        template< typename InputIterator >
        void insert( InputIterator first, InputIterator last )
        {
            scoped_deleter sd( std::distance( first, last ) ); // strong
            this->make_clones( sd, first, last );              // strong
            // hm
            this->c__().insert( sd.begin(), sd.end() );
            sd.release();
        }
        
        void erase( iterator before ) // nothrow 
        { 
            this->remove( before ); 
            this->c__().erase( before.base() );
        }
        
        size_type erase( const key_type& x )
        {
            // ?
            return 0;
        }
        
        void erase( iterator first, iterator last ) // notrow 
        {
            this->remove( first, last ); 
            this->c__().erase( first.base(), last.base() );
        }

        iterator find( const key_type& x )
        {
            return iterator( this->c__().find( x ) );
        }
        
        const_iterator find( const key_type& x ) const 
        {
            return const_iterator( this->c__().find( x ) );
        }
        
        size_type count( const key_type& x ) const 
        {
            return this->c__().count( x ); 
        }
        
        iterator lower_bound( const key_type& x )
        {
            return iterator( this->c__().lower_bound( x ) );
        }
         
        const_iterator lower_bound( const key_type& x ) const 
        {
            return const_iterator( this->c__().lower_bound( x ) );        
        } 
        
        iterator upper_bound( const key_type& x )
        {
            return iterator( this->c__().upper_bound( x ) ); 
        }
        
        const_iterator upper_bound( const key_type& x ) const
        {
            return const_iterator( this->c__().upper_bound( x ) );
        }
        
        template< typename PtrContainer >
        void transfer( typename PtrContainer::iterator object, PtrContainer& from ) // strong
        {
            assert( !from.empty() );
            this->c__().insert( *object.base() ); // strong
            from.c__().erase( object.base() );    // nothrow
        }
        
        template< typename PtrContainer > 
        void transfer( typename PtrContainer::iterator first, 
                       typename PtrContainer::iterator last, PtrContainer& from )
        {
            this->c__().insert( first.base(), last.base() ); // strong 
            from.c__().erase( first.base(), last.base() );   // nothrow
        }
        
        template< typename PtrContainer >
        void transfer( PtrContainer& from )
        {
            this->c__().insert( from.c__().begin(), from.c__().end() ); // strong
            from.c__().clear();                                   // nothrow
        }

        //std::pair<iterator, iterator>
        //                equal_range( const key_type& x) { return make_pair( this->c_().equal_range( x ) ); }
        //std::pair<const_iterator, const_iterator>
        //                equal_range( const key_type& x) const { return make_pair( this->c_().equal_range( x ) ); }
    };
    
} // namespace 'detail'

#define BOOST_FORWARD_ASSOC_TYPEDEF( Base )                      \
BOOST_FORWARD_TYPEDEF( Base );                                   \
typedef BOOST_DEDUCED_TYPENAME Base::key_type key_type;          \
typedef BOOST_DEDUCED_TYPENAME Base::key_compare key_compare;    \
typedef BOOST_DEDUCED_TYPENAME Base::value_compare value_compare


    /////////////////////////////////////////////////////////////////////////
    // default predicates:
    
    template< typename T >
    struct ptr_less
    {
        bool operator()( const T* l, const T* r ) const
        {
            return *l < *r;
        }
    };

} // namespace 'boost'


#endif

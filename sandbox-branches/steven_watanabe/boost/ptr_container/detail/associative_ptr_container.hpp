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

#ifndef BOOST_PTR_CONTAINER_DETAIL_ASSOCIATIVE_PTR_CONTAINER_HPP
#define BOOST_PTR_CONTAINER_DETAIL_ASSOCIATIVE_PTR_CONTAINER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
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
        typedef BOOST_DEDUCED_TYPENAME Config::container_type   C;
        typedef BOOST_DEDUCED_TYPENAME C::key_type              key_type;
        typedef BOOST_DEDUCED_TYPENAME C::key_compare           key_compare;
        typedef BOOST_DEDUCED_TYPENAME C::value_compare         value_compare;
        typedef BOOST_DEDUCED_TYPENAME Base::allocator_type     allocator_type; 
        typedef BOOST_DEDUCED_TYPENAME Base::iterator           iterator; 
        typedef BOOST_DEDUCED_TYPENAME Base::size_type          size_type;
     
    protected:
        
        void assign();                // hide inherited members
        void operator[]( size_type ); // hide inherited members 
        void sort();                  // hide inherited members 
        
    public: // foundation
        
       template< typename Compare >
       explicit associative_ptr_container( const Compare& comp, 
                                           const allocator_type& alloc = allocator_type() ) 
       : Base( comp, alloc ) 
       { }

       template< typename PtrContainer >
       associative_ptr_container( std::auto_ptr<PtrContainer> r ) : Base( r )
       { }
       
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
        
        void erase( iterator before ) // nothrow 
        { 
            this->remove( before ); 
            this->c__().erase( before.base() );
        }
        
        size_type erase( const key_type& x )
        {
            iterator i = find( x );
            if( i == this->end() )
                return;
            this->remove( i );    
            return this->c__().erase( i.base() );
        }
        
        void erase( iterator first, iterator last ) // notrow 
        {
            this->remove( first, last ); 
            this->c__().erase( first.base(), last.base() );
        }
  
        
        template< typename PtrContainer >
        void transfer( typename PtrContainer::iterator object, PtrContainer& from ) // strong
        {
            assert( !from.empty() );
            this->c__().insert( *object.base() ); // strong
            from.c__().erase( object.base() );    // nothrow
        }
        /*
        note: cannot be made safe easily
        template< typename PtrContainer > 
        void transfer( typename PtrContainer::iterator first, 
                       typename PtrContainer::iterator last, PtrContainer& from )
        {
            // need try-catch
            this->c__().insert( first.base(), last.base() ); // strong 
            from.c__().erase( first.base(), last.base() );   // nothrow
        }
        
        template< typename PtrContainer >
        void transfer( PtrContainer& from )
        {
            // need try-catch
            this->c__().insert( from.c__().begin(), from.c__().end() ); // strong
            from.c__().clear();                                         // nothrow
        }
        */
        
    };
    
} // namespace 'detail'
    
    /////////////////////////////////////////////////////////////////////////
    // set algorithms
#define BOOST_PTR_SET_ALGORITHMS( Key )                                          \
                                                                                 \
    iterator find( const Key& x )                                                \
    {   /* rvalue-lvalue conversion problem + broken vc7 lib */                  \
        return iterator( this->c__().find( const_cast<Key*>( &x ) ) );           \
    }                                                                            \
                                                                                 \
    size_type count( const Key& x ) const                                        \
    {                                                                            \
        return this->c__().count( &x );                                          \
    }                                                                            \
                                                                                 \
    iterator lower_bound( const Key& x ) const                                   \
    {                                                                            \
        return iterator( this->c__().lower_bound( &x ) );                        \
    }                                                                            \
                                                                                 \
    iterator upper_bound( const Key& x ) const                                   \
    {                                                                            \
        return iterator( this->c__().upper_bound( &x ) );                        \
    }                                                                            \
                                                                                 \
    std::pair<iterator,iterator> equal_range( const Key& x ) const               \
    {                                                                            \
        std::pair<ptr_iterator,ptr_iterator> p = this->c__().equal_range( &x );  \
        return std::make_pair( iterator( p.first ), iterator( p.second ) );      \
    }

#define BOOST_PTR_MAP_ALGORITHMS( Key, T )                                       \
    iterator find( const Key& x )                                                \
    {                                                                            \
        return iterator( this->c__().find( x ) );                                \
    }                                                                            \
                                                                                 \
    const_iterator find( const Key& x ) const                                    \
    {                                                                            \
        return const_iterator( this->c__().find( x ) );                          \
    }                                                                            \
                                                                                 \
    size_type count( const Key& x ) const                                        \
    {                                                                            \
        return this->c__().count( x );                                           \
    }                                                                            \
                                                                                 \
    iterator lower_bound( const Key& x )                                         \
    {                                                                            \
        return iterator( this->c__().lower_bound( x ) );                         \
    }                                                                            \
                                                                                 \
    const_iterator lower_bound( const Key& x ) const                             \
    {                                                                            \
        return const_iterator( this->c__().lower_bound( x ) );                   \
    }                                                                            \
                                                                                 \
    iterator upper_bound( const Key& x )                                         \
    {                                                                            \
        return iterator( this->c__().upper_bound( x ) );                         \
    }                                                                            \
                                                                                 \
    const_iterator upper_bound( const Key& x ) const                             \
    {                                                                            \
        return const_iterator( this->c__().upper_bound( x ) );                   \
    }                                                                            \
                                                                                 \
    std::pair<iterator, iterator> equal_range( const Key& x )                    \
    {                                                                            \
        std::pair<ptr_iterator,ptr_iterator> p = this->c__().equal_range( x );   \
        return std::make_pair( iterator( p.first ), iterator( p.second ) );      \
    }                                                                            \
                                                                                 \
    std::pair<const_iterator, const_iterator> equal_range( const Key& x ) const  \
    {                                                                            \
        std::pair<ptr_const_iterator,ptr_const_iterator> p = this->c__().equal_range( x ); \
        return std::make_pair( const_iterator( p.first ), const_iterator( p.second ) );    \
    }                                                                            \
                                                                                 \
    T& operator[]( const Key& key )                                              \
    {                                                                            \
       iterator i = this->find( key );                                           \
       if( i != this->end() )                                                    \
           return *i;                                                            \
       else                                                                      \
           throw bad_ptr_container_operation( "'operator[]()' could not find key" ); \
    }                                                                            \
                                                                                 \
    const T& operator[]( const Key& key ) const                                  \
    {                                                                            \
       iterator i = this->find( key );                                           \
       if( i != this->end() )                                                    \
           return *i;                                                            \
       else                                                                      \
           throw bad_ptr_container_operation( "'operator[]()' could not find key" ); \
    }
    
} // namespace 'boost'


#endif

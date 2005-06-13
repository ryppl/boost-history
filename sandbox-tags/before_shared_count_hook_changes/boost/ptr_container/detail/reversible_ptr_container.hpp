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

#ifndef BOOST_PTR_CONTAINER_DETAIL_REVERSIBLE_PTR_CONTAINER_HPP
#define BOOST_PTR_CONTAINER_DETAIL_REVERSIBLE_PTR_CONTAINER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif

#include <boost/ptr_container/detail/map_iterator.hpp>
#include <boost/ptr_container/detail/scoped_deleter.hpp>
#include <boost/ptr_container/detail/size_undoer.hpp>
#include <boost/ptr_container/bad_index.hpp>
#include <boost/ptr_container/bad_pointer.hpp>
#include <boost/ptr_container/bad_ptr_container_operation.hpp>
#include <boost/ptr_container/make_clone.hpp>
#include <boost/ptr_container/ptr_predicate.hpp>
#include <boost/bind.hpp>
#include <boost/config.hpp>
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/operators.hpp>
#include <boost/utility.hpp>
#include <cassert>
#include <algorithm>
#include <exception>
#include <functional>
#include <memory>

// TODO: use concept checks to check interface.

namespace boost
{

    
namespace detail
{
    template< typename C, typename T >
    struct default_config
    {
        typedef C                                                             container_type;
        typedef T                                                             value_type;
        typedef boost::indirect_iterator<typename C::iterator>                iterator;
        typedef boost::indirect_iterator<typename C::const_iterator>          const_iterator;                
        typedef boost::indirect_iterator<typename C::reverse_iterator>        reverse_iterator;
        typedef boost::indirect_iterator<typename C::const_reverse_iterator>  const_reverse_iterator;              
        typedef T                                                             object_type;
    };
    
    
    
    template< typename Config >
    class reversible_ptr_container : less_than_comparable< reversible_ptr_container< Config >, 
                                     equality_comparable< reversible_ptr_container< Config > > >, noncopyable
    {
        // static assert: no array
        typedef BOOST_DEDUCED_TYPENAME Config::container_type C;
        C         c_;

    protected:
        C& c__()                { return c_; }
        const C& c__() const    { return c_; }
    

    protected:
        typedef BOOST_DEDUCED_TYPENAME Config::value_type     T;

    public: // typedefs
        typedef  BOOST_DEDUCED_TYPENAME Config::object_type   object_type;
        typedef  T*                                           value_type;
        typedef  T*                                           pointer;
        typedef  T&                                           reference;
        typedef  const T&                                     const_reference;
        
        typedef  BOOST_DEDUCED_TYPENAME Config::iterator                   iterator;
        typedef  BOOST_DEDUCED_TYPENAME Config::const_iterator             const_iterator;
        typedef  BOOST_DEDUCED_TYPENAME Config::reverse_iterator           reverse_iterator;
        typedef  BOOST_DEDUCED_TYPENAME Config::const_reverse_iterator     const_reverse_iterator;
        typedef  BOOST_DEDUCED_TYPENAME C::difference_type                 difference_type; 
        typedef  BOOST_DEDUCED_TYPENAME C::size_type                       size_type;
        typedef  BOOST_DEDUCED_TYPENAME C::allocator_type                  allocator_type;

        typedef BOOST_DEDUCED_TYPENAME C::iterator                         ptr_iterator; 
        typedef BOOST_DEDUCED_TYPENAME C::const_iterator                   ptr_const_iterator;
        typedef BOOST_DEDUCED_TYPENAME C::reverse_iterator                 ptr_reverse_iterator; 
        typedef BOOST_DEDUCED_TYPENAME C::const_reverse_iterator           ptr_const_reverse_iterator;
        
    protected: // implementation
            
        typedef ptr_container::detail::scoped_deleter<T>                   scoped_deleter;
        typedef ptr_container::detail::size_undoer<C>                      size_undoer;


    protected:

        template< typename InputIterator >  
        void copy( InputIterator first, InputIterator last ) 
        {
            std::copy( first, last, begin() );
        }
        
        void copy( const reversible_ptr_container& r )
        { 
            copy( r.begin(), r.end() );
        }
        
        
        // consider moving this two to scoped_deleter as constructor
        void make_clones( scoped_deleter& sd, size_type n, const_reference x ) // strong
        {
            for( size_type i = 0; i != n; i++ )
                sd.add( make_clone( x ) );
        }
        
        // consider moving this two to scoped_deleter as constructor
        template< typename InputIterator >
        void make_clones( scoped_deleter& sd, InputIterator first, InputIterator last  ) // strong
        {
            for( ; first != last; ++first )
                sd.add( make_clone( *first ) );
        }

        void copy_clones_and_release( scoped_deleter& sd ) // nothrow
        {
            assert( std::distance( sd.begin(), sd.end() ) == c_.size() );
            std::copy( sd.begin(), sd.end(), c_.begin() );
            sd.release(); 
        }
        
        template< typename OutputIterator >
        void copy_clones_and_release( scoped_deleter& sd, OutputIterator where ) // nothrow
        {
            //assert( where + distance( first, last ) < c_.end() );
            std::copy( sd.begin(), sd.end(), where );
            sd.release();
        }
        
        void insert_clones_and_release( scoped_deleter& sd, iterator where ) // strong
        {
            //
            // 'c_.insert' always provides the strong guarantee for T* elements
            // since a copy constructor of a pointer cannot throw
            //
            c_.insert( where.base(), sd.begin(), sd.end() ); 
            sd.release();
        }
        
        template< typename InputIterator >
        void clone_assign( InputIterator first, InputIterator last ) // strong 
        {
            assert( first != last );
            scoped_deleter sd( std::distance( first, last ) ); // strong
            make_clones( sd, first, last );                    // strong 
            copy_clones_and_release( sd );                     // nothrow
        }
        
        void clone_assign( size_type n, const_reference x ) // strong
        {
            assert( n != 0 );
            scoped_deleter sd( n );         // strong     
            make_clones( sd, n, x );        // strong
            copy_clones_and_release( sd );  // nothrow
        }

        void strong_resize_and_remove( size_type n )
        {
            const size_type old_size = c_.size();
            if( n > old_size ) // if resize won't throw
            {
                remove_all();
                c_.resize( n );
            }
            else if( n < old_size ) // if rezise might throw
            {
                size_undoer su( c_ );
                c_.resize( n );
                su.release();
                remove_all();
            }
            else 
            {
                assert( n == old_size );
                remove_all();
            }
        }
        
        void remove_all() 
        {
            remove( begin(), end() ); 
        }
        
        void remove( iterator i )
        { 
            checked_delete( &*i );
        }
        
        void remove( iterator first, iterator last ) 
        {
            for( ; first != last; ++first )
                checked_delete( &*first );
        }

        template< typename ForwardIterator >
        ForwardIterator advance( ForwardIterator begin, size_type n ) 
        {
            ForwardIterator iter = begin;
            std::advance( iter, n );
            return iter;
        }
        
    public: // foundation! should be protected!
        
        reversible_ptr_container( const allocator_type& alloc = allocator_type() ) 
        : c_( alloc )  {}
        
        // overhead: null-initilization of container pointer (very cheap compared to cloning)
        // overhead: 1 heap allocation (very cheap compared to cloning)
        reversible_ptr_container( size_type n, const_reference x, 
                                  const allocator_type& alloc = allocator_type() ) 
        : c_( n, 0, alloc ) 
        { 
            if( n == 0 )
                return;
            clone_assign( n, x ); 
        }
        
        template< typename PtrContainer >
        reversible_ptr_container( std::auto_ptr<PtrContainer> clone )                 
        { 
            swap( *clone ); 
        }
        
        // overhead: null-initilization of container pointer (very cheap compared to cloning)
        // overhead: 1 heap allocation (very cheap compared to cloning)
        template< typename InputIterator >
        reversible_ptr_container( InputIterator first, InputIterator last, 
                                  const allocator_type& alloc = allocator_type() ) 
        : c_( std::distance( first, last ), pointer(), alloc ) 
        { 
            if( first == last )
                return;
            clone_assign( first, last );
        }
        
    public: // map/set foundation - just initialize 'c_'
        
        template< typename Compare >
        explicit reversible_ptr_container( const Compare& comp, 
                                           const allocator_type& alloc = allocator_type() ) 
        : c_( comp, alloc ) {}
        
        template< typename InputIterator, typename Compare >
        reversible_ptr_container( InputIterator first, InputIterator last, const Compare& comp, 
                                  const allocator_type& alloc = allocator_type() )
        : c_( comp, alloc ) {}
         
    public:        
        ~reversible_ptr_container()
        { 
            remove_all();
        }
        
        template< typename PtrContainer >
        void operator=( std::auto_ptr<PtrContainer> clone )     
        {
            swap( *clone );
        }
        
        // overhead: 1 heap allocation (very cheap compared to cloning)
        template< typename InputIterator >
        void assign( InputIterator first, InputIterator last ) // strong 
        { 
            if( first == last ) 
                return;
            difference_type n = std::distance( first, last ); 
            scoped_deleter sd( n );         // strong
            make_clones( sd, first, last ); // strong
            strong_resize_and_remove( n );  // strong
            copy_clones_and_release( sd );  // nothrow
        }
        
        // overhead: 1 heap allocation (very cheap compared to cloning)
        void assign( size_type n, const_reference x ) // strong         
        {
            if( n == 0 )
                return;
            scoped_deleter sd( n );        // strong
            make_clones( sd, n, x );       // strong
            strong_resize_and_remove( n ); // strong
            copy_clones_and_release( sd ); // nothrow
        }
        
        /* not possible right now
        allocater_type& get_allocator()
        {
            return c_.get_allocator();
        }*/
        
        allocator_type get_allocator() const                   
        {
            return c_.get_allocator(); 
        }

    public: // container requirements
        iterator                   begin()            { return iterator( c_.begin() ); }
        const_iterator             begin() const      { return const_iterator( c_.begin() ); }
        iterator                   end()              { return iterator( c_.end() ); }
        const_iterator             end() const        { return const_iterator( c_.end() ); }
        reverse_iterator           rbegin()           { return reverse_iterator( c_.rbegin() ); } 
        const_reverse_iterator     rbegin() const     { return const_reverse_iterator( c_.rbegin() ); } 
        reverse_iterator           rend()             { return reverse_iterator( c_.rend() ); } 
        const_reverse_iterator     rend() const       { return const_reverse_iterator( c_.rend() ); } 
        ptr_iterator               ptr_begin()        { return c_.begin(); }
        ptr_const_iterator         ptr_begin() const  { return c_.begin(); }
        ptr_iterator               ptr_end()          { return c_.end(); }
        ptr_const_iterator         ptr_end() const    { return c_.end(); }
        ptr_reverse_iterator       ptr_rbegin()       { return c_.rbegin(); }
        ptr_const_reverse_iterator ptr_rbegin() const { return c_.rbegin(); }
        ptr_reverse_iterator       ptr_rend()         { return c_.rend(); }
        ptr_const_reverse_iterator ptr_rend() const   { return c_.rend(); }

        void swap( reversible_ptr_container& r ) // notrow
        { 
            c_.swap( r.c_ );
        }
          
        size_type size() const // nothrow
        {
            return c_.size();
        }
        
        void resize( size_type sz, const_reference x )  // strong     
        {
            size_type old_size = size();
            if( sz > old_size )
            {
                size_type n = sz - old_size;
                assert( n > 0 );
                scoped_deleter sd( n );      // strong
                make_clones( sd, n, x );     // strong
                size_undoer su( c_ );        // strong
                c_.resize( n );              // strong or nothrow, commit
                su.release();                // nothrow
                copy_clones_and_release( sd, advance( c_.begin(), old_size ) ); // nothrow
            }
            else if( sz < old_size )
            {
                erase( advance( begin(), sz ), end() ); // nothrow
            }
            else
                ;
        }
        
        size_type max_size() const // nothrow
        {
            return c_.max_size(); 
        }
        
        bool empty() const // nothrow
        {
            return c_.empty();
        }

    public: // optional container requirements
        reference        front()       /* nothrow */ { assert( !empty() ); return *c_.front(); }
        const_reference  front() const /* nothrow */ { assert( !empty() ); return *c_.front(); }
        reference        back()        /* nothrow */ { assert( !empty() ); return *c_.back(); }
        const_reference  back() const  /* nothrow */ { assert( !empty() ); return *c_.back(); }

        bool operator==( const reversible_ptr_container& r ) const // nothrow
        { 
            if( size() != r.size() )
                return false;
            else
                return std::equal( begin(), end(), r.begin() );
        }
        
        bool operator<( const reversible_ptr_container& r ) const // nothrow 
        {
             return std::lexicographical_compare( begin(), end(), r.begin(), r.end() );
        }
        
    public: // modifiers

        void push_back( T* x )  // strong               
        {
            if( 0 == x )
                throw bad_pointer( "Null pointer in 'push_back()'" );
            
            std::auto_ptr<T> ptr( x ); // notrow
            c_.push_back( x );         // strong, commit
            ptr.release();             // nothrow
        }
          
        void push_back( const T& x ) // strong
        { 
            push_back( make_clone( x ) );
        }
        
        void pop_back() // nothrow                               
        {
            assert( !empty() ); // wrong ?
            erase( --end() );
        }
        
        iterator insert( iterator before, T* x ) // strong
        { 
            if( 0 == x )
                throw bad_pointer( "Null pointer in 'insert()'" );
            
            std::auto_ptr<T> ptr( x );                                // nothrow
            iterator res = iterator( c_.insert( before.base(), x ) ); // strong, commit
            ptr.release();                                            // nothrow
            return res;
        }
                
        iterator insert( iterator before, const_reference x ) 
        { 
            return insert( before, make_clone( x ) ); 
        }

        void insert( iterator before, size_type n, const_reference x ) // strong 
        {
            if( n == 0 )
                return;
            scoped_deleter sd( n );                  // strong
            make_clones( sd, n, x );                 // strong 
            insert_clones_and_release( sd, before ); // strong, commit
        }
        
        template< typename InputIterator >
        void insert( iterator before, InputIterator first, InputIterator last ) // strong 
        {
            if( first == last ) 
                return;
            scoped_deleter sd( std::distance( first, last ) ); // strong
            make_clones( sd, first, last );                    // strong
            insert_clones_and_release( sd, before );           // strong, commit 
        } 
                       
        iterator erase( iterator x ) // nothrow 
        { 
            remove( x ); 
            return iterator( c_.erase( x.base() ) );
        }
        
        iterator erase( iterator first, iterator last ) // notrow 
        {
            remove( first, last ); 
            return iterator( c_.erase( first.base(), last.base() ) );
        }
        
        void clear()                               
        { 
            remove_all(); 
            c_.clear();
        }
        
    public: // optional sequence requirements
        
        void push_front( T* x )                
        {
            if( 0 == x )
                throw bad_pointer( "Null pointer in 'push_front()'" );
            
            std::auto_ptr<T> ptr( x ); // nothrow
            c_.push_front( x );        // strong, commit
            ptr.release();             // nothrow
        } 
        
        void push_front( const_reference x ) // strong
        {
            push_front( make_clone( x ) ); 
        }
        
        void pop_front() // nothrow
        {
            assert( !empty() ); // wrong ?
            erase( begin() ); 
        }
           
        // assignment to these won't do the expected thing when T is polymorphic
        // maybe only the non-const versions should be allowed
        reference operator[]( size_type n ) // nothrow 
        {
            assert( n < size() );
            return *c_[n]; 
        }
        
        const_reference operator[]( size_type n ) const // nothrow  
        { 
            assert( n < size() ); 
            return *c_[n];
        }
        
        reference at( size_type n )
        {
            if( n < size() )
                throw bad_index( "'at()' out of bounds" );
             
            return *c_[n];
        }
        
        const_reference at( size_type n ) const
        {
            if( n < size() )
                throw bad_index( "'at()' out of bounds" );
             
            return *c_[n]; 
        }

    public: // access interface
        
        std::auto_ptr<T> release_front() // nothrow
        {
            if( empty() ) 
                throw bad_ptr_container_operation( "'release_front()' on empty container" ); 
            std::auto_ptr<T> ptr( c_.front() ); // nothrow 
            c_.pop_front();                     // nothrow
            return ptr; 
        }
        
        std::auto_ptr<T> release_back() // nothrow
        {
            if( empty() ) 
                throw bad_ptr_container_operation( "'release_back()' on empty container" );
            std::auto_ptr<T> ptr( c_.back() ); // nothrow
            c_.pop_back();                     // nothrow
            return ptr; 
        }
        
        std::auto_ptr<T> release( iterator where )
        { 
            if( empty() )
                throw bad_ptr_container_operation( "'release()' on empty container" ); 
            
            std::auto_ptr<T> ptr( &*where ); // nothrow
            c_.erase( where.base() );        // nothrow
            return ptr; 
        }

        void replace( iterator where, T* x ) // strong  
        { 
            if( 0 == x )
                throw bad_pointer( "Null pointer in 'replace()'" );
            
            std::auto_ptr<T> ptr( x );
            
            if( empty() )
                throw bad_ptr_container_operation( "'replace()' on empty container" );

            std::auto_ptr<T> old( &*where ); // nothrow
            *where.base() = x.release();     // nothrow, commit
        }
        
        void replace( size_type idx, T* x ) // strong
        {
            if( 0 == x )
                throw bad_pointer( "Null pointer in 'replace()'" );
            
            std::auto_ptr<T> ptr( x ); 
            
            if( idx >= size() ) 
                throw bad_index( "'replace()' out of bounds" );
            
            std::auto_ptr<T> old( c_[idx] ); // nothrow
            c_[idx] = ptr.release();         // nothrow, commit
        } 
                                  
        template< typename PtrContainer >
        void transfer( iterator before, typename PtrContainer::iterator object, 
                       PtrContainer& from ) // strong
        {
            assert( !from.empty() );
            c_.insert( before.base(), *object.base() ); // strong
            from.c_.erase( object.base() );             // nothrow
        }
        
        template< typename PtrContainer > 
        void transfer( iterator before, typename PtrContainer::iterator first, 
                       typename PtrContainer::iterator last, PtrContainer& from )
        {
            assert( !from.empty() );
            c_.insert( before.base(), first.base(), last.base() ); // strong 
            from.c_.erase( first.base(), last.base() );            // nothrow
        }
        
        template< typename PtrContainer >
        void transfer( iterator before, PtrContainer& from )
        {
            if( from.empty() )
                return;
            c_.insert( before.base(), from.c_.begin(), from.c_.end() ); // strong
            from.c_.clear();                                            // nothrow
        }
             
    public: // special algorithms
        
        void sort()
        {
            sort( std::less<T>() );
        }
        
        template< typename BinaryPredicate >
        void sort( BinaryPredicate pred )
        {
            std::sort( ptr_begin(), ptr_end(), indirected2<BinaryPredicate>( pred ) );
        }
            
        void unique()
        {
            unique( std::equal_to<T>() );
        }
        
        template< typename BinaryPredicate >
        void unique( BinaryPredicate pred )
        {
            iterator i = begin();
            iterator e = end();
            while( i != e ) // bogus due to invalid iterators?
            {
                iterator next_ = next( i );
                
                if( next_ == e )
                    break;
                
                // see std 25.22.8 
                if( pred( *next_, *i ) )
                {
                    erase( next_ );
                    continue;
                }
                else
                {
                    ++i;
                }
            }
        }
         
        void remove( const T& x )
        {
            remove_if( bind( std::equal_to<T>(), _1, cref( x ) ) ); 
        }
        
        template< typename Predicate >
        void remove_if( Predicate pred )
        {
            ptr_iterator last = std::partition( ptr_begin(), ptr_end(), 
                                                std::not1< indirected1<Predicate,T> >( pred ) );
            erase( iterator( last ), end() );
        }
            
    }; // 'reversible_ptr_container'



    template< typename C >
    void swap( reversible_ptr_container<C>& x, reversible_ptr_container<C>& y )
    {
        x.swap( y );
    }

#define BOOST_FORWARD_TYPEDEF( Base ) \
typedef BOOST_DEDUCED_TYPENAME Base::object_type object_type; \
typedef BOOST_DEDUCED_TYPENAME Base::value_type value_type; \
typedef BOOST_DEDUCED_TYPENAME Base::reference reference; \
typedef BOOST_DEDUCED_TYPENAME Base::const_reference const_reference; \
typedef BOOST_DEDUCED_TYPENAME Base::iterator iterator; \
typedef BOOST_DEDUCED_TYPENAME Base::const_iterator const_iterator; \
typedef BOOST_DEDUCED_TYPENAME Base::difference_type difference_type; \
typedef BOOST_DEDUCED_TYPENAME Base::size_type size_type; \
typedef BOOST_DEDUCED_TYPENAME Base::difference_type difference_type; \
typedef BOOST_DEDUCED_TYPENAME Base::allocator_type allocator_type; \
typedef BOOST_DEDUCED_TYPENAME Base::reverse_iterator reverse_iterator; \
typedef BOOST_DEDUCED_TYPENAME Base::const_reverse_iterator const_reverse_iterator; \
typedef BOOST_DEDUCED_TYPENAME Base::ptr_iterator ptr_iterator; \
typedef BOOST_DEDUCED_TYPENAME Base::const_ptr_iterator const_ptr_iterator; \
typedef BOOST_DEDUCED_TYPENAME Base::reverse_ptr_iterator reverse_ptr_iterator; \
typedef BOOST_DEDUCED_TYPENAME Base::reverse_const_ptr_iterator reverse_const_ptr_iterator;

    //
    // two-phase lookup of template functions 
    // is buggy on most compilers, so we use a macro instead
    //
#define BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( PC ) \
                                                    \
    PC( std::auto_ptr<PC> r )                       \
    : Base( r ) { }                                 \
                                                    \
    void operator=( std::auto_ptr<PC> r )           \
    {                                               \
        Base::operator=( r );                       \
    }                                               \
                                                    \
    std::auto_ptr<PC> release()                     \
    {                                               \
      std::auto_ptr<PC> ptr( new PC );              \
      this->swap( *ptr );                           \
      return ptr;                                   \
    }                                               \
                                                    \
    using Base::release;                            \
                                                    \
    std::auto_ptr<PC> clone() const                 \
    {                                               \
       return std::auto_ptr<PC>( new PC( this->begin(), this->end() ) ); \
    }
                                                    
    
    
    } // namespace 'detail'
    
} // namespace 'boost'  

#endif

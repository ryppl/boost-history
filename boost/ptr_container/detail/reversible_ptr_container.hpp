#ifndef BOOST_PTR_CONTAINER_DETAIL_REVERSIBLE_PTR_CONTAINER_HPP
#define BOOST_PTR_CONTAINER_DETAIL_REVERSIBLE_PTR_CONTAINER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

#include <boost/type_traits/remove_pointer.hpp>
#include <boost/mpl/if.hpp>
#include <boost/ptr_container/detail/map_iterator.hpp>
#include <boost/ptr_container/ptr_container_iterator.hpp>
#include <boost/ptr_container/bad_pointer.hpp>
#include <boost/ptr_container/bad_ptr_container_operation.hpp>
#include <boost/ptr_container/detail/scoped_deleter.hpp>
#include <boost/ptr_container/detail/size_undoer.hpp>
#include <boost/ptr_container/make_clone.hpp>
#include <boost/config.hpp>
#include <boost/iterator/indirect_iterator.hpp>
#include <boost/operators.hpp>
#include <boost/cast.hpp>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <exception>
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
    };
    
    template< typename M, typename T >
    struct map_config
    {
        typedef M                                                                            container_type;
        typedef T                                                                            value_type;
        typedef typename ptr_container::detail::map_iterator<typename M::iterator, value_type&> iterator;
        typedef iterator const_iterator;
        typedef typename ptr_container::detail::map_iterator<typename M::reverse_iterator, value_type&> reverse_iterator;
        typedef reverse_iterator const_reverse_iterator;
    };

    template< typename Config >
    class reversible_ptr_container : less_than_comparable< reversible_ptr_container< Config >, 
                                     equality_comparable< reversible_ptr_container< Config > > >
    {
        typedef BOOST_DEDUCED_TYPENAME Config::container_type C;
        C         c_;

    protected:
        C& c__()                { return c_; }
        const C& c__() const    { return c_; }

        public: // typedefs

    typedef  BOOST_DEDUCED_TYPENAME Config::value_type    value_type;
    typedef  value_type*                                  ptr_type;
    typedef  value_type&                                  reference;
    typedef  const value_type&                            const_reference;
    typedef  std::auto_ptr<value_type>                    auto_type;
    typedef const auto_type                               const_auto_type;

    public: // typedefs

        typedef BOOST_DEDUCED_TYPENAME C::iterator                ptr_iter; 
        typedef BOOST_DEDUCED_TYPENAME C::const_iterator          const_ptr_iter;

        ptr_iter dbegin() { return c_.begin(); }
        ptr_iter dend() { return c_.end(); }
        
        typedef BOOST_DEDUCED_TYPENAME C::reverse_iterator        rev_ptr_iter;
        typedef BOOST_DEDUCED_TYPENAME C::const_reverse_iterator  const_rev_ptr_iter;
        typedef ptr_container::detail::scoped_deleter<value_type>
                                                                  scoped_deleter;
        typedef ptr_container::detail::size_undoer<C>             size_undoer;
        
        
        template< typename T >
        struct is_map
        {
            enum{ value =  isAssociative };
        };
        
        typedef  typename Config::iterator        iterator;
        typedef  typename Config::const_iterator  const_iterator;
        //typedef  boost::indirect_iterator<const_ptr_iter>     const_iterator;
        typedef  BOOST_DEDUCED_TYPENAME C::difference_type    difference_type; 
        typedef  BOOST_DEDUCED_TYPENAME C::size_type          size_type;
        typedef  BOOST_DEDUCED_TYPENAME C::difference_type    difference_type;
        typedef  BOOST_DEDUCED_TYPENAME C::allocator_type     allocator_type;
//        typedef  indirect_iterator<rev_ptr_iter>              reverse_iterator;
//        typedef  indirect_iterator<const_rev_ptr_iter>        const_reverse_iterator;
        typedef  typename Config::reverse_iterator           reverse_iterator;
        typedef  typename Config::const_reverse_iterator     const_reverse_iterator;

        typedef  ptr_container_iterator<ptr_iter>                                     ptr_iterator;
        typedef  ptr_container_iterator<const_ptr_iter>                               const_ptr_iterator;
        //typedef  boost::indirect_iterator< ptr_iter, T,
        //                            reference, T* >    ptr_iterator;
        //typedef  boost::indirect_iterator< const_ptr_iter, BOOST_DEDUCED_TYPENAME detail::iterator_traits<const_ptr_iter>::value_type,
        //                            const_reference >    const_ptr_iterator;
        
        // swap_iterator sbegin(), end();
        // non-mutable


    private: // implementation
        struct deleter_ptr
        {
            template< typename U >
            void operator()( const U* u ) { delete u; }
        };

    protected:

        template< typename InputIterator >  
        void     copy( InputIterator first, InputIterator last ) { std::copy( first, last, begin() );}
        
        void     copy( const reversible_ptr_container& r ){ copy( r.begin(), r.end() );}
        
        // consider moving this two to scoped_deleter as constructor
        void     make_clones( scoped_deleter& sd, size_type n, const_reference x  )
        {
            for( size_type i = 0; i != n; i++ )
                sd.add( ::boost::make_clone( x ) );
        }
        
        // consider moving this two to scoped_deleter as constructor
        template< typename InputIterator >
        void     make_clones( scoped_deleter& sd, InputIterator first, InputIterator last  ) // strong
        {
            for( ; first != last; ++first )
                sd.add( ::boost::make_clone( *first ) );
        }

        void     copy_clones_and_release( scoped_deleter& sd ) // nothrow
        {
            assert( std::distance( sd.begin(), sd.end() ) == c_.size() );
            std::copy( sd.begin(), sd.end(), c_.begin() );
            sd.release(); 
        }
        
        template< typename OutputIterator >
        void     copy_clones_and_release( scoped_deleter& sd, OutputIterator where ) // nothrow
        {
            //assert( where + distance( first, last ) < c_.end() );
            std::copy( sd.begin(), sd.end(), where );
            sd.release();
        }
        
        void     insert_clones_and_release( scoped_deleter& sd, iterator where ) // strong
        {
            //
            // 'c_.insert' always provides the strong guarantee for T* elements
            // since a copy constructor of a pointer cannot throw
            //
            c_.insert( where.base(), sd.begin(), sd.end() ); 
            sd.release();
        }
        
        template< typename InputIterator >
        void     clone_assign( InputIterator first, InputIterator last ) // strong 
        {
            scoped_deleter sd( std::distance( first, last ) ); // strong
            make_clones( sd, first, last );                    // strong 
            copy_clones_and_release( sd );                     // nothrow
        }
        
        void     clone_assign( size_type n, const_reference x ) // strong
        {
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
        
        void     remove_all()                            { remove( begin(), end() ); }
        void     remove( iterator i )                    { remove( i, i ); }
        void     remove( iterator first, iterator last ) 
        {
            int ii = 0;
            for( iterator i = first; i != last; ++i, ++ii )
                delete &*i;
        }

        template< typename ForwardIterator >
        ForwardIterator advance( ForwardIterator begin, size_type n ) 
        {
            ForwardIterator iter = begin;
            std::advance( iter, n );
            return iter;
        }
        
    public: // foundation! should be protected!
        reversible_ptr_container( const allocator_type& alloc = allocator_type() ) : c_( alloc )  {}
        
        // overhead: null-initilization of container pointer (very cheap compared to cloning)
        // overhead: 1 heap allocation (very cheap compared to cloning)
        reversible_ptr_container( size_type n, const_reference x, 
                                  const allocator_type& alloc = allocator_type() ) : c_( n, 0, alloc ) 
                                                                                     { clone_assign( n, x ); }
        template< typename PtrContainer >
        reversible_ptr_container( std::auto_ptr<PtrContainer> clone )                 { swap( *clone ); }
        
        // overhead: null-initilization of container pointer (very cheap compared to cloning)
        // overhead: 1 heap allocation (very cheap compared to cloning)
        template< typename InputIterator >
        reversible_ptr_container( InputIterator first, InputIterator last, 
                                  const allocator_type& alloc = allocator_type() ) 
                                                                 : c_( std::distance( first, last ), ptr_type(), alloc ) 
                                                                   { clone_assign( first, last ); }
    public: // map/set foundation - just initialize 'c_'
        
        template< typename Compare >
        explicit reversible_ptr_container( const Compare& comp, const allocator_type& alloc = allocator_type() ) : c_( comp, alloc ) 
        { }
        
        template< typename InputIterator, typename Compare >
        reversible_ptr_container( InputIterator first, InputIterator last, const Compare& comp, 
                                  const allocator_type& alloc = allocator_type() ) : c_( comp, alloc )
        { }
         
    private:  
        reversible_ptr_container( const reversible_ptr_container& r );
        reversible_ptr_container& operator=( const reversible_ptr_container& r );
    
    public:        
        ~reversible_ptr_container()                             { remove_all(); }
        
        template< typename PtrContainer >
        void operator=( std::auto_ptr<PtrContainer> clone )     { swap( *clone ); }
        
        // overhead: 1 heap allocation (very cheap compared to cloning)
        template< typename InputIterator >
        void assign( InputIterator first, InputIterator last ) // strong 
        { 
            difference_type n = std::distance( first, last ); 
            scoped_deleter sd( n );         // strong
            make_clones( sd, first, last ); // strong
            strong_resize_and_remove( n );  // strong
            copy_clones_and_release( sd );  // nothrow
        }
        
       // overhead: 1 heap allocation (very cheap compared to cloning)
        void assign( size_type n, const_reference x ) // strong         
        {
            scoped_deleter sd( n );        // strong
            make_clones( sd, n, x );       // strong
            strong_resize_and_remove( n ); // strong
            copy_clones_and_release( sd ); // nothrow
        }
        
        allocator_type get_allocator() const                   { return c_.get_allocator(); }

    public: // container requirements
        iterator                begin()        { return make_indirect_iterator( c_.begin() ); }
        const_iterator          begin() const  { return make_indirect_iterator( c_.begin() ); }
        iterator                end()          { return make_indirect_iterator( c_.end() ); }
        const_iterator          end() const    { return make_indirect_iterator( c_.end() ); }
        reverse_iterator        rbegin()       { return make_indirect_iterator( c_.rbegin() ); } 
        const_reverse_iterator  rbegin() const { return make_indirect_iterator( c_.rbegin() ); } 
        reverse_iterator        rend()         { return make_indirect_iterator( c_.rend() ); } 
        const_reverse_iterator  rend() const   { return make_indirect_iterator( c_.rend() ); } 
        ptr_iterator            ptr_begin()       { return ptr_iterator( c_.begin() ); }
        const_ptr_iterator      ptr_begin() const { return const_ptr_iterator( c_.begin() ); }
        ptr_iterator            ptr_end()         { return ptr_iterator( c_.end() ); }
        const_ptr_iterator      ptr_end() const   { return const_ptr_iterator( c_.end() ); }
 /*       ptr_iterator            ptr_rbegin()       { return make_ptr_container_iterator( c_.rbegin() ); }
        const_ptr_iterator      ptr_rbegin() const { return make_ptr_container_iterator( c_.rbegin() ); }
        ptr_iterator            ptr_rend()         { return make_ptr_container_iterator( c_.rend() ); }
        const_ptr_iterator      ptr_rend() const   { return make_ptr_container_iterator( c_.rend() ); }
*/
        void      swap( reversible_ptr_container& r )             { c_.swap( r.c_ ); }  
        size_type size() const                                    { return c_.size(); }
        void      resize( size_type sz, const_reference x )  // strong     
        {
            size_type old_size = size();
            if( sz > old_size )
            {
                size_type n = sz - old_size;
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
        size_type max_size() const                                { return c_.max_size(); }
        bool      empty() const                                   { return c_.empty(); }

    public: // optional container requirements
        reference        front()                                  { assert( !empty() ); return *c_.front(); }
        const_reference  front() const                            { assert( !empty() ); return *c_.front(); }
        reference        back()                                   { assert( !empty() ); return *c_.back(); }
        const_reference  back() const                             { assert( !empty() ); return *c_.back(); }

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

        void     push_back( ptr_type x )  // strong               
        {
            if( 0 == x )
                throw bad_pointer();
            
            auto_type ptr( x ); // notrow
            c_.push_back( x );  // strong
            ptr.release();      // nothrow
        }
          
        void     push_back( const_reference x )            { push_back( ::boost::make_clone( x ) ); }
        void     pop_back()                                { assert( !empty() ); erase( --end() ); }
        
        iterator insert( iterator before, ptr_type x ) // strong
        { 
            if( 0 == x )
                throw bad_pointer();
            
            auto_type ptr( x );                                                     // nothrow
            iterator res = make_indirect_iterator( c_.insert( before.base(), x ) ); // strong, commit
            ptr.release();                                                          // nothrow
            return res;
        }
                
        iterator insert( iterator before, const_reference x ) { return insert( before, ::boost::make_clone( x ) ); }

        void     insert( iterator before, size_type n, const_reference x ) // strong 
        {
            scoped_deleter sd( n );                  // strong
            make_clones( sd, n, x );                 // strong 
            insert_clones_and_release( sd, before ); // strong, commit
        }
        
        template< typename InputIterator >
        void     insert( iterator before, InputIterator first, InputIterator last ) // strong 
        {
            scoped_deleter sd( std::distance( first, last ) ); // strong
            make_clones( sd, first, last );                    // strong
            insert_clones_and_release( sd, before );           // strong, commit 
        } 
                       
        iterator erase( iterator before ) 
        { 
            remove( before ); 
            return make_indirect_iterator( c_.erase( before.base() ) );
        }
        
        iterator erase( iterator first, iterator last ) 
        {
            remove( first, last ); 
            return make_indirect_iterator( c_.erase( first.base(), last.base() ) );
        }
        
        void     clear()                               { remove_all(); c_.clear();}
        

    public: // optional sequence requirements
        
        void             push_front( ptr_type x )                
        {
            if( 0 == x )
                throw bad_pointer();
            
            auto_type ptr( x ); // nothrow
            c_.push_front( x ); // strong, commit
            ptr.release();      // nothrow
        } 
        void             push_front( const_reference x )         { push_front( ::boost::make_clone( x ) ); }
        void             pop_front()                             { assert( !empty() ); erase( begin() ); }
           
        // assignment to these won't do the expected thing when T is polymorphic
        // maybe only the non-const versions should be allowed
        reference        operator[]( size_type n )               { assert( n < size() ); return *c_[n]; }
        const_reference  operator[]( size_type n ) const         { assert( n < size() ); return *c_[n]; }
        reference        at( size_type n )                       { assert( n < size() ); return *c_.at( n ); }
        const_reference  at( size_type n ) const                 { assert( n < size() ); return *c_.at( n ); }

    public: // access interface
        auto_type        release_front()                         { if( empty() ) throw bad_ptr_container_operation( "'release_front()' on empty container" ); 
                                                                   auto_type ptr( c_.front() ); c_.pop_front(); return ptr; }
        auto_type        release_back()                          { if( empty() ) throw bad_ptr_container_operation( "'release_back()' on empty container" );
                                                                   auto_type ptr( c_.back() ); c_.pop_back(); return ptr; }
        auto_type        release( iterator where )               { if( empty() ) throw bad_ptr_container_operation( "'release()' on empty container" ); 
                                                                   auto_type ptr( &*where ); c_.erase( where.base() ); return ptr; }

        void             replace( iterator where, T* x )         { auto_type ptr( &*where ); *where = x; } // nothrow
                                  
        template< typename PtrContainer, typename ReleaseIterator >
        void transfer( iterator before, ReleaseIterator object, PtrContainer& from )
        {
            assert( !from.empty() );
            c_.insert( before.base(), *object.base() ); // strong
            from.c_.erase( object.base() );             // nothrow
        }
        
        template< typename PtrContainer, typename ReleaseIterator >
        void transfer( iterator before, ReleaseIterator first, ReleaseIterator last, PtrContainer& from )
        {
            c_.insert( before.base(), first.base(), last.base() ); // strong 
            from.c_.erase( first.base(), last.base() );            // nothrow
        }
        
        template< typename PtrContainer >
        void transfer( iterator before, PtrContainer& from )
        {
            c_.insert( before.base(), from.c_.begin(), from.c_.end() ); // strong
            from.c_.clear();                                            // nothrow
        }
             
    }; // 'reversible_ptr_container'



    template< typename C >
    void swap( reversible_ptr_container<C>& x, reversible_ptr_container<C>& y )
    {
        x.swap( y );
    }

#define BOOST_FORWARD_TYPEDEF( Base ) \
typedef BOOST_DEDUCED_TYPENAME Base::ptr_type ptr_type; \
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
typedef BOOST_DEDUCED_TYPENAME Base::const_ptr_iterator const_ptr_iterator;

    //
    // two-phase lookup of template functions 
    // is buggy on most compilers, so we use a macro instead
    //
#define BOOST_PTR_CONTAINER_RELEASE_AND_CLONE( PC ) \
    std::auto_ptr<PC> release()           \
    {                                     \
      std::auto_ptr<PC> ptr( new PC );    \
      this->swap( *ptr );                       \
      return ptr;                         \
    }                                     \
                                          \
    using Base::release;                  \
                                          \
    std::auto_ptr<PC> clone() const       \
    {                                     \
       return std::auto_ptr<PC>( new PC( this->begin(), this->end() ) ); \
    }
    
    
    } // namespace 'detail'
    
} // namespace 'boost'  

#endif

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
 
#include <libs/ptr_container/test/test_data.hpp>

template< typename C, typename B, typename T >
void associative_container_test();

template< typename C, typename B, typename T >
void associative_container_test()
{   
    BOOST_MESSAGE( "starting associative container test" ); 
    enum { max_cnt = 10, size = 100 };
    C  c;
    BOOST_CHECK( c.size() == 0 );
    
    const C c2;// c.begin(), c.end() );
    BOOST_CHECK( c.size() == c2.size() );
    
    C c3;
    
    BOOST_MESSAGE( "finished construction test" ); 
                 
    BOOST_DEDUCED_TYPENAME C::allocator_type alloc        = c.get_allocator();
    BOOST_DEDUCED_TYPENAME C::iterator i                  = c.begin();
    BOOST_DEDUCED_TYPENAME C::const_iterator ci           = c2.begin();
    BOOST_DEDUCED_TYPENAME C::iterator i2                 = c.end();
    BOOST_DEDUCED_TYPENAME C::const_iterator ci2          = c2.begin();
    BOOST_DEDUCED_TYPENAME C::reverse_iterator ri         = c.rbegin();
    BOOST_DEDUCED_TYPENAME C::const_reverse_iterator cri  = c2.rbegin();
    BOOST_DEDUCED_TYPENAME C::reverse_iterator rv2        = c.rend();
    BOOST_DEDUCED_TYPENAME C::const_reverse_iterator cvr2 = c2.rend();
                             
    BOOST_DEDUCED_TYPENAME C::ptr_iterator ptr_i                  = c.ptr_begin();
    BOOST_DEDUCED_TYPENAME C::ptr_const_iterator ptr_ci           = c2.ptr_begin();
    BOOST_DEDUCED_TYPENAME C::ptr_iterator ptr_i2                 = c.ptr_end();
    BOOST_DEDUCED_TYPENAME C::ptr_const_iterator ptr_ci2          = c2.ptr_begin();
    BOOST_DEDUCED_TYPENAME C::ptr_reverse_iterator ptr_ri         = c.ptr_rbegin();
    BOOST_DEDUCED_TYPENAME C::ptr_const_reverse_iterator ptr_cri  = c2.ptr_rbegin();
    BOOST_DEDUCED_TYPENAME C::ptr_reverse_iterator ptr_rv2        = c.ptr_rend();
    BOOST_DEDUCED_TYPENAME C::ptr_const_reverse_iterator ptr_cvr2 = c2.ptr_rend();
        
    BOOST_MESSAGE( "finished iterator test" ); 
   
    BOOST_DEDUCED_TYPENAME C::size_type s                 = c.size();
    BOOST_DEDUCED_TYPENAME C::size_type s2                = c.max_size();
    BOOST_CHECK( c.size() == size );
    bool b                                                = c.empty();
    BOOST_MESSAGE( "finished accessors test" ); 
    
    T* t = new T;
    //c.insert( c.end(), t );    // == c.end() );
    //c.insert( c.end(), T() );  // == c.end() );
    //c3.insert( c.begin(), c.end() ); 
    c.erase( c.begin() );
    // c.erase( t ); problem with map! Why does this compile for set???
    // we must lack an indiection on key_type
    c3.erase( c3.begin(), c3.end() );
    BOOST_CHECK( c3.empty() );
    c.swap( c3 );
    BOOST_CHECK( !c3.empty() );
    c3.clear();
    BOOST_CHECK( c3.empty() );
    BOOST_MESSAGE( "finished modifiers test" ); 
             
    std::auto_ptr<B> ptr2  = c.release( c.begin() );
    std::auto_ptr<C> ap    = c.release();
    c                      = c2.clone();
    BOOST_MESSAGE( "finished release/clone test" ); 
  
  /*                   
    c.transfer( c3.begin(), c3 );
    c.transfer( c3.begin(), c3.end(), c3 );
    BOOST_CHECK( c3.empty() );
    BOOST_CHECK( !c.empty() );
    c3.transfer( c );
    BOOST_CHECK( !c3.empty() );
    BOOST_CHECK( c.empty() );
    BOOST_MESSAGE( "finished transfer test" );         
  */
      
    BOOST_MESSAGE( "finished set/map interface test" );         
    
    // @todo: make macro with algorithms so that the right erase() is called.
  //  c.unique();
  //  c.unique( std::not_equal_to<T>() );
  //  c.remove( T() );
  //  c.remove_if( std::binder1st< std::equal_to<T> >( T() ) );

    BOOST_MESSAGE( "finished algorithms interface test" );         
}


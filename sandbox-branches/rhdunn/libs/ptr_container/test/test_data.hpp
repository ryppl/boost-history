
#include <boost/ptr_container/make_clone.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/config.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/lexical_cast.hpp>
#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <cstdlib>

using namespace std;
using namespace boost;
using namespace std::rel_ops;

//////////////////////////////////////////////////////////////////////////////
// Test class 1: a class hierarchy
//////////////////////////////////////////////////////////////////////////////

class Base
{
    Base( const Base& );
    Base& operator=( const Base& );
    
public:
    
    Base()          {}
    virtual ~Base() {}
    void     print( ostream& out ) const  { do_print( out); }
    Base*    clone() const                { return do_clone(); }
    void     foo()                        { do_foo(); }
    
    virtual bool less_than( const Base& b ) const
    {
        return true;
    }
    
    virtual bool equal( const Base& b ) const
    {
        return this == &b ;
    }
    
    
    
private:
    virtual void  do_print( ostream& out ) const = 0;
    virtual Base* do_clone() const = 0; 
    virtual void  do_foo() = 0;
};



ostream& operator<<( ostream& out, Base& b )
{
    b.print( out );
    return out;
}


template<>
Base* boost::make_clone<Base>( const Base& b )
{
    return b.clone();
}



bool operator<( const Base& l, const Base& r )
{
    return l.less_than( r );
}



bool operator==( const Base& l, const Base& r )
{
    return l.equal( r );
}



class Derived : public Base
{   
    int i_;
        
    virtual void  do_print( ostream& out ) const
    {
        out << i_;
    }
    
    
    virtual Base* do_clone() const
    {
        return new Derived;
    }
    
    virtual void do_foo() 
    {
        ++i_;
    }
    
public:
    Derived() : i_( rand() )
    { }

    virtual bool less_than( const Base& b ) const
    {
        const Derived& d = dynamic_cast<const Derived&>( b );
        return i_ < d.i_;
    }
};

//////////////////////////////////////////////////////////////////////////////
// Test class 2: a value class 
//////////////////////////////////////////////////////////////////////////////

class Value 
{   
    string s_;
    
public:
    
    Value() : s_( boost::lexical_cast<string>( rand() ) ) 
    {}
    
    string name() const
    {
        return s_;
    }
};



bool operator<( const Value& l, const Value& r )
{
    return l.name() < r.name();
}



bool operator==( const Value& l, const Value& r )
{
    return l.name() == r.name();
}

ostream& operator<<( ostream& out, const Value& v )
{
    return out << v.name() << " ";
}

/////////////////////////////////////////////////////////////////////////////
// test helper functions
/////////////////////////////////////////////////////////////////////////////

template< typename C, typename T >
void typedef_test();

template< typename C, typename T >
void reversible_container_test();

template< typename C, typename T >
void associative_container_test();

template< typename C, typename T >
void algo_test();

template< typename C, typename T >
void typedef_test()
{
//    BOOST_STATIC_ASSERT(( ::boost::is_same< BOOST_DEDUCED_TYPNAME C::iterator, 
//                          ::boost::indirect_iterator<T*> >                  
}


template< typename C, typename T >
void reversible_container_test()
{
    BOOST_MESSAGE( "starting reversible container test" ); 
    enum { max_cnt = 10, size = 100 };
    C  c;
    BOOST_CHECK( c.size() == 0 );
    
    const C  c2( size, T() );
    BOOST_CHECK( c2.size() == size );
    
    C  c3( c.begin(), c.end() );
    BOOST_CHECK( c.size() == c3.size() );
    
    c.assign( c3.begin(), c3.end() );
    BOOST_CHECK( c.size() == c3.size() );
        
    c.assign( size, T() );
    BOOST_CHECK( c.size() == size );
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
    BOOST_MESSAGE( "finished iterator test" ); 
    
    BOOST_DEDUCED_TYPENAME C::size_type s                 = c.size();
    BOOST_DEDUCED_TYPENAME C::size_type s2                = c.max_size();
    c.resize( size, T() );
    BOOST_CHECK( c.size() == size );
    bool b                                                = c.empty();
    
    BOOST_DEDUCED_TYPENAME C::reference r                 = c.front();
    BOOST_DEDUCED_TYPENAME C::const_reference cr          = c2.front();
    BOOST_DEDUCED_TYPENAME C::reference r2                = c.back();
    BOOST_DEDUCED_TYPENAME C::const_reference cr2         = c2.back();
    BOOST_MESSAGE( "finished accessors test" ); 
    
    c.push_back( new T );
    c.push_back( T() );
    c.pop_back(); // is it required that '!empty()'????
    c.insert( c.end(), new T );// == c.end() );
    c.insert( c.end(), T() );  // == c.end() );
    c.insert( c.end(), size, T() );
    c3.insert( c3.end(), c.begin(), c.end() ); 
    c.erase( c.begin() );
    c3.erase( c3.begin(), c3.end() );
    BOOST_CHECK( c3.empty() );
    c.swap( c3 );
    BOOST_CHECK( !c3.empty() );
    c3.clear();
    BOOST_CHECK( c3.empty() );
    BOOST_MESSAGE( "finished modifiers test" ); 
             
    typedef BOOST_DEDUCED_TYPENAME C::auto_type auto_type;
    c.push_back( T() ); c.push_back( T() ); c.push_back( T() ); 
    auto_type ptr       = c.release_back();
    auto_type ptr2      = c.release( c.begin() );
    std::auto_ptr<C> ap = c.release();
    c                   = c2.clone();
    BOOST_MESSAGE( "finished release/clone test" ); 
                     
    c3.push_back( T() );
    c.transfer( c.begin(), c3.begin(), c3 );
    c.transfer( c.end(), c3.begin(), c3.end(), c3 );
    BOOST_CHECK( c3.empty() );
    BOOST_CHECK( !c.empty() );
    c3.transfer( c3.begin(), c );
    BOOST_CHECK( !c3.empty() );
    BOOST_CHECK( c.empty() );
    BOOST_MESSAGE( "finished transfer test" ); 
    
}

template< typename C >
void print( const C& c, const char* msg )
{
    cout << "\n" << msg << " :   ";
//    for_each( c.begin(), c.end(), bind1st( mem_fun_ref( &T::print ), cout ) );
    for( typename C::const_iterator i = c.begin(); i != c.end(); ++i )
    {
        cout << *i << " ";
        //i->print( cout ); 
        //cout << " ";
    }
    
    cout << endl;    
}


template< typename C, typename T >
void associative_container_test()
{   
    BOOST_MESSAGE( "starting associative container test" ); 
    enum { max_cnt = 10, size = 100 };
    C  c;
    BOOST_CHECK( c.size() == 0 );
    
    const C c2( c.begin(), c.end() );
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
    BOOST_MESSAGE( "finished iterator test" ); 
   
    BOOST_DEDUCED_TYPENAME C::size_type s                 = c.size();
    BOOST_DEDUCED_TYPENAME C::size_type s2                = c.max_size();
    BOOST_CHECK( c.size() == size );
    bool b                                                = c.empty();
    BOOST_MESSAGE( "finished accessors test" ); 
    
    T* t = new T;
    c.insert( c.end(), t );    // == c.end() );
    c.insert( c.end(), T() );  // == c.end() );
    c3.insert( c.begin(), c.end() ); 
    c.erase( c.begin() );
    c.erase( t );
    c3.erase( c3.begin(), c3.end() );
    BOOST_CHECK( c3.empty() );
    c.swap( c3 );
    BOOST_CHECK( !c3.empty() );
    c3.clear();
    BOOST_CHECK( c3.empty() );
    BOOST_MESSAGE( "finished modifiers test" ); 
   /*          
    typedef BOOST_DEDUCED_TYPENAME C::auto_type auto_type;
    c.push_back( T() ); c.push_back( T() ); c.push_back( T() ); 
    auto_type ptr       = c.release_back();
    auto_type ptr2      = c.release( c.begin() );
    std::auto_ptr<C> ap = c.release();
    c                   = c2.clone();
    BOOST_MESSAGE( "finished release/clone test" ); 
                     
    c3.push_back( T() );
    c.transfer( c.begin(), c3.begin(), c3 );
    c.transfer( c.end(), c3.begin(), c3.end(), c3 );
    BOOST_CHECK( c3.empty() );
    BOOST_CHECK( !c.empty() );
    c3.transfer( c3.begin(), c );
    BOOST_CHECK( !c3.empty() );
    BOOST_CHECK( c.empty() );
    BOOST_MESSAGE( "finished transfer test" );         
    */
}



template< typename C, typename T >
void algo_test()
{
    enum { size = 5 };
    srand( 0 );
    C c;
    
    for( int i = 0; i != size; ++ i )
        c.push_back( T() );
        
    copy( c.begin(), c.end(), std::ostream_iterator<T>( std::cout ) ); 
    std::sort( c.ptr_begin(), c.ptr_end() );
    // std::sort( c.begin(), c.end() ); does the job, but much slower!
    cout << endl;     
    copy( c.begin(), c.end(), std::ostream_iterator<T>( std::cout ) ); 
}


template< typename C, typename T >
void algo_test_polymorphic()
{
    enum { size = 5 };
    srand( 0 );
    C c;
    
    for( int i = 0; i != size; ++ i )
        c.push_back( T() );
   
    print<C>( c, "before" );
    std::sort( c.ptr_begin(), c.ptr_end() );
    //std::sort( c.begin(), c.end() );
    // won't work snice T is not copyable
    print<C>( c, "after" );
}


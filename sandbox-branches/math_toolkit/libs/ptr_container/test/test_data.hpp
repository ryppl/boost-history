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
//using namespace std::rel_ops;

//////////////////////////////////////////////////////////////////////////////
// Test class 1: a class hierarchy
//////////////////////////////////////////////////////////////////////////////

class Base
{
    Base( const Base& );
    Base& operator=( const Base& );
    
public:
    
    Base()          {}
    virtual ~Base()                       { /** write debug code here */ }
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


//
// @note: no need for specialization of
//       boost::make_clone; we rely on
//       argument dependent lookup
//
Base* make_clone( const Base& b )
{
    return b.clone();
}



bool operator<( const Base& l, const Base& r )
{
    return l.less_than( r );
}



bool operator>( const Base& l, const Base& r )
{
    return !l.less_than( r ) && r.less_than( l );
}



bool operator==( const Base& l, const Base& r )
{
    return l.equal( r );
}



bool operator!=( const Base& l, const Base& r )
{
    return !l.equal( r );
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
    
    ~Value()      { /** debug code here */ }
    
    string name() const
    {
        return s_;
    }
};



bool operator<( const Value& l, const Value& r )
{
    return l.name() < r.name();
}



bool operator>( const Value& l, const Value& r )
{
    return l.name() > r.name();
}



bool operator==( const Value& l, const Value& r )
{
    return l.name() == r.name();
}



bool operator!=( const Value& l, const Value& r )
{
    return l.name() != r.name();
}



ostream& operator<<( ostream& out, const Value& v )
{
    return out << v.name() << " ";
}



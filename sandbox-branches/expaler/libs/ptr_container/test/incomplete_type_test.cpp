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
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_map.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <boost/utility.hpp>
#include <algorithm>
#include <iostream>

using namespace std;
using namespace boost;

//
// Forward declare 'make_clone()' to be able
// use clonability of 'Composite' inline in the class.
// This is normally not needed when using .hpp + .cpp files.
//
class Composite;
Composite* make_clone( const Composite& );


class Composite 
{
    typedef ptr_vector<Composite>         composite_t;
    typedef composite_t::iterator         iterator;
    typedef composite_t::const_iterator   const_iterator;
    typedef composite_t::size_type        size_type; 
    composite_t                           elements_;

    //
    // only used internally for 'clone()'
    //
    Composite( const Composite& r ) : elements_( r.elements_.clone() )
    { }
    
    //
    // this class is not Copyable nor Assignable 
    //
    void operator=( const Composite& );

public:
    Composite()
    { }
    
    //
    // of course detructor is virtual
    //
    virtual ~Composite()
    { }
 
    //
    // one way of adding new elements
    //
    void add( Composite* c )
    {
        elements_.push_back( c );
    }
    
    //
    // second way of adding new elements
    //
    void add( Composite& c )
    {
        elements_.push_back( c );
    }

    void remove( iterator where )
    {
        elements_.erase( where );
    }
  
    //
    // recusively count the elements 
    //
    size_type size() const
    {
        size_type res = 0;
        for( const_iterator i = elements_.begin(); i != elements_.end(); ++i )
            res += i->size();
        return 1 /* this */ + res;
    }
    
    void foo()
    {
        do_foo();
        for( iterator i = elements_.begin(); i != elements_.end(); ++i )
            i->foo();
    }
    
    //
    // this class is clonable and this is the callback for 'make_clone()'
    //
    virtual Composite* clone() const
    {
        return new Composite( *this );
    }
    
private:
    virtual void do_foo() 
    {
        cout << "composite base" << "\n";
    }
};

//
// make 'Composite' clonable; note that we do not need to overload
// the function in the 'boost' namespace.
//
Composite* make_clone( const Composite& c )
{
    return c.clone();
}


class ConcreteComposite1 : public Composite
{
    virtual void do_foo() 
    {
        cout << "composite 1" << "\n"; 
    }
  
public:
    virtual Composite* clone() const
    {
        return new ConcreteComposite1();
    }
};


class ConcreteComposite2 : public Composite
{
    virtual void do_foo() 
    {
        cout << "composite 2" << "\n";
    }
  
public:
    virtual Composite* clone() const
    {
        return new ConcreteComposite2();
    }
};

void test_incomplete()
{
    Composite c;
    c.add( new ConcreteComposite1 );
    c.add( new ConcreteComposite2 ); 
    BOOST_CHECK( c.size() == 2 );
    c.add( make_clone( c ) ); // add c to itself
    BOOST_CHECK( c.size() == 5 );
    c.add( c );               // add c to itself
    BOOST_CHECK( c.size() == 11 );
    c.foo();     
}

using namespace boost;

int test_main( int, char*[] )
{
    test_incomplete();
    return 0;
}

//
// todo: remake example with shared_ptr
//

//
// rationale for & overloads: the novice programmer might see that he gets a compile errro when he
// tries to add something: c.push_back( obj ); So he fixes it: c.push_back( &obj ); Now his program compiles.
//

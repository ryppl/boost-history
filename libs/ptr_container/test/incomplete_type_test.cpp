
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

class Composite 
{
    typedef ptr_vector<Composite>         composite_t;
    typedef composite_t::iterator         iterator;
    typedef composite_t::const_iterator   const_iterator;
    typedef composite_t::size_type        size_type; 
    composite_t                           elements_;

    Composite( const Composite& r ) : elements_( r.elements_.clone() )
    { }
    
    void operator=( const Composite& );

public:
    Composite()
    { }
    
    virtual ~Composite()
    { }
 
    void add( Composite* c )
    {
        elements_.push_back( c );
    }
    
    void add( Composite& c )
    {
        elements_.push_back( c );
    }

    void remove( iterator where )
    {
        elements_.erase( where );
    }
  
    size_type size() const
    {
        size_type res = elements_.size();
        for( const_iterator i = elements_.begin(); i != elements_.end(); ++i )
            res += i->size();
        return res;
    }
    
    void foo()
    {
        do_foo();
        for( iterator i = elements_.begin(); i != elements_.end(); ++i )
            i->foo();
    }
    
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
    c.add( c ); // add c to itself
    BOOST_CHECK( c.size() == 5 );
    c.add( c ); // ditto
    BOOST_CHECK( c.size() == 11 );
    c.foo();     
}

using namespace boost;

int test_main( int, char*[] )
{
    test_incomplete();
    return 0;
}



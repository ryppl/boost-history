#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/progress.hpp>
#include <libs/ptr_container/test/test_data.hpp>

using namespace boost;
using namespace std;


typedef shared_ptr<Base> PolyPtr;

struct PolyPtrOps
{
  void operator()( const PolyPtr& a )
    { a->foo(); }
};

struct less_than
{
    bool operator()( const PolyPtr& l, const PolyPtr& r )
    {
        return *l < *r;
    }
};


int test_main( int, char*[] )
{
    enum { size = 20000 };
    vector< PolyPtr > svec;
    
    ptr_vector<Base>  pvec;
    
    {
        progress_timer timer;
        for( int i = 0; i < size; ++i )
            svec.push_back( PolyPtr( new Derived ) );
        
    }

    {
        progress_timer timer;
        for( int i = 0; i < size; ++i )
        {
            pvec.push_back( new Derived );        
        }
    }
    
    {
        progress_timer timer;
        for_each( svec.begin(), svec.end(), PolyPtrOps() );
    }
    
    {
        progress_timer timer;
        for_each( pvec.begin(), pvec.end(), mem_fun_ref( &Base::foo ) );
    }
        
    {
        progress_timer timer;
        sort( svec.begin(), svec.end(), less_than() );
    }
    
    {
        progress_timer timer;
        sort( pvec.ptr_begin(), pvec.ptr_end() );
    }
            
    
    return 0;
}

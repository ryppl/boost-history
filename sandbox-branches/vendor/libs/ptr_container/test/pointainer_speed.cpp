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


int main( int, char*[] )
{
    enum { size = 35 };
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
    
     vector<Base*> copy1;
        for( ptr_vector<Base>::ptr_iter i = pvec.dbegin(); i != pvec.dend(); ++ i )
         copy1.push_back( *i ); 
      
    {
        progress_timer timer;
         sort( pvec.ptr_begin(), pvec.ptr_end() );

       }
  vector<Base*> copy2;
        for( ptr_vector<Base>::ptr_iter i = pvec.dbegin(); i != pvec.dend(); ++ i )
         copy2.push_back( *i ); 
   
    
    for( int i =  0; i < copy1.size(); ++i )
    {
        bool found = false;
        for( int j = 0; j < copy1.size(); ++ j )
        if( copy1[i] == copy2[j] )
            found = true;

        if( !found )
            cout << copy1[i] << endl;
    }   

    assert( pvec.size() == size );
            
    
    return 0;
}

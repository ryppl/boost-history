#include <libs/ptr_container/test/test_data.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <iostream>
#include <queue>
#include <stack>

using namespace std;
using namespace boost;



int test_main( int, char*[] )
{
    test_adapter();
    return 0;
}

//
// rationale for & overloads: the novice programmer might see that he gets a compile errro when he
// tries to add something: c.push_back( obj ); So he fixes it: c.push_back( &obj ); Now his program compiles.
//

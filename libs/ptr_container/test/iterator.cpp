#include <boost/ptr_container/shallow_copy_indirection_iterator.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <algorithm>

using namespace std;
using namespace boost;

int main( int, char*[] )
{
    srand( 0 );
    vector<int*> v;
    // problem: if iterator is a pointer, then we try to overlead builtin operator<(int*,int*)!
    // not a big problem though. However, can this lead to unwanted behavior
    // for X*? Can a namespace help.
    //
    typedef vector<int*>::iterator                          ptr_iterator;
    typedef ptr_container_iterator<ptr_iterator>            shallow_iterator;
    
    for( int i = 0; i < 10; i++ )
        v.push_back( new int( rand() ) );
    
    for( ptr_iterator i = v.begin(); i < v.end(); ++i )
    {
        cout << *i << " " << **i << " ";
    }

    cout << "\n before sorting:" << endl;
    copy( make_ptr_container_iterator( v.begin() ), make_ptr_container_iterator( v.end() ), ostream_iterator<int>( cout, " " ) );
    cout << "\n sorting:" << endl;
    sort( make_ptr_container_iterator( v.begin() ), make_ptr_container_iterator( v.end() ) );
    cout << "\n after sorting:" << endl;
    copy( make_ptr_container_iterator( v.begin() ), make_ptr_container_iterator( v.end() ), ostream_iterator<int>( cout, " " ) );
        
    return 0;
}

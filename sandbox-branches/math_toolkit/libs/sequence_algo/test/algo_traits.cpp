// (C) Copyright Thorsten Ottosen 2002. Permission to copy, use, modify,
// sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.

#include <boost/sequence_algo/algo_traits.hpp>

#include <cassert>
#include <vector>

int main()
{
    using namespace std;
    using namespace boost;
    
    typedef algo_traits< vector<int> >::iterator  stl_iterator;
    typedef algo_traits< int[5] >::iterator       array_iterator;
    assert( is_array<int[5]>::value );
    assert( !is_array< vector<int> >::value );
    assert( typeid( array_iterator ) == typeid( int* ) );
    return 0;
}

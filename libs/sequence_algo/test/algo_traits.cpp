#include "algo_traits.hpp"

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
    return 0;
}

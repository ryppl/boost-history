#include <libs/ptr_container/test/associative_test_data.hpp>
#include <libs/ptr_container/test/concept_check_data.hpp>
#include <boost/ptr_container/ptr_map.hpp>

int test_main( int, char*[] )
{
    typedef_test< ptr_map<int, Base>, Derived >();
    typedef_test< ptr_map<int, Value>, Value >();

    associative_container_test< ptr_map<int, Base>, Derived >();
    associative_container_test< ptr_map<int, Value>, Value >();
    
    typedef_test< ptr_multimap<int, Base>, Derived >();
    typedef_test< ptr_multimap<int, Value>, Value >();

    associative_container_test< ptr_multimap<int, Base>, Derived >();
    associative_container_test< ptr_multimap<int, Value>, Value >();
    

    return 0;
}




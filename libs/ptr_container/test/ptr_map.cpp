#include <libs/ptr_container/test/test_data.hpp>
#include <boost/ptr_container/ptr_map.hpp>

int test_main( int, char*[] )
{
    typedef_test< ptr_map<int, Base>, Derived >();
    typedef_test< ptr_map<int, Value>, Value >();

    associative_container_test< ptr_map<int, Base>, Derived >();
    associative_container_test< ptr_map<int, Value>, Value >();
    
//    algo_test< ptr_list<Value>, Value >();
//    algo_test_polymorphic< ptr_list<Base>, Derived >();

    return 0;
}




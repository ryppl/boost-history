#include <libs/ptr_container/test/test_data.hpp>
#include <boost/ptr_container/ptr_set.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>

int test_main( int, char*[] )
{    
    typedef_test< ptr_set<Base>, Derived >();
    typedef_test< ptr_set<Value>, Value >();

    associative_container_test< ptr_set<Base>, Derived >();
    associative_container_test< ptr_set<Value>, Value >();
    
/*
    algo_test< ptr_vector<Value>, Value >();
    algo_test_polymorphic< ptr_vector<Base>, Derived >();
*/
    return 0;
}



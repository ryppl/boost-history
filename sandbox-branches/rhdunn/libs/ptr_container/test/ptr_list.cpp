#include <libs/ptr_container/test/test_data.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>

int test_main( int, char*[] )
{
    
    typedef_test< ptr_list<Base>, Derived >();
    typedef_test< ptr_list<Value>, Value >();

    reversible_container_test< ptr_list<Base>, Derived >();
    reversible_container_test< ptr_list<Value>, Value >();

//    algo_test< ptr_list<Value>, Value >();
//    algo_test_polymorphic< ptr_list<Base>, Derived >();

    return 0;
}



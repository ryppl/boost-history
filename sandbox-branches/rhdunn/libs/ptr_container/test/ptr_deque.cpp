#include <libs/ptr_container/test/test_data.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>

int test_main( int, char*[] )
{
    
    typedef_test< ptr_deque<Base>, Derived >();
    typedef_test< ptr_deque<Value>, Value >();

    reversible_container_test< ptr_deque<Base>, Derived >();
    reversible_container_test< ptr_deque<Value>, Value >();

    algo_test< ptr_deque<Value>, Value >();
    algo_test_polymorphic< ptr_deque<Base>, Derived >();

    return 0;
}



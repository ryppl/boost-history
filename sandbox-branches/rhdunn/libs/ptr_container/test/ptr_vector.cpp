#include <libs/ptr_container/test/test_data.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
//#include <boost/test/included/test_exec_monitor.hpp>

int main( int, char*[] )
{
 /*   
    typedef_test< ptr_vector<Base>, Derived >();
    typedef_test< ptr_vector<Value>, Value >();

    reversible_container_test< ptr_vector<Base>, Derived >();
    reversible_container_test< ptr_vector<Value>, Value >();
*/
    algo_test< ptr_vector<Value>, Value >();
    algo_test_polymorphic< ptr_vector<Base>, Derived >();
    return 0;
}

#include <libs/ptr_container/test/sequence_test_data.hpp>
#include <libs/ptr_container/test/concept_check_data.hpp>
#include <boost/ptr_container/ptr_deque.hpp>

int test_main( int, char*[] )
{
    
    typedef_test< ptr_deque<Base>, Derived >();
    typedef_test< ptr_deque<Value>, Value >();

    reversible_container_test< ptr_deque<Base>, Derived >();
    reversible_container_test< ptr_deque<Value>, Value >();

//    algo_test< ptr_deque<Value>, Value >();
//    algo_test_polymorphic< ptr_deque<Base>, Derived >();

    return 0;
}



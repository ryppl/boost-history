/**
 * Pointer-Container Library
 * (C) Thorsten Ottosen 2003-2004
 * 
 * Permission to copy, use, modify,
 * sell and distribute this software is granted provided this
 * copyright notice appears in all copies. This software is provided
 * "as is" without express or implied warranty, and with no claim as
 * to its suitability for any purpose.
 *
 * See http://www.boost.org/libs/ptr_container for documentation.
 *
 */
 
#include <libs/ptr_container/test/sequence_test_data.hpp>
#include <libs/ptr_container/test/concept_check_data.hpp>
#include <boost/ptr_container/ptr_deque.hpp>

int test_main( int, char*[] )
{
    
    typedef_test< ptr_deque<Base>, Derived >();
    typedef_test< ptr_deque<Value>, Value >();

    reversible_container_test< ptr_deque<Base>, Base, Derived >();
    reversible_container_test< ptr_deque<Value>, Value, Value >();

//    algo_test< ptr_deque<Value>, Value >();
//    algo_test_polymorphic< ptr_deque<Base>, Derived >();

    return 0;
}



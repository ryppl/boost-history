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
 
#include <libs/ptr_container/test/associative_test_data.hpp>
#include <libs/ptr_container/test/concept_check_data.hpp>
#include <boost/ptr_container/ptr_set.hpp>

int test_main( int, char*[] )
{    
    typedef_test< ptr_set<Base>, Derived >();
    typedef_test< ptr_set<Value>, Value >();

    associative_container_test< ptr_set<Base>, Base, Derived >();
    associative_container_test< ptr_set<Value>, Value, Value >();
    
    typedef_test< ptr_multiset<Base>, Derived >();
    typedef_test< ptr_multiset<Value>, Value >();

    associative_container_test< ptr_multiset<Base>, Base, Derived >();
    associative_container_test< ptr_multiset<Value>, Value, Value >();

/*
    algo_test< ptr_vector<Value>, Value >();
    algo_test_polymorphic< ptr_vector<Base>, Derived >();
*/
    //c.insert( T() );
    //c.insert( t );

    return 0;
}



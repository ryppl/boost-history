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
#include <boost/ptr_container/ptr_map.hpp>
#include <string>

using namespace std;

int test_main( int, char*[] )
{
    typedef_test< ptr_map<int, Base>, Derived >();
    typedef_test< ptr_map<int, Value>, Value >();

    associative_container_test< ptr_map<int, Base>, Base, Derived >();
    associative_container_test< ptr_map<int, Value>, Value, Value >();
    
    typedef_test< ptr_multimap<int, Base>, Derived >();
    typedef_test< ptr_multimap<int, Value>, Value >();

    associative_container_test< ptr_multimap<int, Base>, Base, Derived >();
    associative_container_test< ptr_multimap<int, Value>, Value, Value >();
    
    string joe   = "joe";
    string brian = "brian";
    
    ptr_map<string,int> m;
    m.insert( joe, new int( 4 ) );
    m.insert( brian, new int( 6 ) );
    m[ joe ]   += 56;
    m[ brian ] += 10;

    try
    {
        m[ "hans" ] = 4;
    }
    catch( const bad_ptr_container_operation& )
    { }

    return 0;
}




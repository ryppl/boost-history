#include <boost/container_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>
#include <iostream>

using namespace boost;
using namespace std;

void check_array()
{
    typedef int   array_t[9];
    int           my_array[9]  = { 1,2,3,4,5,6,7,8,9 };
    const array_t ca           = { 1, 4 };
    BOOST_STATIC_CONSTANT( std::size_t, the_size = sizeof( sizer( my_array ) ) );
    BOOST_STATIC_ASSERT(( the_size == 9 ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<array_t>::value_type, int >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<array_t>::iterator, int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<array_t>::const_iterator, const int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<array_t>::difference_type, std::ptrdiff_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<array_t>::size_type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<array_t>::result_iterator, int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<const array_t>::result_iterator, const int* >::value ));
    //typedef container_traits<const array_t>::result_iterator iter;
    //cout << typeid( iter() ).name() << endl;
    
    BOOST_CHECK( begin( my_array ) == my_array );
    BOOST_CHECK( end( my_array )   == my_array + size( my_array ) );
    BOOST_CHECK( empty( my_array ) == false );
    BOOST_CHECK( size( my_array ) == sizeof( sizer( my_array ) ) );
}



int test_main( int, char*[] )
{
    check_array();

    return 0;
}



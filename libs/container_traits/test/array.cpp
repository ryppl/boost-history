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
    BOOST_STATIC_ASSERT(( is_same< container_value_type<array_t>::type, int >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_iterator<array_t>::type, int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_const_iterator<array_t>::type, const int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_difference_type<array_t>::type, std::ptrdiff_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_size_type<array_t>::type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_result_iterator<array_t>::type, int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_result_iterator<const array_t>::type, const int* >::value ));
    
    BOOST_STATIC_ASSERT(( is_same< container_value_type<const array_t>::type, const int >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_iterator<const array_t>::type, const int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_const_iterator<const array_t>::type, const int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_difference_type<const array_t>::type, std::ptrdiff_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_size_type<const array_t>::type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_result_iterator<const array_t>::type, const int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_result_iterator<const array_t>::type, const int* >::value ));

    //typedef container<const array_t>::result_iterator iter;
    //cout << typeid( iter() ).name() << endl;
    
    BOOST_CHECK( begin( my_array ) == my_array );
    BOOST_CHECK( end( my_array )   == my_array + size( my_array ) );
    BOOST_CHECK( empty( my_array ) == false );
    BOOST_CHECK( size( my_array ) == sizeof( sizer( my_array ) ) );
    
    BOOST_CHECK( begin( ca ) == ca );
    BOOST_CHECK( end( ca )   == ca + size( ca ) );
    BOOST_CHECK( empty( ca ) == false );
    BOOST_CHECK( size( ca ) == sizeof( sizer( ca ) ) );

}



int test_main( int, char*[] )
{
    check_array();

    return 0;
}



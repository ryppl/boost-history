#include <boost/container_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>
#include <iostream>
#include <boost/container_traits/config.hpp>

using namespace boost;
using namespace std;

void check_array()
{
    typedef int   array_t[9];
    int           my_array[9]  = { 1,2,3,4,5,6,7,8,9 };
    const array_t ca           = { 1,2,3,4,5,6,7,8,10 };

#ifndef BOOST_CT_NO_STATIC_ASSERT 
    BOOST_STATIC_CONSTANT( std::size_t, the_size = sizeof( sizer( my_array ) ) );
    BOOST_STATIC_ASSERT(( the_size == 9 ));
    BOOST_STATIC_ASSERT(( is_same< value_type_of<array_t>::type, int >::value ));
    BOOST_STATIC_ASSERT(( is_same< iterator_of<array_t>::type, int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< const_iterator_of<array_t>::type, const int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< difference_type_of<array_t>::type, std::ptrdiff_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< size_type_of<array_t>::type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< result_iterator_of<array_t>::type, int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< result_iterator_of<const array_t>::type, const int* >::value ));
    
    BOOST_STATIC_ASSERT(( is_same< value_type_of<const array_t>::type, const int >::value ));
    BOOST_STATIC_ASSERT(( is_same< iterator_of<const array_t>::type, const int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< const_iterator_of<const array_t>::type, const int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< difference_type_of<const array_t>::type, std::ptrdiff_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< size_type_of<const array_t>::type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< result_iterator_of<const array_t>::type, const int* >::value ));
    BOOST_STATIC_ASSERT(( is_same< result_iterator_of<const array_t>::type, const int* >::value ));
#endif
    //typedef container<const array_t>::result_iterator iter;
    //cout << typeid( iter() ).name() << endl;
    
    BOOST_CHECK( begin( my_array ) == my_array );
    BOOST_CHECK( end( my_array )   == my_array + size( my_array ) );
    BOOST_CHECK( empty( my_array ) == false );
    //BOOST_CHECK( size( my_array ) == sizeof( sizer( my_array ) ) );

    BOOST_CHECK( begin( ca ) == ca );
    BOOST_CHECK( end( ca )   == ca + size( ca ) );
    BOOST_CHECK( empty( ca ) == false );
    //BOOST_CHECK( size( ca ) == sizeof( sizer( ca ) ) );

}



int test_main( int, char*[] )
{
    check_array();

    return 0;
}



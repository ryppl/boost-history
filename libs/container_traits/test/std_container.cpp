#include <boost/container_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>
#include <vector>

using namespace boost;
using boost::unit_test_framework::test_suite;

void check_std_container()
{
    typedef std::vector<int> vec_t;
    vec_t                    vec;
    vec.push_back( 3 ); vec.push_back( 4 );
    const vec_t              cvec( vec ); 
    
    BOOST_STATIC_ASSERT(( is_same< container_value_type<vec_t>::type, vec_t::value_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_iterator<vec_t>::type, vec_t::iterator >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_const_iterator<vec_t>::type, vec_t::const_iterator >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_difference_type<vec_t>::type, vec_t::difference_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_size_type<vec_t>::type, vec_t::size_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_result_iterator<vec_t>::type, vec_t::iterator >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_result_iterator<const vec_t>::type, vec_t::const_iterator >::value ));
    
    BOOST_STATIC_ASSERT(( is_same< container_value_type<const vec_t>::type, vec_t::value_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_iterator<const vec_t>::type, vec_t::iterator >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_const_iterator<const vec_t>::type, vec_t::const_iterator >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_difference_type<const vec_t>::type, vec_t::difference_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_size_type<const vec_t>::type, vec_t::size_type >::value ));

    BOOST_CHECK( begin( vec ) == vec.begin() );
    BOOST_CHECK( end( vec )   == vec.end() );
    BOOST_CHECK( empty( vec ) == vec.empty() );
    BOOST_CHECK( size( vec ) == vec.size() );
    
    BOOST_CHECK( begin( cvec ) == cvec.begin() );
    BOOST_CHECK( end( cvec )   == cvec.end() );
    BOOST_CHECK( empty( cvec ) == cvec.empty() );
    BOOST_CHECK( size( cvec ) == cvec.size() );

}



int test_main( int, char*[] )
{
    check_std_container();

    return 0;
}


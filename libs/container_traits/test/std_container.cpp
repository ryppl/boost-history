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
    
    BOOST_STATIC_ASSERT(( is_same< container_traits<vec_t>::value_type, vec_t::value_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<vec_t>::iterator, vec_t::iterator >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<vec_t>::const_iterator, vec_t::const_iterator >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<vec_t>::difference_type, vec_t::difference_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<vec_t>::size_type, vec_t::size_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<vec_t>::result_iterator, vec_t::iterator >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<const vec_t>::result_iterator, vec_t::const_iterator >::value ));
    
    BOOST_CHECK( begin( vec ) == vec.begin() );
    BOOST_CHECK( end( vec )   == vec.end() );
    BOOST_CHECK( empty( vec ) == vec.empty() );
    BOOST_CHECK( size( vec ) == vec.size() );
}



int test_main( int, char*[] )
{
    check_std_container();

    return 0;
}


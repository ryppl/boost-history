#include <boost/container_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>
#include <vector>

using namespace boost;
using boost::unit_test_framework::test_suite;

void check_iterator_pair()
{
    typedef std::vector<int> vec_t;
    vec_t                    vec;
    vec.push_back( 4 );
    typedef std::pair<vec_t::iterator,vec_t::iterator>
                             pair_t;
    typedef std::pair<vec_t::const_iterator,vec_t::const_iterator>
                             const_pair_t;
    typedef const pair_t     const_pair_tt;
    pair_t                   pair       = std::make_pair( begin( vec ), end( vec ) );
    const_pair_t             const_pair = std::make_pair( begin( vec ), end( vec ) );
    const_pair_tt            constness_pair( pair );
    
    BOOST_STATIC_ASSERT(( is_same< container_value_type<pair_t>::type, 
                          detail::iterator_traits<pair_t::first_type>::value_type>::value ));
    BOOST_STATIC_ASSERT(( is_same< container_iterator<pair_t>::type, pair_t::first_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_const_iterator<pair_t>::type, pair_t::first_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_difference_type<pair_t>::type,                           
                          detail::iterator_traits<pair_t::first_type>::difference_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_size_type<pair_t>::type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_result_iterator<pair_t>::type, pair_t::first_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_result_iterator<const_pair_t>::type, const_pair_t::first_type >::value ));
    
    BOOST_STATIC_ASSERT(( is_same< container_value_type<const_pair_tt>::type, 
                          detail::iterator_traits<const_pair_t::first_type>::value_type>::value ));
    BOOST_STATIC_ASSERT(( is_same< container_iterator<const_pair_tt>::type, const_pair_tt::first_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_const_iterator<const_pair_tt>::type, const_pair_tt::first_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_difference_type<const_pair_tt>::type,                           
                          detail::iterator_traits<const_pair_tt::first_type>::difference_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_size_type<const_pair_tt>::type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_result_iterator<const_pair_tt>::type, const_pair_tt::first_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_result_iterator<const_pair_tt>::type, const_pair_tt::first_type >::value ));

    BOOST_CHECK( begin( pair ) == pair.first );
    BOOST_CHECK( end( pair )   == pair.second );
    BOOST_CHECK( empty( pair ) == (pair.first == pair.second) );
    BOOST_CHECK( size( pair )  == std::size_t( std::distance( pair.first, pair.second ) ) );
    
    BOOST_CHECK( begin( const_pair ) == const_pair.first );
    BOOST_CHECK( end( const_pair )   == const_pair.second );
    BOOST_CHECK( empty( const_pair ) == (const_pair.first == const_pair.second) );
    BOOST_CHECK( size( const_pair )  == std::size_t( std::distance( const_pair.first, const_pair.second ) ) );

    BOOST_CHECK( begin( constness_pair ) == constness_pair.first );
    BOOST_CHECK( end( constness_pair )   == constness_pair.second );
    BOOST_CHECK( empty( constness_pair ) == (constness_pair.first == const_pair.second) );
    BOOST_CHECK( size( constness_pair )  == std::size_t( std::distance( constness_pair.first, constness_pair.second ) ) );

}



int test_main( int, char*[] )
{
    check_iterator_pair();

    return 0;
}


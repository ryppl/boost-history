#include <boost/container_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/test/unit_test.hpp>
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
    pair_t                   pair       = std::make_pair( begin( vec ), end( vec ) );
    const_pair_t             const_pair = std::make_pair( begin( vec ), end( vec ) );
    
    BOOST_STATIC_ASSERT(( is_same< container_traits<pair_t>::value_type, 
                          detail::iterator_traits<pair_t::first_type>::value_type>::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<pair_t>::iterator, pair_t::first_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<pair_t>::const_iterator, pair_t::first_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<pair_t>::difference_type,                           
                          detail::iterator_traits<pair_t::first_type>::difference_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<pair_t>::size_type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<pair_t>::result_iterator, pair_t::first_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<const_pair_t>::result_iterator, const_pair_t::first_type >::value ));
    
    BOOST_CHECK( begin( pair ) == pair.first );
    BOOST_CHECK( end( pair )   == pair.second );
    BOOST_CHECK( empty( pair ) == (pair.first == pair.second) );
    BOOST_CHECK( size( pair )  == (std::distance( pair.first, pair.second ) == 0) );
    
    BOOST_CHECK( begin( const_pair ) == const_pair.first );
    BOOST_CHECK( end( const_pair )   == const_pair.second );
    BOOST_CHECK( empty( const_pair ) == (const_pair.first == const_pair.second) );
    BOOST_CHECK( size( const_pair )  == (std::distance( const_pair.first, const_pair.second ) == 0) );

}



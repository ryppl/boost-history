#include <boost/container_traits.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>
#include <boost/config.hpp>
#include <vector>
#include <fstream>
#include <algorithm>

template< typename Container, typename OutputIterator >
void copy( const Container& c, OutputIterator out )
{
    std::copy( boost::begin( c ), boost::end( c ), out );
}


using namespace boost;
using boost::unit_test_framework::test_suite;

void check_iterator()
{
    const char*                         this_file = "iterator.cpp";
    std::ifstream                       in( this_file );          
    typedef std::istream_iterator<char> iterator_t;
    typedef std::vector<std::string>    vec_t;
    
    iterator_t iter     = iterator_t( in );
    vec_t      vec;
    
    BOOST_STATIC_ASSERT(( is_same< container_traits<iterator_t>::value_type, 
                          detail::iterator_traits<iterator_t>::value_type>::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<iterator_t>::iterator, iterator_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<iterator_t>::const_iterator, iterator_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<iterator_t>::difference_type,                           
                          detail::iterator_traits<iterator_t>::difference_type >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<iterator_t>::size_type, std::size_t >::value ));
    BOOST_STATIC_ASSERT(( is_same< container_traits<iterator_t>::result_iterator, iterator_t >::value ));
    
    BOOST_CHECK( begin( iter ) == iter );
    BOOST_CHECK( end( iter )   == iterator_t() );
    BOOST_CHECK( empty( iter ) == (begin( iter ) == end( iter )) );
    // size not available for iterators
    
    BOOST_CHECK( !empty( iter ) );
    BOOST_CHECK( empty( vec ) );
    copy( iter, std::back_inserter( vec ) );
    BOOST_CHECK( !empty( vec ) );
    BOOST_CHECK( empty( iter ) ); 
}



int test_main( int, char*[] )
{
    check_iterator();

    return 0;
}

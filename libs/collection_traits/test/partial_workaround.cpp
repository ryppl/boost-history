#include <boost/collection_traits/detail/iterator.hpp>
#include <boost/collection_traits/detail/const_iterator.hpp>
#include <boost/collection_traits/detail/result_iterator.hpp>
#include <boost/collection_traits/detail/size_type.hpp>
#include <boost/collection_traits/detail/value_type.hpp>
#include <boost/collection_traits/detail/difference_type.hpp>

#include <boost/collection_traits/detail/begin.hpp>
#include <boost/collection_traits/detail/end.hpp>
#include <boost/collection_traits/detail/size.hpp>
#include <boost/collection_traits/detail/empty.hpp>
#include <boost/collection_traits/detail/sfinae.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include <iostream>
#include <vector>

using namespace boost;
using namespace std;

void check_partial_workaround()
{
    using namespace collection_traits_detail;
    using type_traits::yes_type;
    using type_traits::no_type;

    //////////////////////////////////////////////////////////////////////
    // string
    //////////////////////////////////////////////////////////////////////
    char*          c_ptr;
    const char*    cc_ptr;
    wchar_t*       w_ptr;
    const wchar_t* cw_ptr;
        
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_string_impl( c_ptr ) ) );
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_string_impl( cc_ptr ) ) );
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_string_impl( w_ptr ) ) );
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_string_impl( cw_ptr ) ) );
    
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_char_ptr_impl( c_ptr ) ) );
    BOOST_STATIC_ASSERT( sizeof( no_type ) == sizeof( is_char_ptr_impl( cc_ptr ) ) );

    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_wchar_t_ptr_impl( w_ptr ) ) );
    BOOST_STATIC_ASSERT( sizeof( no_type ) == sizeof( is_wchar_t_ptr_impl( cw_ptr ) ) );
    
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_const_char_ptr_impl( c_ptr ) ) );
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_const_char_ptr_impl( cc_ptr ) ) );
    
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_const_wchar_t_ptr_impl( w_ptr ) ) );
    BOOST_STATIC_ASSERT( sizeof( yes_type ) == sizeof( is_const_wchar_t_ptr_impl( cw_ptr ) ) );

	BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::collection_traits_detail::std_container_, 
		                  ::boost::collection_traits_detail::collection< vector<int> >::type >::value ));
    BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::collection_traits_detail::std_pair_, 
		                  ::boost::collection_traits_detail::collection< pair<int,int> >::type >::value ));
	BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::collection_traits_detail::array_, 
		                  ::boost::collection_traits_detail::collection< int[42] >::type >::value ));
	BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::collection_traits_detail::char_ptr_, 
		                  ::boost::collection_traits_detail::collection< char* >::type >::value ));
	BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::collection_traits_detail::const_char_ptr_, 
		                  ::boost::collection_traits_detail::collection< const char* >::type >::value ));
	BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::collection_traits_detail::wchar_t_ptr_, 
		                  ::boost::collection_traits_detail::collection< wchar_t* >::type >::value ));
	BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::collection_traits_detail::const_wchar_t_ptr_,
		                  ::boost::collection_traits_detail::collection< const wchar_t* >::type >::value ));
	BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::collection_traits_detail::std_container_, 
		                  ::boost::collection_traits_detail::collection< vector<int> >::type >::value ));

/* 
        typedef mpl::int_<10>::type   iterator_;
        typedef mpl::int_<11>::type   string_;
  */    

}




#include <boost/test/included/unit_test_framework.hpp> 
#include <iostream>

using boost::unit_test_framework::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "Collection Traits Test Suite" );

    test->add( BOOST_TEST_CASE( &check_partial_workaround ) );

    return test;
}







// Boost.Range library
//
//  Copyright Thorsten Ottosen 2003-2004. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// For more information, see http://www.boost.org/libs/range/
//

#define BOOST_NO_TEMPLATE_PARTIAL_SPECIALIZATION 1
     
#include <boost/collection_traits/iterator.hpp>
#include <boost/collection_traits/const_iterator.hpp>
#include <boost/collection_traits/size_type.hpp>
#include <boost/collection_traits/value_type.hpp>
#include <boost/collection_traits/difference_type.hpp>
#include <boost/collection_traits/result_iterator.hpp>

#include <boost/collection_traits/begin.hpp>
#include <boost/collection_traits/end.hpp>
#include <boost/collection_traits/size.hpp>
#include <boost/collection_traits/empty.hpp>
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
        typedef mpl::int_<11>::type   string_;
  */    

}




#include <boost/test/included/unit_test_framework.hpp> 

using boost::unit_test_framework::test_suite;

test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    test_suite* test = BOOST_TEST_SUITE( "Collection Traits Test Suite" );

    test->add( BOOST_TEST_CASE( &check_partial_workaround ) );

    return test;
}







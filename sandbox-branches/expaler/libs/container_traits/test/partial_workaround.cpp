#include <boost/container_traits/detail/iterator.hpp>
#include <boost/container_traits/detail/const_iterator.hpp>
#include <boost/container_traits/detail/result_iterator.hpp>
#include <boost/container_traits/detail/size_type.hpp>
#include <boost/container_traits/detail/value_type.hpp>
#include <boost/container_traits/detail/difference_type.hpp>

#include <boost/container_traits/detail/begin.hpp>
#include <boost/container_traits/detail/end.hpp>
#include <boost/container_traits/detail/size.hpp>
#include <boost/container_traits/detail/empty.hpp>
#include <boost/container_traits/detail/sfinae.hpp>

#include <boost/static_assert.hpp>
#include <boost/type_traits.hpp>
#include <boost/test/included/test_exec_monitor.hpp>
#include <boost/test/test_tools.hpp>
#include <iostream>
#include <vector>

using namespace boost;
using namespace std;

void check_sfinae()
{
    using namespace container_traits_detail;
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

	BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::container_traits_detail::std_container_, 
		                  ::boost::container_traits_detail::container< vector<int> >::type >::value ));
    BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::container_traits_detail::std_pair_, 
		                  ::boost::container_traits_detail::container< pair<int,int> >::type >::value ));
	BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::container_traits_detail::array_, 
		                  ::boost::container_traits_detail::container< int[42] >::type >::value ));
	BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::container_traits_detail::char_ptr_, 
		                  ::boost::container_traits_detail::container< char* >::type >::value ));
	BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::container_traits_detail::const_char_ptr_, 
		                  ::boost::container_traits_detail::container< const char* >::type >::value ));
	BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::container_traits_detail::wchar_t_ptr_, 
		                  ::boost::container_traits_detail::container< wchar_t* >::type >::value ));
	BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::container_traits_detail::const_wchar_t_ptr_,
		                  ::boost::container_traits_detail::container< const wchar_t* >::type >::value ));
	BOOST_STATIC_ASSERT(( ::boost::is_same< ::boost::container_traits_detail::std_container_, 
		                  ::boost::container_traits_detail::container< vector<int> >::type >::value ));

/* 
        typedef mpl::int_<10>::type   iterator_;
        typedef mpl::int_<11>::type   string_;
  */    

}


int test_main( int, char*[] )
{
    check_sfinae();

    return 0;
}



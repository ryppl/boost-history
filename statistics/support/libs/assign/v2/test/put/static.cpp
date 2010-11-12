//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

// TODO : the forward declarations are not sufficient as the default
// arguments aren't specified. If they were, redefinition error. Is there 
// a better way?
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <boost/array.hpp>

#include <boost/ptr_container/ptr_array.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <boost/preprocessor/punctuation/comma_if.hpp>
#include <boost/preprocessor/control/expr_if.hpp>
#include <boost/assign/v2/detail/checking/assert.hpp>
#include <boost/assign/v2/put/modifier/modifier.hpp>
#include <libs/assign/v2/test/put/static.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_static{

#define LIBS_ASSIGN_V2_TEST_PUT_static( V, T0, T1, n )\
	{ \
    	typedef V< T0 BOOST_PP_COMMA_IF(n) BOOST_PP_EXPR_IF(n, T1) > cont_; \
        typedef put_aux::deduce_modifier<cont_>::type found_; \
        checking::do_assert_is_same<found_,wanted_>(); \
    } \
/**/

    void test()
    {
    	using namespace boost::assign::v2;
        {
        	typedef modifier_tag::push_back wanted_;
        	LIBS_ASSIGN_V2_TEST_PUT_static( std::list, int, , 0 )
        	LIBS_ASSIGN_V2_TEST_PUT_static( std::vector, int, ,	0 )
			LIBS_ASSIGN_V2_TEST_PUT_static( std::deque, int, , 0 )

			LIBS_ASSIGN_V2_TEST_PUT_static( boost::ptr_deque, int, , 0 )
			LIBS_ASSIGN_V2_TEST_PUT_static( boost::ptr_list, int, ,	0 )
			LIBS_ASSIGN_V2_TEST_PUT_static( boost::ptr_vector, int, , 0 )
        }
        {
        	typedef modifier_tag::at_next wanted_;
			LIBS_ASSIGN_V2_TEST_PUT_static( boost::array, int, 1, 1 )
			LIBS_ASSIGN_V2_TEST_PUT_static( boost::ptr_array, int, 1, 1 )
        }
        {
        	typedef modifier_tag::push wanted_;
			LIBS_ASSIGN_V2_TEST_PUT_static( std::queue, int, , 0 )
			LIBS_ASSIGN_V2_TEST_PUT_static( std::stack, int, ,0 )
        }
        {
        	typedef modifier_tag::insert wanted_;
        	LIBS_ASSIGN_V2_TEST_PUT_static( std::set, int, , 0 )
			LIBS_ASSIGN_V2_TEST_PUT_static( std::map, const char* , int, 1 )
        }
        
    }

#undef LIBS_ASSIGN_V2_TEST_PUT_static

}// xxx_static
}// xxx_put
}// test_assign_v2


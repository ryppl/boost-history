//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/array.hpp>
#include <boost/static_assert.hpp>
#include <boost/assign/v2/detail/type_traits/container/has_static_size.hpp>
#include <boost/assign/v2/ref/anon/unary/assign_copy.hpp>
#include <libs/assign/v2/test/type_traits/has_static_size.h>

struct foo{
	typedef std::size_t size_type;
};

namespace test_assign_v2{
namespace xxx_type_traits{
namespace xxx_has_static_size{

	void test()
    {
    	using namespace boost::assign::v2;
    	{
        	typedef boost::array<int,1> cont_;
            typedef container_type_traits::has_static_size<cont_> pred_;
            BOOST_STATIC_ASSERT( pred_::value );
        }
    	{
        	typedef ref::assign_copy::result_of::empty_anon<
            	int>::type cont_;
            typedef container_type_traits::has_static_size<cont_> pred_;
            BOOST_STATIC_ASSERT( pred_::value );
        }
    	{
        	typedef foo cont_;
            typedef container_type_traits::has_static_size<cont_> pred_;
            BOOST_STATIC_ASSERT( !pred_::value );
        }
    }

}// xxx_has_static_size
}// xxx_type_traits
}// test_assign_v2

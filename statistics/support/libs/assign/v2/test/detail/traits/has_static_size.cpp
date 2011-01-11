//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <cstddef>
#include <boost/array.hpp>
#include <boost/static_assert.hpp>
#include <boost/assign/v2/detail/traits/container/has_static_size.hpp>
#include <boost/assign/v2/ref/array/functor.hpp>
#include <libs/assign/v2/test/detail/traits/has_static_size.h>

struct my_cont{
	typedef std::size_t size_type;
};

namespace test_assign_v2{
namespace xxx_detail{
namespace xxx_traits{
namespace xxx_has_static_size{

	void test()
    {
    	namespace as2 = boost::assign::v2;
    	{
        	typedef boost::array<int,1> cont_;
            typedef as2::container_traits::has_static_size<cont_> pred_;
            BOOST_STATIC_ASSERT( pred_::value );
        }
    	{
        	typedef as2::ref::result_of::empty_array<int>::type cont_;
            typedef as2::container_traits::has_static_size<cont_> pred_;
            BOOST_STATIC_ASSERT( pred_::value );
        }
    	{
        	typedef my_cont cont_;
            typedef as2::container_traits::has_static_size<cont_> pred_;
            BOOST_STATIC_ASSERT( !pred_::value );
        }
    }

}// xxx_has_static_size
}// xxx_traits
}// xxx_detail
}// test_assign_v2

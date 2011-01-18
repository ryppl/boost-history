//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <cstddef>
#include <boost/static_assert.hpp>
#include <boost/assign/v2/detail/traits/type/has_value_type.hpp>
#include <libs/assign/v2/test/detail/traits/has_value_type.h>

namespace test_assign_v2{
namespace xxx_detail{
namespace xxx_traits{
namespace xxx_has_value_type{

    void test(){
    	
        namespace as2 = boost::assign::v2;
        {
            typedef std::vector<int> inp_;
        	typedef as2::type_traits::has_value_type<inp_>::type pred_;
        	BOOST_STATIC_ASSERT(pred_::value);
    	}
        {
            typedef int inp_;
        	typedef as2::type_traits::has_value_type<inp_>::type pred_;
        	BOOST_STATIC_ASSERT(!pred_::value);
    	}
    }
    
    
}// xxx_has_value_type
}// xxx_traits
}// xxx_detail
}// test_assign

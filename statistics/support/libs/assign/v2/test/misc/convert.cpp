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
#include <boost/assign/v2/detail/checking/container.hpp>
#include <boost/assign/v2/misc/convert/check.hpp>
#include <libs/assign/v2/test/misc/convert.h>

namespace test_assign_v2{
namespace xxx_misc{
namespace xxx_convert{

    void test()
    {
    	namespace as2 = boost::assign::v2;
        namespace ns = as2::checking::convert;

		std::vector<int> v;
		{
        	using namespace as2::checking::constants;
            v.push_back( a );
            v.push_back( b );
            v.push_back( c ); 
            v.push_back( d ); 
            v.push_back( e ); 
            v.push_back( f ); 
            v.push_back( g );
            v.push_back( h );
        }

		// Any range will do, so need to check, say, csv_deque
        ns::do_check( v );
    	
    }
        
}// xxx_convert
}// xxx_misc
}// xxx_test_assign


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
#include <boost/assign/v2/put/deque.hpp>
#include <boost/assign/v2/ref/array.hpp>
#include <boost/assign/v2/utility/sub_range.hpp>
#include <libs/assign/v2/example/include.h>
#include <libs/assign/v2/example/utility/sub_range.h>

namespace example_assign_v2{
namespace xxx_misc{
namespace xxx_sub_range{

    void run(std::ostream& os)
    {
    	os << "* sub_range" << std::endl;
    	//[sub_range1
    	std::vector<int> v(3, -1);  v[2] = 1;
        assert( as2::sub_range( v ) < as2::csv_deque( -1, 0, 1 )  );
		//]
    	//[sub_range2
        os << as2::sub_range( 
        	as2::ref::csv_array( "x", "y", "z" ) 
        ); //outputs xyz 
		//]
        os << std::endl;
    }

}// xxx_sub_range
}// xxx_misc
}// example_assign_v2

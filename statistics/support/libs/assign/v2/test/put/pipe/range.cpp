////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                       //
//                                                                        //
//  Copyright (C) 2003-2004 Thorsten Ottosen                              //
//  Copyright (C) 2010 Erwann Rogard                                      //
//  Use, modification and distribution are subject to the                 //
//  Boost Software License, Version 1.0. (See accompanying file           //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)      //
////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <boost/type.hpp>
#include <boost/array.hpp>

#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/detail/checking/container.hpp>
#include <boost/assign/v2/put/pipe/range.hpp>

#include <libs/assign/v2/test/put/pipe/range.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_range{

	void test()
    {
    	std::vector<int>  from;
        namespace as2 = boost::assign::v2;
        {
        	using namespace as2::checking::constants;
            from.push_back( a );
            from.push_back( b );
            from.push_back( c );
            from.push_back( d );
            from.push_back( e );
            from.push_back( f );
            from.push_back( g );
            from.push_back( h );
        }
        {
        	namespace ns = as2::checking::put_range;
            ns::do_check<boost::array<int,8> >( from );
            ns::do_check<std::deque<int> >( from );
            ns::do_check<std::list<int> >( from );
            ns::do_check<std::queue<int> >( from );
            ns::do_check<std::set<int> >( from );
            ns::do_check<std::stack<int> >( from );
            ns::do_check<std::vector<int> >( from );
        }
		{
    		typedef std::stack<int> cont_;
    		std::vector<int> v(2); v[0] = 1; v[1] = -99; 
    		assert(
    			(boost::type<cont_>() | as2::_put_range( v ) ).top() == -99
    		);
        }
    }

}// xxx_range
}// xxx_pipe
}// xxx_put
}// test_assign_v2

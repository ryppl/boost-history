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
#include <boost/array.hpp>

#include <boost/assign/v2/detail/checking/container.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/put/range/range.hpp>

#include <libs/assign/v2/test/put_range/convert.h>

namespace test_assign_v2{
namespace xxx_put_range{
namespace xxx_convert{

	void test()
    {
    	typedef std::vector<int> from_;
        from_ from;
        using namespace boost::assign::v2;
        namespace chk_cont = checking::container;
        {
        	using namespace checking::constants;
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
        	typedef boost::array<int,8> to_;
            chk_cont::do_check( convert<to_>( from ) );
        }
        {
        	typedef std::deque<int> to_;
            chk_cont::do_check( convert<to_>( from ) );
        }
        {
        	typedef std::list<int> to_;
            chk_cont::do_check( convert<to_>( from ) );
        }
        {
        	typedef std::queue<int> to_;
            chk_cont::do_check( convert<to_>( from ) );
        }
        {
        	typedef std::set<int> to_;
            chk_cont::do_check( convert<to_>( from ) );
        }
        {
        	typedef std::stack<int> to_;
            chk_cont::do_check( convert<to_>( from ) );
        }
        {
        	typedef std::vector<int> to_;
            chk_cont::do_check( convert<to_>( from ) );
        }
    }

}// xxx_convert
}// xxx_put_range
}// test_assign_v2

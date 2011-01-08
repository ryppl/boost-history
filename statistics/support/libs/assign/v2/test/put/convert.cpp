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
#include <boost/assign/v2/put/pipe/convert.hpp>

#include <libs/assign/v2/test/put/convert.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_convert{

	void test()
    {
    	typedef std::vector<int> from_;
        from_ from;
        namespace as2 = boost::assign::v2;
        namespace chk_cont = as2::checking::container;
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
        	typedef boost::array<int,8> to_;
            chk_cont::do_check( from | as2::put_convert<to_>() );
        }
        {
        	typedef std::deque<int> to_;
            chk_cont::do_check( from | as2::put_convert<to_>() );
        }
        {
        	typedef std::list<int> to_;
            chk_cont::do_check( from | as2::put_convert<to_>() );
        }
        {
        	typedef std::queue<int> to_;
            chk_cont::do_check( from | as2::put_convert<to_>() );
        }
        {
        	typedef std::set<int> to_;
            chk_cont::do_check( from | as2::put_convert<to_>() );
        }
        {
        	typedef std::stack<int> to_;
            chk_cont::do_check( from | as2::put_convert<to_>() );
        }
        {
        	typedef std::vector<int> to_;
            chk_cont::do_check( from | as2::put_convert<to_>() );
        }
    }

}// xxx_convert
}// xxx_put
}// test_assign_v2

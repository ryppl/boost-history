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
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/detail/keyword/keyword.hpp>
#include <boost/assign/v2/detail/checking/container.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/detail/checking/relational_op.hpp>
#include <boost/assign/v2/detail/checking/check_convert.hpp>

#include <boost/assign/v2/anon/anon.hpp>

#include <libs/assign/v2/test/anon/anon.h>

namespace test_assign_v2{
namespace xxx_anon{
namespace xxx_anon{

	void test()
    {
    	using namespace boost::assign::v2; 
        {
        	// CSV
            using namespace checking::constants;
            using namespace checking::container;
            do_check( csv_anon( a,  b, c, d, e, f, g, h ) );
        }
        {
        	// Operator%
            BOOST_AUTO(tmp, ( anon<int>( _nil ) % (_repeat = 3) ) );
            tmp( 1 );
            BOOST_ASSIGN_V2_CHECK_EQUAL( boost::size( tmp ), 3 );
            BOOST_ASSIGN_V2_CHECK_EQUAL( tmp.front(), 1 );
            BOOST_ASSIGN_V2_CHECK_EQUAL( tmp.back(), 1 );
        }
        {
        	// Conversion
        	typedef result_of::anon<int>::type anon_;
            anon_ from;
            {
            	using namespace checking::constants;
            	from = csv_anon( a, b, c, d, e, f, g, h );
            }
        	using namespace checking::container;
        	{
                typedef boost::array<int,8> to_;
                BOOST_ASSIGN_V2_check_convert
                {
                    to_ to; to = from;
                }
        	}
        	{
                typedef std::deque<int> to_;
                BOOST_ASSIGN_V2_check_convert
        	}
        	{
                typedef std::list<int> to_;
                BOOST_ASSIGN_V2_check_convert
        	}
        	{
                typedef std::queue<int> to_;
                BOOST_ASSIGN_V2_check_convert
        	}
        	{
                typedef std::set<int> to_;
                BOOST_ASSIGN_V2_check_convert
        	}
        	{
                typedef std::stack<int> to_;
                BOOST_ASSIGN_V2_check_convert
        	}
	        {
                typedef std::vector<int> to_;
                BOOST_ASSIGN_V2_check_convert
        	}
#undef BOOST_ASSIGN_V2_check_convert
        }
        { 	// Relational
            using namespace checking::constants;
            using namespace checking::relational_op;
            do_check(
            	csv_anon( a, b, c, d, e, f, g, h )
            );
        }

    }

}// xxx_anon
}// xxx_anon
}// test_assign_v2

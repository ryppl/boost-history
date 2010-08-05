//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <boost/array.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/v2/chain/chain.hpp>
#include <boost/assign/v2/detail/checking/container.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <libs/assign/v2/test/chain/convert.h>

namespace test_assign_v2{
namespace xxx_chain{
namespace xxx_convert{

    void test()
    {	
    	using namespace boost::assign::v2;
    	{
            typedef std::vector<int> vec_;
        	typedef boost::array<int,3> array_;
			
            vec_ cont1, cont3;
            array_ cont2;
            {	
            	using namespace checking::constants;
                cont1.push_back( a ); cont1.push_back( b );
                cont2[0] = c;
                cont2[1] = d;
                cont2[2] = e;
                cont3.push_back( f );
                cont3.push_back( g );
                cont3.push_back( h );
            } 
            BOOST_AUTO(from, cont1 && cont2 && cont3);
            {	// Conversion
        		using namespace checking::container;
        		{
        			typedef boost::array<int,8> to_;
            		do_check( from.convert<to_>() );
                	to_ to; to = from;
                	do_check( from );
        		}
        		{
        			typedef std::deque<int> to_;
            		do_check( from.convert<to_>() );
                	to_ to; to = from;
                	do_check( from );
        		}
        		{
        			typedef std::list<int> to_;
            		do_check( from.convert<to_>() );
                	to_ to; to = from;
                	do_check( from );
        		}	
        		{
        			typedef std::queue<int> to_;
            		do_check( from.convert<to_>() );
                	to_ to; to = from;
                	do_check( from );
        		}
        		{
        			typedef std::set<int> to_;
            		do_check( from.convert<to_>() );
                	to_ to; to = from;
                	do_check( from );
        		}
        		{
        			typedef std::stack<int> to_;
            		do_check( from.convert<to_>() );
                	to_ to; to = from;
                	do_check( from );
        		}	
	        	{
    	    		typedef std::vector<int> to_;
        	    	do_check( from.convert<to_>() );
                	to_ to; to = from;
                	do_check( from );
        		}
            }
        }
    }

}// xxx_convert
}// xxx_chain
}// test_assign_v2

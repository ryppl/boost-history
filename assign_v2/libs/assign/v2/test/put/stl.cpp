//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/detail/workaround.hpp>

#include <boost/array.hpp>
#include <deque>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <vector>
#include <boost/assign/v2/put/sub.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/detail/checking/container.hpp>


#include <libs/assign/v2/test/put/stl.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_stl{

    void test(){
        namespace as2 = boost::assign::v2;
        {	// Array
        
            {	
                typedef boost::array<int,8> cont_;
                cont_ cont;
                {
                    using namespace as2::checking::constants;
                    as2::put( cont )( a )( b )( c )( d )( e )( f )( g )( h );
                }
                {
                    namespace ns = as2::checking::container;
                    ns::do_check( cont );   
                }
            }
        }
        {	// Associative
        	
            {	
                typedef std::map<int,int> cont_;
                cont_ cont;
                {
                    namespace ns = as2::checking::container;
                    using namespace as2::checking::constants;
                    ns::do_check(
                        as2::put( cont )
                            ( a, a )( b, b )( c, c )
                            ( d, d )( e, e )( f, f )
                            ( g, g )( h, h ).get()
                    );
                }
            }// map
            {	
        		typedef std::set<int> cont_;
        		cont_ cont;
        		{
        		    namespace ns = as2::checking::container;
        		    using namespace as2::checking::constants;
        		    ns::do_check(
                        as2::put( cont )
        		            ( a )( b )( c )
        		            ( d )( e )( f )
        		            ( g )( h ).get()
        		        );
        		}
            }// map
        }// Associative
        {
            // Sequence
            {	
        		typedef std::deque<int> cont_;
        		cont_ cont;
        		{
        		    using namespace as2::checking::constants;
        		    namespace ns = as2::checking::container;
        		    ns::do_check(
        		        as2::put( cont )
        		            ( a )( b )( c )( d )( e )( f )( g )( h ).get()
        		    );
        		}
            }// deque
            {	
        		typedef std::list<int> cont_;
        		cont_ cont;
        		{
            		using namespace as2::checking::constants;
                	namespace ns = as2::checking::container;
                    ns::do_check(
                		as2::put( cont )
                        	( a )( b )( c )( d )( e )( f )( g )( h ).get()
        			);
                }
            }// list
            {	
        		typedef std::vector<int> cont_;
                cont_ cont;
                {
            		using namespace as2::checking::constants;
                	namespace ns = as2::checking::container;
                    ns::do_check(
                		as2::put( cont )
                        	( a )( b )( c )( d )( e )( f )( g )( h ).get()
                    );
                }
            }// vector
        } // Sequence
        {
            // Push
            {	
                typedef std::queue<int> cont_;
                cont_ cont;
                {
                    using namespace as2::checking::constants;
                    namespace ns = as2::checking::container;
                    ns::do_check(
                        as2::put( cont )
                            ( a )( b )( c )( d )( e )( f )( g )( h ).get()
                    );
                }
            }// queue
            // Push
            {	
                typedef std::stack<int> cont_;
                cont_ cont;
                {
                    using namespace as2::checking::constants;
                    namespace ns = as2::checking::container;
                    ns::do_check(
                        as2::put( cont )
                            ( a )( b )( c )( d )( e )( f )( g )( h ).get()
                    );
                }
            }// queue
        }// Push
    }// test()
  
}// xxx_stl
}// xxx_put
}// xxx_test_assign

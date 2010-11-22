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

#include <boost/assign/v2.hpp>

#include <libs/assign/v2/test/put/compose/functor/stl.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_compose{
namespace xxx_functor{
namespace xxx_stl{

	void test(){
		using namespace boost::assign::v2;
        using namespace adaptor;
        {	// Array
        	
            {	
        		typedef boost::array<int,8> cont_;
                cont_ cont;
                {
            		using namespace checking::constants;
                	using namespace checking::container;
                    do_check(
                    	cont | _put( a )( b )( c )( d )( e )( f )( g )( h )
                    );
                }
			}
        }
        {	// Associative
        	
            {	
        		typedef std::map<int,int> cont_;
                cont_ cont;
                {
            		using namespace checking::container;
                    using namespace checking::constants;
                    do_check(
                		cont |
                    		_put( a, a )( b, b )( c, c )
                        	( d, d )( e, e )( f, f )
                        	( g, g )( h, h )
                    );
                }
            }// map
            {	
        		typedef std::set<int> cont_;
                cont_ cont;
                {
            		using namespace checking::container;
                    using namespace checking::constants;
                    do_check(
                		cont |
                    		_put( a )( b )( c )
                        	( d )( e )( f )
                        	( g )( h )
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
            		using namespace checking::constants;
                	using namespace checking::container;
                    do_check(
                		cont | _put( a )( b )( c )( d )( e )( f )( g )( h )
                    );
                }
            }// deque
            {	
        		typedef std::list<int> cont_;
                {
            		using namespace checking::constants;
                	using namespace checking::container;
                    cont_ cont;
                    do_check(
                		cont | _put( a )( b )( c )( d )( e )( f )( g )( h )
        			);
                }
            }// list
            {	
        		typedef std::vector<int> cont_;
                {
            		using namespace checking::constants;
                	using namespace checking::container;
               	 	cont_ cont;
                    do_check(
                		cont |
                        	_put( a )( b )( c )( d )( e )( f )( g )( h )
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
            		using namespace checking::constants;
                	using namespace checking::container;
                    do_check(
                        cont |
                        	_put( a )( b )( c )( d )( e )( f )( g )( h )
                    );
                }
            }// queue
        	// Push
            {	
        		typedef std::stack<int> cont_;
                cont_ cont;
                {
            		using namespace checking::constants;
                	using namespace checking::container;
                    do_check(
                		cont |
                        	_put( a )( b )( c )( d )( e )( f )( g )( h )
                    );
                }
            }// queue
        }// Push
	}// test()
  
}// xxx_stl
}// xxx_functor
}// xxx_compose
}// xxx_put
}// xxx_test_assign

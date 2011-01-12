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

#include <boost/ptr_container/ptr_array.hpp>
#include <boost/ptr_container/ptr_deque.hpp>
#include <boost/ptr_container/ptr_list.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/assign/v2/detail/checking/constants.hpp>
#include <boost/assign/v2/detail/checking/container.hpp>
#include <boost/assign/v2/put/sub.hpp>

#include <libs/assign/v2/test/put/ptr.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_ptr{

	void test(){
        namespace as2 = boost::assign::v2;
        {    // Array
            {	
                typedef boost::ptr_array<int, 8> cont_;
                cont_ cont;
                {
                    using namespace as2:: checking::constants;
                    namespace ns = as2::checking::container;
                    ns::do_check(
                        as2::csv_put( cont )
                            ( a , b , c , d , e , f , g , h ).unwrap()
                    );
                }
            }
        }
        {    // Sequence
        	
            {	
                typedef boost::ptr_deque<int> cont_;
                cont_ cont;
                {
                    using namespace as2:: checking::constants;
                    namespace ns = as2::checking::container;
                    ns::do_check(
                        as2::put( cont )
                            ( a )( b )( c )( d )( e )( f )( g )( h ).unwrap()
                    );
                }
            }// ptr_deque
            {	
                typedef boost::ptr_list<int> cont_;
                cont_ cont;
                {
                    using namespace as2:: checking::constants;
                    namespace ns = as2::checking::container;
                    ns::do_check(
                        as2::put( cont )
                            ( a )( b )( c )( d )( e )( f )( g )( h ).unwrap()
                    );
                }
            }// ptr_list
            {	
                typedef boost::ptr_vector<int> cont_;
                cont_ cont;
                {
                    using namespace as2:: checking::constants;
                    namespace ns = as2::checking::container;
                    ns::do_check(
                        as2::put( cont )
                            ( a )( b )( c )( d )( e )( f )( g )( h ).unwrap()
                    );
                }
            }// ptr_vector
        }// Sequence
    }// test
  
}// xxx_ptr
}// xxx_put
}// xxx_test_assign

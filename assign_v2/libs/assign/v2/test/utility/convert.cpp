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
#include <stack>
#include <boost/array.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/utility/convert.hpp>
#include <boost/assign/v2/utility/convert/check.hpp>
#include <libs/assign/v2/test/utility/convert.h>

namespace test_assign_v2{
namespace xxx_utility{
namespace xxx_convert{

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace ns = as2::check_aux;
        
        {
            // containers developed in this library are tested in conjunction 
            // with converter() in their respective directories
        }
        {
            //[convert_inpl 
            std::vector<int> v( 3 ); v[0] = 1; v[1] = 2; v[2] = 0;
            typedef boost::array<int, 3> ar_;
            BOOST_ASSIGN_V2_CHECK( 
                ( as2::converter( v ).type<ar_>() )[1] == v[1] 
            );
            //]
            BOOST_ASSIGN_V2_CHECK( 
                ( as2::converter( v ).type<ar_>() )[0] == v[0] 
            );
            BOOST_ASSIGN_V2_CHECK( 
                ( as2::converter( v ).type<ar_>() )[2] == v[2] 
            );
        }
        {
            //[convert_copy
            std::vector<int> v( 3 ); v[0] = -1; v[1] = 0; v[2] = 1;
            std::stack<int> lifo = as2::converter( v ); 
            BOOST_ASSIGN_V2_CHECK( lifo.top() == 1 );
            //]
        }
    }
        
}// xxx_convert
}// xxx_utility
}// xxx_test_assign


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
#include <boost/spirit/home/phoenix.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/modulo/fun.hpp>
#include <boost/assign/v2/put/pipe/functor.hpp>
#include <boost/assign/v2/put/pipe/csv.hpp>
#include <libs/assign/v2/test/put/pipe/modulo/fun.h>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_modulo{
namespace xxx_fun{

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace lambda = boost::phoenix;
        {
            //[pipe_modulo_fun
            typedef int int_; std::vector<int_> incr_fact;
            int_ front = ( 
            	incr_fact | ( as2::_put % ( as2::_fun = lambda::arg_names::arg1 + 1 ) )/*<<1!, 2!, 3!, 4!, 5!>>*/( 1 )( 2 )( 6 )( 24 )( 120 ) 
            ).front(); 
		
            BOOST_ASSIGN_V2_CHECK( front == 2  );
            BOOST_ASSIGN_V2_CHECK( incr_fact.back() == 121 );
            //]
        }
        {
            //[csv_pipe_modulo_fun
            typedef int int_; std::vector<int_> incr_fact;
            int_ front = ( 
            	incr_fact | ( as2::_csv_put % ( as2::_fun = lambda::arg_names::arg1 + 1 ) )/*<<1!, 2!, 3!, 4!, 5!>>*/( 1, 2, 6, 24, 120 ) 
            ).front(); 
		
            BOOST_ASSIGN_V2_CHECK( front == 2 );
            BOOST_ASSIGN_V2_CHECK( incr_fact.back() == 121 );
            //]
		}
    }

}// xxx_fun
}// xxx_modulo
}// xxx_pipe
}// xxx_put
}// test_assign_v2

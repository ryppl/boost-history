//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <cmath>
#include <list>
#include <vector>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <boost/assign/v2/put/modulo/fun.hpp>
#include <boost/assign/v2/put/pipe/put.hpp>
#include <boost/assign/v2/put/pipe/csv_put.hpp>
#include <boost/function.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <libs/assign/v2/test/put/pipe/fun.h>

#include <boost/range/algorithm/for_each.hpp>


#include <iostream>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_pipe{
namespace xxx_fun{

    void test()
    {
        namespace as2 = boost::assign::v2;
        namespace lambda = boost::lambda;
        {
         	//[pipe_modulo_fun_math
            std::vector<double> exponent; 
            typedef boost::function<double(double)> f1_;
            typedef boost::function<double(double, double)> f2_;
            typedef boost::function<bool(double)> g1_;
            typedef boost::function<bool(double, double)> g2_;
			double eps = boost::numeric::bounds<double>::smallest();
			f2_ f2 = lambda::bind( f1_( fabs ), lambda::_1 - lambda::_2 ) ;
			g1_ g1 = ( eps > lambda::_1 );
            g2_ g2 = lambda::bind( g1, lambda::bind( f2, lambda::_1, lambda::_2 ) );
            //<-
            BOOST_ASSERT(  g2( 0.0, 0 * eps ) );
            BOOST_ASSERT( !g2( 0.0, eps     ) );
            BOOST_ASSERT( !g2( 0.0, - eps   ) );
			//<-
            BOOST_ASSIGN_V2_CHECK(
            	boost::range::equal(
            		exponent | ( as2::_put % ( as2::_fun = f1_( log10 ) ) )( 1000.0 )( 10.0 )( 10000.0 )( 1.0 )( 100.0 ),
              		as2::csv_deque<double>( 3.0, 1.0, 4.0, 0.0, 2.0 ),
                	g2
                )
            );
            //]
        }
        {
            //[csv_pipe_modulo_fun_math
            int i = 0, k = 1; std::list<int> factorials;
            BOOST_ASSIGN_V2_CHECK(
            	boost::range::equal(
	                factorials | ( 
                    	as2::_put % ( as2::_fun = ( lambda::var(k) *= ( ++lambda::var(i) ) ) ) 
                    )()()()()(),
                    as2::csv_deque<int>( 1 )( 2 )( 6 )( 24 )( 120 )
                )
            );
            //]
		}
    }

}// xxx_fun
}// xxx_pipe
}// xxx_put
}// test_assign_v2

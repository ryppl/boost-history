//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <cmath> // MSVC #include <math.h>
#include <deque>
#include <list>
#include <vector>
#include <boost/array.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/container/put.hpp>
#include <boost/assign/v2/put/deque.hpp>
#include <boost/assign/v2/put/fun.hpp>
#include <boost/function.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/construct.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/typeof/typeof.hpp>
#include <libs/assign/v2/test/put/fun.h>

#include <boost/lexical_cast.hpp>

namespace test_assign_v2{
namespace xxx_put{
namespace xxx_fun{

#ifdef __llvm__ 

    struct f{
        
        typedef int result_type;
        
        template<typename T>
        result_type operator()(T const& i)const{ return i + 1; };
        
    };
    
    void test()
    {
        namespace as2 = assign::v2;
        {
            std::vector<int> incr;
            (
             as2::put( incr ) % ( as2::_fun = f() )
             )( 1 )( 2 )( 3 )( 4 )( 5 ); 
            
            BOOST_ASSIGN_V2_CHECK( incr.front() == ( 2 ) );
            BOOST_ASSIGN_V2_CHECK( incr.back() == ( 6 ) );
            // TODO fix Bug :
            // LLVM 1.5 - Release mode, EXC_BAD_ACCESS, stl_vector.h #602
        }        
        
    }
    
#else

    void test()
    {
        using namespace boost;
        using namespace lambda;
        namespace as2 = assign::v2;
        {
            //[modulo_fun_math
            std::vector<double> exponent;
            typedef function<double(double)> f_;
            (
                as2::put( exponent ) % ( as2::_fun = f_( log10 ) )
            )/*<<Calls `exponent.push_back( log10( 1000.0 ) )`>>*/( 1000.0 )( 10.0 )( 10000.0 )( 1.0 )( 100.0 ); 


            double eps = numeric::bounds<double>::smallest();
            BOOST_ASSIGN_V2_CHECK( fabs( exponent.front() - 3.0 ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( exponent.back() - 2.0 ) < eps );
            //]
        }
        {
            //[modulo_fun_deque
            int i = 1, k = 1;
            BOOST_AUTO(
                factorials, (
                    as2::deque<int>( as2::_nil ) % (
                        as2::_fun = ( var(k) *= ( var(i)++ ) )
                    )
                )()()()()()
            );

            BOOST_ASSIGN_V2_CHECK( factorials.front() == ( 1 ) );
            BOOST_ASSIGN_V2_CHECK( factorials.back() == ( 120 ) );
            //]
        }

    }

#endif


}// xxx_fun
}// xxx_put
}// test_assign_v2

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

#include <boost/assign/v2/value/put.hpp>
#include <boost/assign/v2/value/deque.hpp>
#include <boost/assign/v2/value/fun.hpp>
#include <boost/function.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/construct.hpp>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/typeof/typeof.hpp>
#include <libs/assign/v2/test/value/fun.h>

#include <boost/lexical_cast.hpp>

namespace test_assign_v2{
namespace xxx_value{
namespace xxx_fun{

#ifdef __llvm__

    struct f{

        typedef int result_type;

        template<typename T>
        result_type operator()(T const& i)const{ return i + 1; };

    };

    void test()
    {
        using namespace boost;
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
            //[test_value_fun_math
            std::vector<double> exponent;
            typedef function<double(double)> f_;
            (
                as2::put( exponent ) % ( as2::_fun = f_( log10 ) )
            )/*<<Equivalent to `as2::put( exponent )( log10( 1000.0 ) )( log10( 10.0 ) )( log10( 10000.0 ) )( log10( 1.0 ) )( log10( 100.0 ) )`>>*/( 1000.0 )( 10.0 )( 10000.0 )( 1.0 )( 100.0 );


            double eps = numeric::bounds<double>::smallest();
            BOOST_ASSIGN_V2_CHECK( fabs( exponent.front() - 3.0 ) < eps );
            BOOST_ASSIGN_V2_CHECK( fabs( exponent.back() - 2.0 ) < eps );
            //]
        }
        {
            //[test_value_fun_recursive
            int i = 1, k = 1;
            BOOST_AUTO(
                factorials, (
                    as2::deque<int>( as2::_nil ) % (
                        as2::_fun = ( var(k) *= ( var(i)++ ) )
                    )
                )/*<<Equivalent to `factorials( k *= i++ )` for [^i = 1, ..., 5]>>*/()()()()()
            );

            BOOST_ASSIGN_V2_CHECK( range::equal( factorials, as2::csv_deque( 1, 2, 6, 24, 120 ) ) );
            //]
        }

    }

#endif


}// xxx_fun
}// xxx_value
}// test_assign_v2

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
#include <boost/assign/v2/detail/pp/ignore.hpp>
#include <boost/assign/v2/put/deque/csv_deque.hpp>
#include <boost/assign/v2/put/fun.hpp>
#include <boost/assign/v2/put/pipe/put.hpp>
#include <boost/lambda/lambda.hpp>
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
        using namespace boost;
        using namespace lambda;
        namespace as2 = assign::v2;
        {
            //[csv_pipe_modulo_fun_math
            int i = 0, k = 1; std::list<int> factorials;
            BOOST_ASSIGN_V2_CHECK(
                range::equal(
                    factorials | ( 
                        as2::_put % ( as2::_fun = ( var(k) *= ( ++var(i) ) ) ) 
                    )/*Equivalent to calling `factorials.push_back( k *= ++i )`*/()()()()(),
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

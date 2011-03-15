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
#include <boost/lambda/lambda.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/algorithm/stable_partition.hpp>
#include <boost/assign/v2/detail/config/check.hpp>
#include <boost/assign/v2/put/pipe/csv_put.hpp>
#include <boost/assign/v2/ref/array.hpp>
#include <libs/assign/v2/test/mix.h>

namespace test_assign_v2{
namespace xxx_mix{

    // Tests interaction between different functionalities

    void test(){

        using namespace boost;
        using namespace lambda;
        namespace as2 = assign::v2;

        {
            // suggested by JB:
            //[mix_stable_partition
            std::deque<int> cont;
            range::stable_partition(
                /*<< Calls `cont.push_back( t )` for [^t=0,...,5], and returns `cont` >>*/
                cont | as2::_csv_put( 0, 1, 2, 3, 4, 5 ),
                _1 % 2
            );
            //BOOST_ASSIGN_V2_CHECK(range::equal(
            //    cont,
                /*<< [^1, 3, ..., 4] are held by reference (not copies) >>*/
            //    as2::ref::csv_array(1, 3, 5, 0, 2, 4)
            //));

               // as2::ref::csv_array(1, 3, 5, 0, 2, 4);

            //]
        }
    }

}// xxx_mix
}// xxx_test_assign

///////////////////////////////////////////////////////////////////////////////
//  accumulator_ecdf                                                         //
//                                                                           //
//  Copyright (C) 2005 Eric Niebler                                          //
//  Copyright (C) 2011 Erwann Rogard                                         //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <cstddef>
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/stats.hpp>
#include <boost/accumulators/statistics/ecdf/aux_/check.hpp>
#include <boost/accumulators/statistics/ecdf/count.hpp>
#include <libs/accumulators/ecdf/test/count.h>

void test_ecdf_count()
{
    using namespace boost;
    using namespace accumulators;

//[test_ecdf_count
    accumulator_set<
        int, 
        stats<ecdf::tag::count> 
    > acc;

    // Layout is chosen for its clarify, but there is no need to assume a
    // particular ordering
    acc( 1 );
    acc( 2 ); acc( 2 ); 
    acc( 3 ); acc( 3 ); acc( 3 ); 

    BOOST_ACCUMULATORS_ECDF_CHECK(  ecdf::count( acc , 0 ) == 0  );
    BOOST_ACCUMULATORS_ECDF_CHECK(  ecdf::count( acc , 1 ) == 1  );
    BOOST_ACCUMULATORS_ECDF_CHECK(  ecdf::count( acc , 2 ) == 2  );
    BOOST_ACCUMULATORS_ECDF_CHECK(  ecdf::count( acc , 3 ) == 3  );
    BOOST_ACCUMULATORS_ECDF_CHECK(  ecdf::count( acc , 4 ) == 0  );
//]
    
}


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
#include <boost/accumulators/statistics/ecdf/cdf.hpp>
#include <libs/accumulators/ecdf/test/cdf.h>

void test_ecdf_cdf()
{
    using namespace boost;
    using namespace accumulators;

//[test_ecdf_cdf
    typedef double float_;
    accumulator_set<
        int, 
        stats<ecdf::tag::cdf<float_> > 
    > acc;

    // Layout is chosen for its clarify, but there is no need to assume a
    // particular ordering
    acc( 1 );
    acc( 2 ); acc( 2 ); 
    acc( 3 ); acc( 3 ); acc( 3 ); 

    typedef numeric::bounds<float_> bounds_;
    typedef numeric::converter<std::size_t, float_> converter_;

    float_ n = converter_::convert( count( acc ) );
    float_ result, benchmark, eps = bounds_::smallest(); 

    result = ecdf::cdf<float_>( acc , 0 );
    benchmark = converter_::convert( 0 ) / n;
    BOOST_ACCUMULATORS_ECDF_CHECK(  fabs( result - benchmark ) < eps  );

    result = ecdf::cdf<float_>( acc , 1 );
    benchmark = converter_::convert( 1 ) / n;
    BOOST_ACCUMULATORS_ECDF_CHECK(  fabs( result - benchmark ) < eps  );

    result = ecdf::cdf<float_>( acc , 2 );
    benchmark = converter_::convert( 1 + 2 ) / n;
    BOOST_ACCUMULATORS_ECDF_CHECK(  fabs( result - benchmark ) < eps  );

    result = ecdf::cdf<float_>( acc , 3 );
    benchmark = 1.0;
    BOOST_ACCUMULATORS_ECDF_CHECK(  fabs( result - benchmark ) < eps  );

    result = ecdf::cdf<float_>( acc , 4 );
    benchmark = 1.0;
    BOOST_ACCUMULATORS_ECDF_CHECK(  fabs( result - benchmark ) < eps  );

//]
    
}
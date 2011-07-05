///////////////////////////////////////////////////////////////////////////////
// kolmogorov_smirnov_statistic.h     						     	         //
//                                                                           //
//  Copyright 2010 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef LIBS_STATISTICS_DETAIL_NON_PARAMETRIC_KOLMOGOROV_SMIRNOV_HPP_ER_2010
#define LIBS_STATISTICS_DETAIL_NON_PARAMETRIC_KOLMOGOROV_SMIRNOV_HPP_ER_2010
#include <ostream>

void example_kolmogorov_smirnov(
	std::ostream& os,
    double poisson_mean = 1.0,
    long offset = 1,
    long base = 10,
    long first_p = 0,
    long last_p = 5
);

#endif



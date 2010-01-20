///////////////////////////////////////////////////////////////////////////////
// non_parametric::kolmogorov_smirnov.h     						     	 //
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
    std::size_t n_loops = 1,
    std::size_t n_init = 10,
    std::size_t n_factor = 10 // n_init *= n_factor at each loop
);

#endif



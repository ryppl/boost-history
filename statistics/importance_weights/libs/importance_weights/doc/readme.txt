//////////////////////////////////////////////////////////////////////////////
// statistics::importance_weights::doc::readme                              //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

This C++ package contains tools that are commonly needed in manipulating 
importance weights. 

[ algorithm ]

The file prepare_weights.hpp probably meets the most common needs and has 
detailed instructions.

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/usr/local/boost_1_39_0/ 
/sandbox/statistics/empirical_cdf/ 
/sandbox/statistics/functional/

[ Sources ]

[1] http://en.wikipedia.org/wiki/Multinomial_distribution
[2] http://en.wikipedia.org/wiki/Particle_filter
[3] A note on Importance Sampling using standardized weights, Augustine Kong,
Technical report 348, Department of statistics, UChicago, 1992.
[4] Bayesian analysis of massive datasets via particle filters, Ridgeway and 
Madigan

[ History ]

Sep  3 2009 : 
    - replaced importance_weights by statistics::importance_weights
    - simplified the interface of prepare_weights
July   2009 : Creation

[ Output ]

main.cpp:

-> example_algorithm_scale_sum_to_finite : val = 1.79769e+308
c = 13
cum_sum = 1.79769e+308
<-

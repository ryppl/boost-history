//////////////////////////////////////////////////////////////////////////////
// importance_sampling::doc::readme                                         //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

These are C++ tools for computing importance weights and sampling.

[ Note ]

To ensure that the draws are from the desired target distribution, we recommend
monitoring
a) The effective sample size (ESS) 
b) The uniformity of the Cook-Gelman statistics. The latter may be violated by
either i) too small a ESS ii) incorrect computations

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/usr/local/boost_1_40_0/ 
/Users/erwann/sandbox/statistics/detail/fusion/ 
/Users/erwann/sandbox/statistics/non_parametric/ 
/Users/erwann/sandbox/statistics/random/ 
/Users/erwann/sandbox/statistics/distribution_common/ 
/Users/erwann/sandbox/statistics/distribution_toolkit/

[ Sources ]

[1] http://en.wikipedia.org/wiki/Multinomial_distribution

[2] http://en.wikipedia.org/wiki/Particle_filter

[3] A note on Importance Sampling using standardized weights, Augustine Kong,
Technical report 348, Department of statistics, UChicago, 1992.

[4] Bayesian analysis of massive datasets via particle filters,
Ridgeway and Madigan

[5] Andrew Gelman et al., Bayesian Data Analysis, CRC Press

[ History ]

October 11 2009:
    Merged importance_sampling and importance_weights, and embedded it in 
    namespace statistics::detail

Sep    3  2009 : 
    - replaced is:: by statistics::importance_sampling
    - modified the interface of generate_n
    
August 18 2009 : refactored sampler to use discrete_distribution, no longer 
    categorical_distribution
    
July 2009 : First version

[ Output ]

Ouput of main.cpp:

->example_sampler : 
weights : 
(offset,scaling_factor,pc_ess,pc_lt_eps)
(99,1,0.444955,0)
proposal : normal(0,1)
target : normal(1,0.707107)
(10,0.306601)
(110,0.0819208)
(1110,0.0167257)
(11110,0.00736992)
(111110,0.00692354)
(1111110,0.00604868)
(11111110,0.00637739)
<-
-> example_algorithm_scale_sum_to_finite : val = 1.79769e+308
c = 13
cum_sum = 1.79769e+308
<-
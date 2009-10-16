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

These are C++ tools for sampling by importance weights. For example:
    generate(urng,b_w, e_w, b_p, b_t, n);


[ Advice ]

To compute the appropriate weights, we recommend using prepare_weights defined
in the package sandbox/statistics/importance_weights

To ensure that the draws are from the desired target distribution, we recommend
monitoring
a) The effective sample size (ESS) 
b) The uniformity of the Cook-Gelman statistics. The latter may be violated by
either i) too small a ESS ii) incorrect computations

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/usr/local/boost_1_39_0/ 

/sandbox/statistics/arithmetic/ 
/sandbox/statistics/dist_random/ 
/sandbox/statistics/random/ 
/sandbox/statistics/functional/ 
/sandbox/statistics/binary_op/ 
/sandbox/statistics/importance_weights/ 
/sandbox/statistics/scalar_dist/ 
/sandbox/statistics/empirical_cdf/ 
/sandbox/statistics/standard_distribution/ 
/sandbox/statistics/importance_sampling/


[ Models RandomDistribution ]
    Let R denote a range of values (not restricted to scalars) representing
    the proposal sample.

    Class
    statistics::importance_sampling::sampler<R>                      SIR sampler    

[ Sources ]

[1] http://en.wikipedia.org/wiki/Multinomial_distribution

[2] http://en.wikipedia.org/wiki/Particle_filter

[3] A note on Importance Sampling using standardized weights, Augustine Kong,
Technical report 348, Department of statistics, UChicago, 1992.

[4] Bayesian analysis of massive datasets via particle filters,
Ridgeway and Madigan

[5] Andrew Gelman et al., Bayesian Data Analysis, CRC Press

[ History ]

Sep    3  2009 : 
    - replaced is:: by statistics::importance_sampling
    - modified the interface of generate_n
August 18 2009 : refactored sampler to use discrete_distribution, no longer 
    categorical_distribution
    
July 2009 : Current version

[Output]

Here's the ouput from main.cpp:

->example_sampler : 

Initial sample : 
(offset,scaling_factor,pc_ess,pc_lt_eps)
(-1,1,0.438938,0)
kolmorov smirnov distance series :0.212692,0.162692,0.135872,0.110872,0.0926924,
0.079359,0.0436733,0.0530073,0.043285,0.0455073,0.0580085,0.0564934,0.0552113,
0.0469696,0.0464934,0.0520131,0.0516455,0.0568742,0.0595787,0.0448241,0.0462527,
0.0475514,0.0487372,0.0456575,0.0414679,0.0402088,0.0416852,0.0436694,0.0403644,
0.0401345,0.0393403,0.0373241,0.0517948,0.0560643,0.0486694,0.0416852,0.0377813,
0.0419776,0.0485228,0.0497408,0.0519114,0.0546989,0.0503799,0.0511118,0.0526995,
0.0437623,0.0463637,0.0450339,0.0432469,0.0407926,0.0433182,0.0421493,0.0435474,
0.0394593,0.0373381,0.0365077,0.0342207,0.037185,0.0349641,0.0328172,0.0326287,
0.0324701,0.0307292,0.0290426,0.0289465,0.0318836,0.0332404,0.0330868,0.0343868,
0.0370784,0.0354687,0.0380704,0.039231,0.0376575,0.036126,0.0359505,0.0409745,
0.0420234,0.04178,0.0415426,0.0413112,0.0398658,0.0384553,0.0370784,0.0357338,
0.0355833,0.0365857,0.0352926,0.0362758,0.0376072,0.0350797,0.0347811,0.0335453,
0.0320753,0.0318177,0.0321924,0.0315911,0.0300562,0.0338698,0.0366072,<-

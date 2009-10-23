//////////////////////////////////////////////////////////////////////////////
// statistics::survival::model::doc::readme                                 //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

    In this C++ library, we provide models of HasLogLikelihood ( defined in statistics/model) for 
    survival likelihoods.

[ Notation ] 

    See statistics/model/libs/model/doc/readme. Let mcp  an instance of model_covariate_parameter_<M, X, P>


[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/boost_1_39_0/ 
/sandbox/statistics/arithmetic/ 
/sandbox/statistics/importance_sampling/ 
/sandbox/statistics/importance_weights/ 
/sandbox/statistics/joint_dist/ 
/sandbox/statistics/empirical_cdf/ 
/sandbox/statistics/mpl/ 
/sandbox/statistics/random/ 
/sandbox/statistics/binary_op/ 
/sandbox/statistics/functional/ 
/sandbox/statistics/arithmetic/ 
/sandbox/statistics/iterator/ 
/sandbox/statistics/dist_random/ 
/sandbox/statistics/matrix_view/ 
/sandbox/statistics/scalar_dist/ 
/sandbox/statistics/standard_distribution/ 
/sandbox/statistics/survival_data/ 
/sandbox/statistics/model/

Link to : libboost_serialization-xgcc42-mt-1_39.a

[ History ]

July 2009 : Creation

[ models ]

    e.g. model::exponential::model

[ meta ]

    - meta::failure_distribution maps M to a distribution in math/distributions
    - meta::failure_random maps M to a RandomDistribution in boost/random
    
    If these mappings are specified for M, random::failure_time is automatically
    defined for M. 
    
[ Sources ] 

[1] Bayesian survival analysis By Joseph George Ibrahim, Ming-Hui Chen, 
Debajyoti Sinha

[ Output ]
-> example_model_exponential : size(x_cycle) = 100
(-2.48172,-1.87264)
log(prior,likelihood,posterior)
(-2,-11.3373,-13.3373)
(-0.5,-7.28159,-7.78159)
(-0,-5.08452,-5.08452)
(-0.5,-4.18529,-4.68529)
(-2,-4.35436,-6.35436)

log(prior,prior2)
(-2,-2)
(-0.5,-0.5)
(-0,0)
(-0.5,-0.5)
(-2,-2)
<-
-> example_posterior_analysis : 
i = 0, t = 0.018712, pws = (222.593,1,0.0431399,0.861)
i = 1000, t = 0.018008, pws = (195.971,1,0.0198646,0.982)
i = 2000, t = 0.018061, pws = (190.532,1,0.0202048,0.996)
i = 3000, t = 0.017771, pws = (197.043,1,0.0549215,0.866)
i = 4000, t = 0.017818, pws = (195.51,1,0.0671974,0.73)
i = 5000, t = 0.017887, pws = (199.301,1,0.0386544,0.976)
i = 6000, t = 0.017974, pws = (205.461,1,0.0598528,0.866)
i = 7000, t = 0.018313, pws = (182.456,1,0.0708064,0.866)
i = 8000, t = 0.017948, pws = (181.937,1,0.0683872,0.861)
i = 9000, t = 0.017947, pws = (188.914,1,0.00839656,0.248)<-



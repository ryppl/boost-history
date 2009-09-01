//////////////////////////////////////////////////////////////////////////////
// statistics::survival::model::doc::readme                                              //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

    In this C++ library, we provide models of Model (statistics/model) in the 
    survival modeling framework.

[ Notation ] 

    See statistics/model/libs/model/doc/readme and the concept Model. Let mcp 
    denote an instance of model_covariate_parameter_<M, X, P>

[ Bug ]

    See libs/statistics/survival/example/posterior_analysis.cpp     

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/boost_1_39_0/ 
/sandbox/statistics/arithmetic/ 
/sandbox/statistics/non_par/ 
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
/sandbox/statistics/scalar_dist/ 
/sandbox/statistics/standard_distribution/ 
/sandbox/statistics/survival_data/ 
/sandbox/statistics/model/

Link to : libboost_serialization-xgcc42-mt-1_39.a

[ History ]

July 2009 : Current version

[ models ]

These are models of [ SurvivalModel ] defined as:

Let U/u denote a type/object modeling UniformaRandomNumberGenerator (see 
Boost.Random)

M refines SurvivalModel if it models Model with the requirement that
    Expression                          Returns
    random::failure_time<T>(mcp,urng)   Object of type T

Modeled by : 
    model::exponential::model

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
i = 0, t = 0.169766, pws = (222.593,1,0.047237,0.5896)
i = 1000, t = 0.167177, pws = (195.97,1,0.0261956,0.7733)
i = 2000, t = 0.166789, pws = (190.525,1,0.0163538,0.836)
i = 3000, t = 0.167136, pws = (197.041,1,0.0519114,0.5842)
i = 4000, t = 0.168067, pws = (195.495,1,0.05305,0.5729)
i = 5000, t = 0.171331, pws = (199.301,1,0.0381148,0.6841)
i = 6000, t = 0.167311, pws = (205.46,1,0.0556848,0.5621)
i = 7000, t = 0.167732, pws = (182.455,1,0.0639737,0.5192)
i = 8000, t = 0.166502, pws = (181.937,1,0.0661214,0.5163)
i = 9000, t = 0.16732, pws = (188.906,1,0.0135898,0.8683)<-



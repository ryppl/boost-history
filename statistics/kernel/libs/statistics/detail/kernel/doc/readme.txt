//////////////////////////////////////////////////////////////////////////////
// kernel::doc::readme                                                      //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

These are C++ classes for kernel based density estimation and conditional mean 
estimation by the method of Rosenblatt-Parzen and Nadaraya-Watson, respectively.

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/usr/local/boost_1_40_0/ 
/Users/erwann/sandbox/statistics/detail/fusion/ 
/Users/erwann/sandbox/statistics/detail/tuple/
/Users/erwann/sandbox/statistics/cross_validation/ 
/Users/erwann/sandbox/statistics/vector_space/ 
/Users/erwann/sandbox/statistics/mpl/ 

[ Sources ]

http://en.wikipedia.org/wiki/Kernel_(statistics)
http://en.wikipedia.org/wiki/Kernel_density_estimation
@book{citeulike:300228,
	author = {Silverman, B. W.},
	howpublished = {Hardcover},
	isbn = {0412246201},
	keywords = {density-estimation},
	month = {April},
	posted-at = {2008-01-13 17:43:25},
	priority = {2},
	publisher = {{Chapman \& Hall/CRC}},
	title = {Density Estimation for Statistics and Data Analysis},
	year = {1986}
}

[ History ]

October 3rd 2009    : 
    - Replaced namespace statistics by statistics::detail
    - Finer directory structure
    - nw_visitor_tuple replaced by nw_visitor_unary which no longer constrains
    the training data to be a tuple.
    - Added cross_validation 

July 2009           : Creation
March 2009:         : improved_fast_gauss_transform (now deprecated)

[Design]

Let h denote bandwidth, d dimension, ||.|| norm. 

To perform density or conditional mean estimation, we are interested in 
operations that involve an abitrary bandwidth binary kernel, K[h](x0,x1), 
and/or possibly its mono_bw_kernelvariate extension  K[d,h](x0,x1). 

Our design reflects the nested structure of the mathematical pertaining to 
kernels:
 - standardized vs scaled by a bandwidth
 - unnormalized vs normalized
 - unary kernels, K(x) vs binary kernels, K(x0,x1)
 - univariate vs multivariate
 

[ Directories ]

/bandwidth_selection
    
    Analytical and cross validation bandwidth selectors
    
/estimation
    
    The classes rp_visitor and nw_visitor implement the Rosenblatt-Parzen and 
    Nadaraya-Watson estimators for a given test value, x. 

    The class estimator automates the task of visiting a dataset
            
/kernels
    /scalar
        The client need only specify the standardized kernel, such as 
        scalar::gaussian_kernel, while a crtp mechanism, implemented in 
        scalar::crtp, adds the remaining functionality. 

    /multivariate

    Maps a unary kernel to a multivariate one. 
    
[ Math representation ]

Univariate
    Unary:
        Unnormalized:   Ku(x)   :   [0,inf)   ->  [0,inf)
        Normalized:     K(x)    :   [0,inf)   ->  [0,1)
            K(x)  =  Ku(x) / c such that integral K(x) dx = 1
        Parameterized   
        by bandwidth    K[h](x) =   K(x/h) / h

    e.g. 
    Ku(x)   :   exp(-x^2/2) 
    c       :   sqrt(2 pi)
    K(x)    :   exp(-x^2/2) / sqrt(2 pi)
    K[h](x) :   exp(-(x/h)^2 /2) / ( sqrt(2 pi) * h)

Binary:
    K(x0,x1) = K(|x0-x1|)

Multivariate:
    Ku[d](x)    :       Ku(||x||)
    
    e.g
    Ku[d](x)    :       exp(-||x||^2)       :   Ku(||x||)
    c[d]        :       (sqrt(2 pi))^d      :   c^d
    K[d](x)     :       Ku[d](x)/c[d]   
    
    K[d,h](x)   :       exp(-||x/h||^2)/(c * h)^d       
                        = Ku(||x||/h) / (c * h)^d

[ main.cpp output ]

-> example_scalar_rp : <-
-> example_scalar_nw : <-
-> example_mono_bw_kernel_rp : <-
-> test_scalar_nw : 
((n_test,index,n_folds),bandwidth,sqrt_mse)
(partition(10,10,10),0.668325,0.268464)
(partition(100,10,10),0.421685,0.19132)
(partition(1000,10,10),0.266065,0.0698525)
(partition(10000,10,10),0.167876,0.0282646)
(partition(100000,10,10),0.105922,0.0112862)
<-


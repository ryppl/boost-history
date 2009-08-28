//////////////////////////////////////////////////////////////////////////////
// statistics::kernel::doc::readme                                                      //
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

boost_1_39_0
/sandbox/statistics/vector_space/ 
/sandbox/statistics/binary_op/ 
/sandbox/statistics/arithmetic/ 
/sandbox/statistics/scalar_dist/ 
/sandbox/statistics/mpl/ 
/sandbox/statistics/standard_distribution/ 
/sandbox/statistics/kernel/

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

July 2009 : Current version
March 2009: improved_fast_gauss_transform is now deprecated

[Design]

Let h denote bandwidth, d dimension, ||.|| norm. 

To perform density or conditional mean estimation, we are interested in 
operations that involve an abitrary bandwidth binary kernel, K[h](x0,x1), 
and/or possibly its kernel_monovariate extension  K[d,h](x0,x1). 

Our design rests on the following relationships (see [Math representation])
 - standardized kernel (bandwidth = 1) and kernels indexed by a bandwidth
 - unary kernels, K(x), and binary kernels, K(x0,x1)
 - univariate and kernel_monovariate kernels.

The client need only specify the standardized kernel, such as 
scalar::gaussian, while a crtp mechanism, implemented in scalar::crtp,  
adds the remaining functionality. 

The class template statistics::kernel::kernel_mono, parameterized by an arbitrary unavariate 
kernel provides a kernel_monovariate analogue to the latter.

The classes rp_visitor and nw_visitor are implementations for the Rosenblatt-
Parzen and Nadaraya-Watson estimators for a given test value, x. 

Iterating either of the above estimators over a training sample is implemented
by statistics::kernel::estimator, which, in turn, can be used in a std::for_each construct 
to iterate over test data. 

Bandwidth selection for density estimation is provided by 
bandwith::normal_distribution.

[ Directory structure ]

/bandwidth
    Tools for bandwidth selection

/functional
    
    Tools for iterating over a training or test data
    
/scalar

    Univariate kernels
    
/joint
    
    Multivariate kernels

[TODO]

- Bandwidth selection by cross validation and other methods.
- Provide a IFGT version of the above (see our deprecated 
improved_fast_gauss_transform package)
- Kernel regression etc.

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

[Output]

Here's a copy of the output from libs/src/main.cpp:

-> example_scalar_rp : 0.256009 0.390986 0.229151 0.261866 0.232843 0.228334 
0.185519 0.382394 0.388576 0.383203 <-
-> example_scalar_nw : <-
-> example_kernel_mono_rp : <-
-> example_benchmark_scalar : m = 10000
rp : (n, avg_t,e)nw : (n, avg_t,e_p,e_y)
rp : (2, 4.0672e-06, 0.173297) nw : (2, 8.3822e-06, 0.173297, 1.2265)
rp : (4, 2.859e-06, 0.138165) nw : (4, 5.86085e-06, 0.138165, 1.06847)
rp : (8, 3.01747e-06, 0.104602) nw : (8, 6.289e-06, 0.104602, 0.831601)
rp : (16, 3.96233e-06, 0.0978568) nw : (16, 8.16075e-06, 0.0978568, 0.702637)
rp : (32, 5.94066e-06, 0.0818718) nw : (32, 1.20646e-05, 0.0818718, 0.595622)
rp : (64, 9.40283e-06, 0.0719671) nw : (64, 1.95258e-05, 0.0719671, 0.531848)
rp : (128, 1.57752e-05, 0.0634377) nw : (128, 3.26977e-05, 0.0634377, 0.477009)
rp : (256, 2.72618e-05, 0.0565864) nw : (256, 5.64065e-05, 0.0565864, 0.432414)
rp : (512, 4.81542e-05, 0.0511678) nw : (512, 0.00010056, 0.0511678, 0.398781)
rp : (1024, 8.67954e-05, 0.0469132) nw : (1024, 0.000179972, 0.0469132, 0.371483)
<-



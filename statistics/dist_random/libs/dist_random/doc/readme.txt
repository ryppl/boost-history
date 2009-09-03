//////////////////////////////////////////////////////////////////////////////
// dist_random::doc::readme                                                 //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

These are C++ classes that bridge a distribution type (D) defined in boost::math
to a class type (Rd) that models RandomDistribution. For example,

    generate_n(b_x, n,dist, urng);
    sample(dist,urng)

[ Useful links ]

http://www.boost.org/doc/libs/1_35_0/libs/math/doc/sf_and_dist/html/index.html
http://www.boost.org/doc/libs/1_39_0/libs/random/index.html

[ meta ]

    A metafunction that map a distribution to a type (or related traits) that 
    model RandomDistribution. 

[ distributions ]

    Specializes dist_random for each distribution (normal, exponential etc.).

[ functional ]

    check_convergence<T>    Checks agreement between the distribution and its
                            random counterpart based using a kolmogorov-smirnov
                            methodology.
[ random ]    
    
    Function templates that sample from the distribution 
        generate_n(b_x, n,dist, urng);
        sample(dist,urng)
    The actual random generator is hidden in the implementation. These functions
    are conditioned on is_scalar_dist<D> to avoid ambiguity (SFINAE) with a 
    similar interface defined in joint_dist.

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

boost_1_39_0
/sandbox/arithmetic/ 
/sandbox/statistics/scalar_dist/ 
/sandbox/statistics/empirical_cdf/ 
/sandbox/statistics/random/ 
/sandbox/statistics/standard_distribution/ 

[ History ]

July 2009 : Current version

[ Output ]

main.cpp:

example_chi_squared -> 
chi_squared(10) : ks distances by increments of n = 100 : 0.0453213 0.0646862 0.0456301 0.0390222 0.0376862 0.0480195 0.0357961 0.0262651 0.0223762 0.018956 <-

example_location_scale_students_t -> 
location_scale_distribution( 1 + 2 students_t(10)) : ks distances by increments of n = 100 : 0.0801433 0.0751433 0.0354292 0.0220838 0.0296084 0.034622 0.044622 0.0444561 0.0418166 0.040737 <-

-> example_generate_n-0.0227775 -0.1228 -1.24091 -0.560956 -1.69145 -1.77806 -0.182816 -0.066972 -0.167195 -0.201216 -0.530185 -0.262165 -0.00662101 -0.202104 -4.52822 -0.416806 -5.25114 -0.392863 -3.26816 -0.164367 -0.389214 -0.904826 -3.83513 -0.133972 -0.108151 -0.00822772 -1.58654 -0.0134714 -0.034021 -0.318419 -0.00189254 -0.00290242 -0.0927241 -0.0265827 -0.760555 -0.260439 -0.144761 -1.96251 -0.656093 -0.044406 -0.49832 -1.10081 -0.426116 -0.0221952 -0.0810457 -0.157115 -0.957996 -0.185695 -0.0953613 -0.413364 -0.00318939 -1.34638 -0.00119814 -0.642699 -0.333056 -0.215277 -0.0598107 -0.131187 -0.0811628 -0.278249 -0.118659 -1.47727 -0.365579 -0.0150239 -0.0583624 -2.00072 -0.148264 -0.0132129 -3.44333 -1.68415 -0.342194 -1.38323 -0.0154377 -0.118303 -0.241623 -1.20124 -0.610643 -0.063857 -1.03923 -0.0502416 -0.115589 -0.0189673 -0.127707 -0.10828 -0.000498153 -0.0520764 -0.00291445 -0.0342074 -0.0784259 -0.447059 -0.60994 -0.00252972 -0.593436 -0.0751535 -0.580848 -0.99891 -0.1618 -2.37492 -0.00142848 -1.64639 <-

-> example_sample<-


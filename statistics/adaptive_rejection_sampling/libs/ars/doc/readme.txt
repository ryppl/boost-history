//////////////////////////////////////////////////////////////////////////////
// ars::doc::readme                                                         //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

er.ci.2020 at gmail.com

[ Overview ]

This package implements the derivative version of the adaptive rejection
sampler (Gilks, 1992). 

A tool for testing is provided for standard distributions, which makes use of 
their known cdf to produce kolmogorov-smirnov statistics.

[ Bugs ]

Since Jan 20, convergence fails in the test.

[ Compiler ]

Tested Jan 20, 2010
Mac OS X 10.6        gcc 4.0                                    boost_1_41_0
Note: gcc4.2 currently does not mix well with boost::format

Tested Jan 8 2010
Win7-32 V6.1.7100    Dev-C++ 5.0 beta 9.2 with Mingw/GCC 3.4.2 	boost_1_41_0

[ Dependencies ]

boost_1_41_0
/sandbox/statistics/non_parametric
/sandbox/statistics/distribution_common
/sandbox/statistics/distribution_toolkit
/sandbox/statistics/random

[ History ]

Jan 20  2010 : small changes to test functions to adapt to the new 
	non_parametric::kolmogorov_smirnov functionality.

Jan 8 	2010 : in proposal_sampler, found and fixed bug
		//T t = max_element(
        //    begin(datas_),back_iter,
        //    bind<const T&>( &tang_t::t, _1 ) // not a comparison predicate
        //)->t();

Dec 4	2009 : misc changes due to updated dependencies
Sept    2009 : standard_distribution.hpp modified due to changes described in
 sandbox/distribution_toolkit's history.
July    2009 : Previous package superseded by adaptive_rejection_sampling
March   2009 : Conditionally_specified_distribution/adaptive_rejection_sampling 

[Articles]

@article{citeulike:3015268,
	abstract = {We propose a method for rejection sampling from any
	univariate log-concave probability density function.
	The method is adaptive: as sampling proceeds, the rejection envelope
	and the squeezing function converge to the density function.
	The rejection envelope and squeezing function are piece-wise exponential
	functions, the rejection envelope touching the density at previously
	sampled points, and the squeezing function forming arcs between those
	points of contact. The technique is intended for situations where
	evaluation of the density is computationally expensive, in particular
	for applications of Gibbs sampling to Bayesian models with non-conjugacy.
	We apply the technique to a Gibbs sampling analysis of monoclonal
	antibody reactivity.},
	author = {Gilks, W. R.  and Wild, P. },
	citeulike-article-id = {3015268},
	doi = {10.2307/2347565},
	journal = {Applied Statistics},
	number = {2},
	pages = {337--348},
	posted-at = {2008-10-16 11:59:33},
	priority = {5},
	publisher = {Blackwell Publishing for the Royal Statistical Society},
	title = {Adaptive Rejection Sampling for {Gibbs} Sampling},
	url = {http://dx.doi.org/10.2307/2347565},
	volume = {41},
	year = {1992}
}

http://www.econ.ucsb.edu/~tedb/Theory/delta.pdf
Log-Concave Probability and Its Applications by  Mark Bagnoli,
Ted Bergstrom, University of Michigan, Current version: October 1989

On Adaptive Rejection Sampling, Rajeeva L. Karandikar, Indian Statistical
Institute, 2005.

[Sofware]

I coding this software we have looked at these implementations:

ars.f by Gilks
http://www.maths.leeds.ac.uk/~wally.gilks/adaptive.rejection/web_page/Welcome.html

ars1d.h, started 22 April 2008 by tss
www.cs.cmu.edu/~tss/README_ars1d.txt

[Output]

Here's the output from main.cpp :

[Session started at 2009-07-24 20:48:28 -0400.]
-> example_search_reflection
N(0,2)x_0 = 100, x_1 = 100.01, , n = 14, p_0 : (-63.83,-509.284,15.9575), p_1 : (100.01,-1250.25,-25.0025)
x_0 = -100.01, x_1 = -100, , n = 14, p_0 : (-100.01,-1250.25,25.0025), p_1 : (63.83,-509.284,-15.9575)
x_0 = -0.02, x_1 = -0.01, , n = 2, p_0 : (-0.02,-5e-05,0.005), p_1 : (0.02,-5e-05,-0.005)
<-
-> example_standard_distribution 
Gamma(3,1) : ars initialized every 1 with init_0 = 102 and init_1 = 102.01
(Kolmogorov-statistic, # rejections per draw): 
((10,0.16757),2.1)
((100,0.0580157),2.46)
((1000,0.0309302),2.318)
((10000,0.00768814),2.3112)
((100000,0.0021249),2.32135)
((1000000,0.000751711),2.32073)

Gamma(3,1) : ars initialized every 1 with init_0 = 2.01 and init_1 = 2.02
(Kolmogorov-statistic, # rejections per draw): 
((10,0.379978),2.3)
((100,0.0560606),1.71)
((1000,0.018001),1.683)
((10000,0.00552136),1.6643)
((100000,0.00235738),1.65932)
((1000000,0.000615244),1.65827)

N(0,2) : ars initialized every 1 with init_0 = -100 and init_1 = 0.01
(Kolmogorov-statistic, # rejections per draw): 
((10,0.153306),4.8)
((100,0.0478689),4.29)
((1000,0.0237086),4.679)
((10000,0.00603333),4.6361)
((100000,0.00306973),4.65347)
((1000000,0.000669519),4.65573)

N(0,2) : ars initialized every 1 with init_0 = -0.01 and init_1 = 100
(Kolmogorov-statistic, # rejections per draw): 
((10,0.275829),5.4)
((100,0.0408603),4.57)
((1000,0.0170958),4.683)
((10000,0.00669968),4.6581)
((100000,0.0023244),4.65068)
((1000000,0.000614343),4.65194)

N(0,2) : ars initialized every 1 with init_0 = -0.01 and init_1 = 0.01
(Kolmogorov-statistic, # rejections per draw): 
((10,0.173027),5.7)
((100,0.104005),5.99)
((1000,0.0356427),5.933)
((10000,0.00754988),6.0014)
((100000,0.00280912),5.98497)
((1000000,0.0013802),5.99028)

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
March 3rd, 2010
    - Fixed an apparent slow convergence in the output from 
    standard_distribution.cpp which was in fact due to a formatting defect.
    - Added MSVC workarounds in proposal_sampler.hpp 

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


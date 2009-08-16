//////////////////////////////////////////////////////////////////////////////
// ars::doc::readme                                       		    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

This package implements the derivative version of the adaptive rejection
sampler (Gilks, 1992). 

A tool for testing is provided for standard distributions, which makes use of 
their known cdf to produce kolmogorov-smirnov statistics.

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

boost_1_39_0

[ History ]

July 2009 : Current version
March 2009 : 
sandbox/conditionally_specified_distribution/adaptive_rejection_sampling is 
now deprecated.

[Artiles]

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
-> example_standard_distribution 
Gamma(3,1) : init_0 = 102, init_1 = 102.01, by increments of n = 1, 
KS distance : 0.899296 0.399296 0.565963 0.649296 0.499296 0.399296 0.327868 
0.29257 0.232629 0.26757 0.303933 0.250903 0.282954 0.26833 0.225473 0.187973 
0.20242 0.225296 0.245764 0.214185  
n_reject : 3 2 2 2 2 2 1 3 2 2 5 3 2 3 2 2 4 3 2 2 

Gamma(3,1) : init_0 = 2.01, init_1 = 2.02, by increments of n = 1, 
KS distance : 0.905859 0.405859 0.572525 0.655859 0.505859 0.572525 0.620144 
0.530859 0.546644 0.479978 0.492329 0.439299 0.394427 0.427394 0.389299 0.355966
0.326554 0.30041 0.277018 0.255966  
n_reject : 3 1 3 2 2 3 3 2 1 3 0 2 3 1 1 1 3 3 2 1 

N(0,2) : init_0 = -100, init_1 = 0.01, by increments of n = 1, 
KS distance : 0.329606 0.170394 0.21905 0.170394 0.270394 0.170394 0.171431
0.190783 0.260179 0.215734 0.27028 0.312011 0.273549 0.28062 0.247287 0.21812 
0.192385 0.169509 0.149041 0.13062 0.161573 0.144257 0.171925 0.15562 0.18062 
0.165236 0.188028 0.173477 0.159931 0.147287 0.135459 0.128283 0.145329 0.133561
0.123477 0.113954 0.123216 0.138152 0.126681 0.115783 0.105417 0.0955451 
0.086132 0.0929632 0.104579 0.115691 0.126329 0.115691 0.125895 0.115691 
0.125494 0.115691 0.106257 0.097172 0.0884178 0.0799762 0.0893747 0.0812077 
0.0902668 0.0990238 0.0911003 0.0995615 0.091881 0.100066 0.0926136 0.0853875 
0.0933024 0.0862787 0.0794586 0.0871191 0.0804792 0.0740238 0.0677453 0.0616365 
0.0690238 0.0630589 0.0572489 0.0644085 0.0587285 0.0656905 0.060135 0.06691 
0.0614736 0.0561667 0.0627493 0.057551 0.0524721 0.0588723 0.0538928 0.0490238 
0.0442619 0.0470498 0.0531274 0.0488212 0.0519567 0.0550269 0.0580337 0.0507752 
0.0473638 0.0478689  
n_reject : 6 8 6 5 4 6 2 2 4 5 5 8 6 6 4 4 6 2 5 7 8 3 2 4 1 2 0 6 4 3 4 3 5 5 3
 6 2 7 4 3 4 7 3 3 4 7 6 7 6 3 2 4 5 4 3 6 7 5 3 3 7 7 6 2 5 6 3 4 5 5 7 4 5 3 4
  1 4 4 6 3 3 5 3 3 5 2 1 3 4 4 5 4 4 1 8 2 4 2 3 8 
  
N(0,2) : init_0 = -0.01, init_1 = 100, by increments of n = 1, 
KS distance : 0.224171 0.424326 0.590992 0.424326 0.324326 0.257659 0.21004 
0.275829 0.220273 0.175829 0.230374 0.275829 0.237367 0.2044 0.242495 0.217415 
0.246417 0.245193 0.204257 0.175829 0.176324 0.187147 0.194709 0.203767 0.172101
 0.181331 0.189878 0.197815 0.170721 0.178767 0.186294 0.193351 0.169676 
 0.147395 0.154958 0.162101 0.14183 0.148943 0.15569 0.137101 0.143808 0.150196 
 0.133031 0.139373 0.145434 0.129492 0.135505 0.120434 0.126386 0.112101 
 0.117983 0.104408 0.110214 0.115804 0.121191 0.126386 0.113855 0.101756 
 0.107016 0.112101 0.117019 0.121778 0.117197 0.121661 0.125988 0.115032 
 0.119329 0.108794 0.113056 0.102911 0.0930521 0.0973557 0.101541 0.105614 
 0.109578 0.10028 0.0912229 0.0952189 0.0864555 0.0779112 0.0819236 0.0736429 
 0.07761 0.069869 0.0736505 0.0773441 0.0809527 0.0731157 0.0766908 0.0690753 
 0.0718321 0.0699209 0.0687169 0.0662206 0.0644289 0.0626745 0.0609563 0.0592732
  0.057624 0.0560079  n_reject : 6 5 6 4 6 4 7 5 4 7 2 9 5 5 7 3 3 5 6 7 7 4 5 4
   6 4 8 5 5 6 3 6 5 1 2 5 7 4 6 6 4 5 6 3 5 3 5 4 3 0 6 8 6 5 5 6 6 5 5 6 5 4 4
    5 6 2 5 6 6 4 5 4 0 6 7 5 6 4 2 5 6 4 4 5 3 3 7 1 6 2 2 1 5 3 4 7 4 4 5 1 
    
N(0,2) : init_0 = -0.01, init_1 = 0.01, by increments of n = 1, KS distance : 
0.94637 0.44637 0.349201 0.19637 0.282535 0.349201 0.387313 0.423027 0.339693 
0.273027 0.30939 0.25636 0.211488 0.173027 0.150519 0.148475 0.13224 0.0995606 
0.122952 0.144005 0.115434 0.134914 0.152701 0.148219 0.163219 0.177065 0.172077
 0.183982 0.195066 0.205411 0.215088 0.192911 0.20238 0.211293 0.191125 0.199855
  0.181086 0.163305 0.146436 0.155411 0.163947 0.172077 0.17993 0.166188 
  0.153057 0.162236 0.171024 0.158613 0.146708 0.135279 0.143907 0.152202 
  0.160185 0.167872 0.175279 0.182422 0.189314 0.195969 0.202398 0.191946 
  0.19823 0.204311 0.194327 0.184654 0.190664 0.18134 0.172294 0.17822 0.183975 
  0.175279 0.166828 0.158613 0.150622 0.15636 0.161946 0.167384 0.159695 
  0.152202 0.157558 0.162779 0.155526 0.14845 0.141544 0.146708 0.139985 
  0.133419 0.127003 0.123551 0.128275 0.121783 0.126423 0.130962 0.135403 
  0.13975 0.133479 0.137755 0.141943 0.146046 0.139965 0.134005  
  n_reject : 9 6 8 5 4 4 5 3 7 6 7 3 4 7 5 3 7 6 7 4 5 7 3 7 7 8 8 8 5 3 6 3 4 8
   3 6 4 5 6 7 6 6 7 3 9 2 8 1 3 6 5 8 8 6 7 4 8 5 8 6 6 7 8 4 7 8 7 5 8 6 5 6 6
    3 4 7 6 4 8 4 6 4 7 8 10 7 7 7 5 8 8 6 3 4 8 6 8 7 6 7 
    
-> example_search_reflection
N(0,2)x_0 = 100, x_1 = 100.01, , n = 14, p_0 : (-63.83,-509.284,15.9575), 
p_1 : (100.01,-1250.25,-25.0025)
x_0 = -100.01, x_1 = -100, , n = 14, p_0 : (-100.01,-1250.25,25.0025), 
p_1 : (63.83,-509.284,-15.9575)
x_0 = -0.02, x_1 = -0.01, , n = 2, p_0 : (-0.02,-5e-05,0.005), 
p_1 : (0.02,-5e-05,-0.005)
<-

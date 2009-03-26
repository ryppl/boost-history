//////////////////////////////////////////////////////////////////////////////
// adaptive_rejection_sampling
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

///////////
/ Contact /
///////////

Please send questions or suggestions to erwann.rogard@gmail.com

////////////
/ Overview /
////////////

This collection of C++ classes constructs an approximation to an
arbitrary log-concave distribution by evaluating its unnormalized log-pdf
and derivative thereof, at a set of abscissae,  and samples from the
true distribution with a rejection rule.

Individual files contain additional information.

See the accompanying algorithm description (together with references)
in ars.pdf.

////////////////
/ Requirements /
////////////////

Compiles fine under

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

The compiler search path must include
boost_1_37_0
sandbox/conditionally_specified_distribution/adaptive_rejection_sampling
sandbox/conditionally_specified_distribution/math_limit
sandbox/conditionally_specified_distribution/utility
//////////////
/ Discussion /
//////////////

The algorithm may run into overflows/underflows problems. 1) Some of them
are already dealt with (exceptions) but a more comprehensive
approach is needed. 2) With a choice of initial values that are not too
far in the tails, such problems usually do not arise.

//////////////////////
/ Output of main.cpp /
//////////////////////

limits::negative_infinity()-1.79769e+308
limits::infinity()1.79769e+308
limits::min_value()2.22507e-308
limits::log_min_value()-708
limits::log_max_value()709
-> test_gaussian
We draw from the standard-gaussian using the algorithm.
Results are compared with a similar implementation inMathematica.
A failure will be generated if the difference is greater than 0.0001
The uniform draws were Mathematica generated.
Initialized with (x1,x2)=(-0.5,1.5)
At i = 1, normalizing constant = 3.45306 vs true value 2.50663
At i = 21, normalizing constant = 2.57501 vs true value 2.50663
At i = 41, normalizing constant = 2.57328 vs true value 2.50663
At i = 61, normalizing constant = 2.56461 vs true value 2.50663
At i = 81, normalizing constant = 2.56461 vs true value 2.50663
At i = 101, normalizing constant = 2.54456 vs true value 2.50663
At i = 121, normalizing constant = 2.54456 vs true value 2.50663
At i = 141, normalizing constant = 2.52898 vs true value 2.50663
At i = 161, normalizing constant = 2.52898 vs true value 2.50663
At i = 181, normalizing constant = 2.52588 vs true value 2.50663
Initialized with (x1,x2)=(-0.001,0.001)
At i = 1, normalizing constant = 3.94626 vs true value 2.50663
At i = 21, normalizing constant = 2.57218 vs true value 2.50663
At i = 41, normalizing constant = 2.55671 vs true value 2.50663
At i = 61, normalizing constant = 2.53946 vs true value 2.50663
At i = 81, normalizing constant = 2.52847 vs true value 2.50663
At i = 101, normalizing constant = 2.52636 vs true value 2.50663
At i = 121, normalizing constant = 2.52378 vs true value 2.50663
At i = 141, normalizing constant = 2.52378 vs true value 2.50663
At i = 161, normalizing constant = 2.52378 vs true value 2.50663
At i = 181, normalizing constant = 2.52269 vs true value 2.50663
<-

///////////
/ History /
///////////

2009/03/18
Fixed some stability issues pertaining to shift_ and removed the reliance
on a truncated range e.g. exp(-inf) is not only fine but desirable.

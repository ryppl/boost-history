//////////////////////////////////////////////////////////////////////////////
// monomials_horner::doc                                                    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)	    //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1 (Apple Inc. build 5490)

[Dependencies]

/boost_1_39_0/ 
/sandbox/statistics/view/ 

[ Overview  ]

This collection of C++ classes computes multivariate monomials and derived
quantities using Horner's rule a.k.a. graded lexicographic ordering. 

A possible application is the Fast Gauss Transform.

[ Notation ]

D       dimension
a       multivariate power
|a|     total degree of a             |a| = sum{a(d):d=0,...,D-1}
p       a bound on |a|
b       coefficient
x       range of size D

[ History ]

Sep 09  Refactored to account for the change from matrix::view::row_iterator
 to view::detail::rows_iterator


[ Output ]

Here's the output from from main.cpp

-> example_monomials

 -> d=1
  ->p=1
1 0   <-
  ->p=2
1 0 0   <-
  ->p=3
1 0 0 0   <-
  ->p=4
1 0 0 0 0   <-
 <-
 -> d=2
  ->p=1
1 0 1   <-
  ->p=2
1 0 1 0 0 1   <-
  ->p=3
1 0 1 0 0 1 0 0 0 1   <-
  ->p=4
1 0 1 0 0 1 0 0 0 1 0 0 0 0 1   <-
 <-
 -> d=3
  ->p=1
1 0 1 2   <-
  ->p=2
1 0 1 2 0 0 0 1 2 4   <-
  ->p=3
1 0 1 2 0 0 0 1 2 4 0 0 0 0 0 0 1 2 4 8   <-
  ->p=4
1 0 1 2 0 0 0 1 2 4 0 0 0 0 0 0 1 2 4 8 0 0 0 0 0 0 0 0 0 0 1 2 4 8 16   <-
 <-
 -> d=4
  ->p=1
1 0 1 2 3   <-
  ->p=2
1 0 1 2 3 0 0 0 0 1 2 3 4 6 9   <-
  ->p=3
1 0 1 2 3 0 0 0 0 1 2 3 4 6 9 0 0 0 0 0 0 0 0 0 0 1 2 3 4 6 9 8 12 18 27   <-
  ->p=4
1 0 1 2 3 0 0 0 0 1 2 3 4 6 9 0 0 0 0 0 0 0 0 0 0 1 2 3 4 6 9 8 12 18 27 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 1 2 3 4 6 9 8 12 18 27 16 24 36 54 81   <-
 <-
<-
-> example_multi_indexes
0 0 0
1 0 0
0 1 0
0 0 1
2 0 0
1 1 0
1 0 1
0 2 0
0 1 1
0 0 2

0 0 0
1 0 0
0 1 0
0 0 1
2 0 0
1 1 0
1 0 1
0 2 0
0 1 1
0 0 2
3 0 0
2 1 0
2 0 1
1 2 0
1 1 1
1 0 2
0 3 0
0 2 1
0 1 2
0 0 3
<-
-> example_multi_factorial
 -> multi_factorial<3>::degree(5)
1 1 1 1 2 1 1 2 1 2 6 2 2 2 1 2 6 2 2 6 24 6 6 4 2 4 6 2 2 6 24 6 4 6 24 120 24 24 12 6 12 12 4 4 12 24 6 4 6 24 120 24 12 12 24 120
 -> multi_degree<3>::degree(5)
0 1 1 1 2 2 2 2 2 2 3 3 3 3 3 3 3 3 3 3 4 4 4 4 4 4 4 4 4 4 4 4 4 4 4 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5 5
 -> multi_p2df<3>::degree(5)
1 2 2 2 2 4 4 2 4 2 1.33333 4 4 4 8 4 1.33333 4 4 1.33333 0.666667 2.66667 2.66667 4 8 4 2.66667 8 8 2.66667 0.666667 2.66667 4 2.66667 0.666667 0.266667 1.33333 1.33333 2.66667 5.33333 2.66667 2.66667 8 8 2.66667 1.33333 5.33333 8 5.33333 1.33333 0.266667 1.33333 2.66667 2.66667 1.33333 0.266667
<-


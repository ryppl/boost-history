//////////////////////////////////////////////////////////////////////////////
// miscellanea_algorithm.hpp
//
//  Copyright 2009 Erwann Rogard. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

///////////
/ Contact /
///////////

Please send questions or suggestions to erwann.rogard@gmail.com

/////////////
/ Overview  /
/////////////

This collection of C++ classes are take one more ranges as input
and return an iterator (such as the nearest neighbor to the input) or a value
(such as the norm of the input).

Warning: these were designed for very specific applications and have very few
safeguards.

//////////////////
/ Requirements   /
//////////////////

Compiles fine under

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1 (Apple Inc. build 5490)

The compiler search path must include
boost_1_37_0
boost/sandbox/miscellanea_iterator_facade
boost/sandbox/miscellanea_algorithm

////////////////////////
/ Output from main.cpp /
////////////////////////
->example_find_nearest_neighbor
3 4 5 <-
->example_squared_l2_norm()
 l2_norm(vec0) = 1.41421
 linfinity_norm(vec0) = 1
 l2_distance_squared (vec0,vec1) = 8
 linfinity_distance (vec0,vec1) = 2
<-
->example_inner_product_skip_one_position
0
-2
0
exception=inner_product_skip_one_position size(range0)!=size(range1)
<-


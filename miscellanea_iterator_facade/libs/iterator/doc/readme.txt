//////////////////////////////////////////////////////////////////////////////
// miscellanea_iterator_facade
//
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
///////////
/ Contact /
///////////

Please send questions or suggestions to erwann.rogard@gmail.com

/////////////
/ Overview  /
/////////////

This collection of C++ classes are iterators that derive from iterator_facade,
 or ranges thereof.

Warning: these were designed for very specific applications and have very few
safeguards.

//////////////////
/ Requirements   /
//////////////////

Compiles fine under

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1 (Apple Inc. build 5490)

The compiler search path must include
boost_1_37_0
sandbox/boost/miscellanea_iterator_facade

////////////////////////
/ Output from main.cpp /
////////////////////////

->example_vector2matrix
0 1 2 3 4 5 6 7
std::distance(b,e)=0
std::distance(b,e)=0
std::distance(b,e)=1
std::distance(b,e)=1
std::distance(b,e)=2
std::distance(b,e)=2
std::distance(b,e)=3
std::distance(b,e)=3
exited loop
<-
->example_skip_range_iterator
i=1
 j=1
  0 1 2 3 4 5 6 7 8 9
 j=2
  0 2 3 4 5 6 7 8 9
 j=3
  0 3 4 5 6 7 8 9
 j=4
  0 4 5 6 7 8 9
 j=5
  0 5 6 7 8 9
 j=6
  0 6 7 8 9
 j=7
  0 7 8 9
 j=8
  0 8 9
 j=9
  0 9
 j=10
  0
i=2
 j=2
  0 1 2 3 4 5 6 7 8 9
 j=3
  0 1 3 4 5 6 7 8 9
 j=4
  0 1 4 5 6 7 8 9
 j=5
  0 1 5 6 7 8 9
 j=6
  0 1 6 7 8 9
 j=7
  0 1 7 8 9
 j=8
  0 1 8 9
 j=9
  0 1 9
 j=10
  0 1
i=3
 j=3
  0 1 2 3 4 5 6 7 8 9
 j=4
  0 1 2 4 5 6 7 8 9
 j=5
  0 1 2 5 6 7 8 9
 j=6
  0 1 2 6 7 8 9
 j=7
  0 1 2 7 8 9
 j=8
  0 1 2 8 9
 j=9
  0 1 2 9
 j=10
  0 1 2
i=4
 j=4
  0 1 2 3 4 5 6 7 8 9
 j=5
  0 1 2 3 5 6 7 8 9
 j=6
  0 1 2 3 6 7 8 9
 j=7
  0 1 2 3 7 8 9
 j=8
  0 1 2 3 8 9
 j=9
  0 1 2 3 9
 j=10
  0 1 2 3
i=5
 j=5
  0 1 2 3 4 5 6 7 8 9
 j=6
  0 1 2 3 4 6 7 8 9
 j=7
  0 1 2 3 4 7 8 9
 j=8
  0 1 2 3 4 8 9
 j=9
  0 1 2 3 4 9
 j=10
  0 1 2 3 4
i=6
 j=6
  0 1 2 3 4 5 6 7 8 9
 j=7
  0 1 2 3 4 5 7 8 9
 j=8
  0 1 2 3 4 5 8 9
 j=9
  0 1 2 3 4 5 9
 j=10
  0 1 2 3 4 5
i=7
 j=7
  0 1 2 3 4 5 6 7 8 9
 j=8
  0 1 2 3 4 5 6 8 9
 j=9
  0 1 2 3 4 5 6 9
 j=10
  0 1 2 3 4 5 6
i=8
 j=8
  0 1 2 3 4 5 6 7 8 9
 j=9
  0 1 2 3 4 5 6 7 9
 j=10
  0 1 2 3 4 5 6 7
i=9
 j=9
  0 1 2 3 4 5 6 7 8 9
 j=10
  0 1 2 3 4 5 6 7 8
i=10
 j=10
  0 1 2 3 4 5 6 7 8 9
<-
->example_skip_one_step_iterator
i=1
  0 2 3 4 5 6 7 8 9
i=2
  0 1 3 4 5 6 7 8 9
i=3
  0 1 2 4 5 6 7 8 9
i=4
  0 1 2 3 5 6 7 8 9
i=5
  0 1 2 3 4 6 7 8 9
i=6
  0 1 2 3 4 5 7 8 9
i=7
  0 1 2 3 4 5 6 8 9
i=8
  0 1 2 3 4 5 6 7 9
i=9
  0 1 2 3 4 5 6 7 8
<-
->example_skip_one_step_iterator
i=1
  0 2 3 4 5 6 7 8 9
i=2
  0 1 3 4 5 6 7 8 9
i=3
  0 1 2 4 5 6 7 8 9
i=4
  0 1 2 3 5 6 7 8 9
i=5
  0 1 2 3 4 6 7 8 9
i=6
  0 1 2 3 4 5 7 8 9
i=7
  0 1 2 3 4 5 6 8 9
i=8
  0 1 2 3 4 5 6 7 9
i=9
  0 1 2 3 4 5 6 7 8
<-
->example_skip_one_step_iterator
1 0 0 1 0 0 1 0 0 1 0 0 1 0 0 <-
->example_subset_of_range
<-

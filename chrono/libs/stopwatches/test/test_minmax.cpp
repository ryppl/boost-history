//  test_minmax.cpp  ----------------------------------------------------------//
//  Copyright 2011 Vicente J. Botet Escriba
//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

#if !defined(__GNUC__)

#define min(A,B) ((A)<(B)?(A):(B))
#define max(A,B) ((A)>(B)?(A):(B))

#include <boost/stopwatches/stopwatches.hpp>

 int main()
 {
    boost::stopwatches::stopclock<> _;
 }

#endif

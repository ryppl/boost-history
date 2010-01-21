//////////////////////////////////////////////////////////////////////////////
// non_parametric::doc::readme                                              //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

I welcome feedback including bug reports at erwann.rogard@gmail.com

[ Overview ]

    C++ classes for non-parametric estimation. 

[ Also see ]

	http://boost-sandbox.sourceforge.net/libs/accumulators/doc/html/index.html

[ Compiler ]

    gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

    /boost_1_40_0


[ History] 
	January 20th, 2010
    	- Miscellanea reorganization of the files

	January 17th, 2010. 
        - Modified non_parametric/kolmogorov_smirnov/statistic.hpp from an 
        STL-like algorithm to a simpler accumulator.
        - Added check_convergence
        
	January 14th, 2010. 
    	- Added non_parametric/empirical_distribution
        - Modified non_parametric/kolmogorov_smirnov/statistic.hpp to allow
        combos

    October 11th 2009. First version. Implementation of kolmogorov_smirnov
    transfered from distribution_toolkit

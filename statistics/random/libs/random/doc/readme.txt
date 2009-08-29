//////////////////////////////////////////////////////////////////////////////
// random::doc::readme                                                      //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

These are C++ that extend the boost::random framework.

[ Useful links ]

http://www.boost.org/doc/libs/1_39_0/libs/random/index.html

[ models of RandomDistribution ]

    Let 
        T           scalar
        Z, X, U     model RandomDistribution

    Class                           Description
	chi_squared<T>				
    	students_t_distribution<T>         
	location_scale<Z>               X = sigma Z + mu
	categorical_distribution<U>     Samples indices by their probability
	ref_distribution<X&>            A reference wrapper
	gen_to_random<X>                A generator that acts like a RandomDist


[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

boost_1_39_0

[ History ]

August 18 2009  : renamed multinomial_distribution to categorical_distribution
July 2009       : Current version


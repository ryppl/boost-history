//////////////////////////////////////////////////////////////////////////////
// random::doc::readme                                                      //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

er.ci.2020 at gmail.com

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
	categorical_distribution<U>     Samples indices by their probability (deprecated)
	ref_distribution<X&>            A reference wrapper
	gen_to_random<X>                A generator that acts like a RandomDist


[ Platform/Compiler ]

Mac OS Leopard 10.5         gcc version 4.0.1
                            Intel C++ compiler 11.1

[ Dependencies ]

boost_1_40_0

[ History ]

Sept   25 2009  : location_scale : modified constructor and constrain Z to be 
a class type because a reference would be redundant with ref_distribution.
August 18 2009  : renamed multinomial_distribution to categorical_distribution
July 2009       : Current version


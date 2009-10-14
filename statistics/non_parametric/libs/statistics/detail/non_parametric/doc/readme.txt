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

    C++ class for non-parametric estimation.

[ Design ]

    The argument(s) passed to a statistic has to be a type that is an instance 
    of fusion::map i.e. its elements, say x and y are identified by two keys, 
    k1 and k2, respectively. 
    
    If the data is not of this type, for example, if x and y are stored in 
    separate containers, a combination of zip_iterator and transform_iterator 
    can achieve the requirement. 
    
    This approach is not less general, but somewhat more straightforward
    than leaving the type unspecified but specifying extractor functions for x 
    and y. 

[ Compiler ]

    gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

    /boost_1_40_0

[ History] 

    October 11th 2009. First version. Implementation of kolmogorov_smirnov
    transfered from distribution_toolkit
	

	
                        
        

                                   
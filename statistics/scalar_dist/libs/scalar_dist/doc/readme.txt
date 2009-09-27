//////////////////////////////////////////////////////////////////////////////
// scalar_dist::doc::readme                                                 //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

These are C++ classes that extend boost/math/distributions by providing 

    - wrappers that decouple the function, such as pdf, from the distribution 
    to which it is applied.
    - a distribution function iterator
    - mappings of distribution functionals (pdf->1/pdf) 

[ Useful links ]

http://www.boost.org/doc/libs/1_35_0/libs/math/doc/sf_and_dist/html/index.html

[ unscope ]

    pdf, cdf etc. are defined in scope boost::math. Unscope creates a version 
    defined in scope boost that forwards to that in boost::math. 
    
    The rationale for this, is that some function templates in other libraries 
    in sandbox/statitics apply to scalar distribution and joint distributions 
    alike, and therefore require a common interface.

[ fun_wrap ]

    Decouples the function, such as pdf, from the distribution D:
        fun_wrap::cdf_<D>::instance

[ iterator ]

    Thanks to the tools develepoped in fun_wrap, we may call, for example,
    
    std::copy(
        make_distribution_function_iterator<math::pdf_>(dist,b),
        make_distribution_function_iterator<math::pdf_>(dist,e),
        std::back_inserter(vec_pdf)
    );

[ map_pdf ] 

    Compare a hypothetical function:
        inverse_pdf(dist,x)
    to
        pdf(make_inverse_pdf(dist))
    The second form, used here, is more economical because we can reuse the 
    functionality associated with pdf, such as fun_wrap::pdf_. It is also 
    straightforward to combine mappings:
        Expression:                 Result type
        make_ratio_pdf(a,b)         product_pdf<A,inverse_pdf<B> >
    Usage:
        pdf(make_ratio_pdf(a,b),x)


[ meta ]

    - signature, pointer and delegate are behind fun_wrap's implementation
    - The meta-functions
        is_scalar_dist
        has_pdf
    distinguish between distributions and objects that aren't but share their
    functionality (such as those in map_pdf).
    
    They are used to manage the overload set non-intrusively (SFINAE), by client
    libraries, where both scalar and joint dists are valid arguments to 
    a function template.
        
[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

boost_1_39_0
/sandbox/statistics/standard_distribution/ 

[ History ]

Sep 16 2009 : deleted /algorithm and added /iterator
July 2009   : First version


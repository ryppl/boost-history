//////////////////////////////////////////////////////////////////////////////
// statistics::doc::readme                                                  //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

I welcome feedback including bug reports at erwann.rogard@gmail.com

[ Overview ]

    This C++ library provides statistical tools that builds on the STL/Boost framework. 
    
[ Themes ]

    * adaptive_rejection_sampling : see http://www.maths.leeds.ac.uk/~wally.gilks/adaptive.rejection/web_page/Welcome.html

    * arithmetic

    * autocovariance : see www.ims.nus.edu.sg/Programs/mcmc/files/berg_tl3.pdf

    * binary_op : from a pair of sequence to a sequence of pairs and vice-versa

    * dist_random : maps http://www.boost.org/doc/libs/1_35_0/libs/math/doc/sf_and_dist/html/math_toolkit/dist.html 
	to their http://www.boost.org/doc/libs/1_39_0/libs/random/random-distributions.html

    * estimator_concept

    * functional

    * goodness_of_fit

    * importance_sampling

    * importance_weights

    * iterator

    * joint_dist : Same as scalar_dist but in the multivariate framework.

    * kernel  : Rosenblatt-Parzen and Nadaraya-Watson

    * matrix_view : uses a vector and a stride to create a row-major matrix view

    * model : abstraction for likelihood computations

    * monomials_horner

    * mpl

    * nn1 : nearest neighbor tools

    * non_param : functions of the empircal cdf

    * random : an extension of random distributions

    * scalar_dist : tools for http://www.boost.org/doc/libs/1_35_0/libs/math/doc/sf_and_dist/html/math_toolkit/dist.html

    * standard_distribution : an extension of http://www.boost.org/doc/libs/1_35_0/libs/math/doc/sf_and_dist/html/math_toolkit/dist.html

    * survival_data

    * survival_model 

    * tree_view : uses a vector to create a tree view

    * vector_space 

[ Inside a theme ]
        
     boost/theme/
     libs/theme/
     	/doc/readme
        /example
        /src/main.cpp

[ Inside boost/theme ]

    Often contains a subset of
        /algorithm
        /data
        /iterator
        /functional
    which are named after the STL's subdivisions. 
                        
        

                                   
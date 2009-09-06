//////////////////////////////////////////////////////////////////////////////
// statistics::survival::data::doc::readme                                  //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

These C++ classes are abstractions for survival data (right censored, staggered 
entry) and associated tools. See survival_model.

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/boost_1_39_0/ 
/sandbox/statistics/arithmetic/ 
/sandbox/statistics/binary_op/ 
/sandbox/statistics/dist_random/ 
/sandbox/statistics/empirical_cdf/ 
/sandbox/statistics/matrix_view/ 
/sandbox/statistics/functional/ 
/sandbox/statistics/iterator/ 
/sandbox/statistics/importance_weights/
/sandbox/statistics/importance_sampling/
/sandbox/statistics/joint_dist/ 
/sandbox/statistics/random/ 
/sandbox/statistics/scalar_dist/ 
/sandbox/statistics/standard_distribution/ 
/sandbox/statistics/survival_model/ 


Link to : libboost_serialization-xgcc42-mt-1_39.a

[ History ]

July 2009 : Current version

[ Sources ]

http://en.wikipedia.org/wiki/Survival_analysis

[ Notation ]

    Consider, for example, a clinical trial. t is a time measured since the 
    beginning of the trial. rt is a time relative to a data-unit's entry time 
    into the trial. 

    Type  Class         Object id       Abstraction                 Concept
    alias    
    T                   t, t1, rt                                   is_scalar
    R     record<T>     r               entry & fail time                      
    E     event<T>      e               fail indic & event time
    Me    mean_event<T> me              mean indic & event time

[ data ] 

    R r(t,rt);
    Me me; 
    E e0 = make_event(r,t0); me += e0;
    E e1 = make_event(r,t1); me += e1;

[ algorithm ] 

    Algorithms that iterate over a dataset

[ random ] 

    Classes that simulate data given a model 
        
[ meta ]

    A bridge between statistics::survival::data and statistics::survival::model
    
    
[ Output ]

main.cpp

-> example_data :  records : (0,2) (1,2) (2,2) (3,2)  events : (1,2) (1,2) (0,1)
 size(x_cycle) = 4
mean_events : (2,(0,1)) (1,(1,2)) flattened mean_events : 0 1 1 2 <-
-> example_random : 
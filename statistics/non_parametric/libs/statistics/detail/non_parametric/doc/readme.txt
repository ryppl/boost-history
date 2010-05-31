//////////////////////////////////////////////////////////////////////////////
// non_parametric::doc::readme                                              //
//                                                                          //
//  (C) Copyright 2009-2010 Erwann Rogard                                   //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

er.ci.2020 <at> gmail.com

[ Overview ]

    C++ classes for non-parametric estimation using the Boost.Accumulator 
    framework.


[ Dependencies ]

    /boost_1_41_0
    /sandbox/statistics/support

[ Contingency Table ]

   Background:
   
   http://en.wikipedia.org/wiki/Contingency_table
   http://en.wikipedia.org/wiki/Pearson%27s_chi-square_test
      
   A 2-way contingency table records the counts, in a sample, of each possible 
   outcome or "class" of a variable, Z = (X,Y), where each of X and Y are 
   categorical variables.
   
   As given in this oder, it is customary to call X and Y the row and column 
   variables, respectively. 

   A factor is the vector of categories, known as "levels", representing the 
   domain of a marginal variable, in our case, X and Y.

   In a simple clinical trial, for example, we record X = 0 if a patient 
   receives the placebo, X = 1 if it receives the treatment, Y = 0 and Y = 1, 
   if his/her condition deteriorated or improved, respectively.
   
   Let j = 1,...,J index the classes of Z, each denoted z_j with probability
   p_j, and recorded count and frequency, respectively, n_j and q_j. Let 
   n = {sum n_j : j=1,...,J} denote the sample size. Each (z_j,n_j) pair is 
   called a cell.
            
   The above extends in an intuitive way to n-way tables.
      
   A hypothesis about the distribution of Z is usually formulated as follows:
      
       H0 = {p_j = k_j : j = 1,...,J}
   
   Evidence against H0 is aggregated into a chi-square statistic:
   
       X^2 = sum_j (q_j - k_j)^2 / k_j
       
   By statistical theory, if the sample is drawn under certain conditions of 
   randomness (iid), as n tends to infinity,
   
      X^2 ~ Chi-squared(df)
      
   where df is given by a formula that takes into account J, and H0. 
   Given alpha > 0, if X^2 > x such that  Pr(Chi-squared(df)>x) = alpha, H0 is
   rejected.
   
   For example, 
       H0 = {independence between rows and columns} 
   is equivalent to
   k_j = Pr(X=x)P(Y=y), for each j=(x,y). If P(X=x) and P(Y=y) are known, we 
   apply the above procedure directly. Otherwise, P(X=x) and P(Y=y) are 
   estimated from the table.
   
   

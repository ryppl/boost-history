//////////////////////////////////////////////////////////////////////////////
//  Boost.Assign v2                                                         //
//                                                                          //
//  Copyright (C) 2003-2004 Thorsten Ottosen                                //
//  Copyright (C) 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_ASSIGN_V2_LIMIT_CSV_ARITY_ER_2010_HPP
#define BOOST_ASSIGN_V2_LIMIT_CSV_ARITY_ER_2010_HPP
#ifndef BOOST_ASSIGN_V2_LIMIT_CSV_ARITY
//[limit_csv_arity
/* n = BOOST_ASSIGN_V2_CSV_LIMIT_ARITY
   Overload                           | Arity
   -------------------------------------
   csv_fun(t[0])                      |    1
   csv_fun(t[0], t[1])                |    2
   ...
   csv_fun(t[0], ...., t[n-1])        |    n
*/
#define BOOST_ASSIGN_V2_LIMIT_CSV_ARITY 20
//]
#endif // BOOST_ASSIGN_V2_LIMIT_CSV_ARITY
#endif // BOOST_ASSIGN_V2_LIMIT_CSV_ARITY_ER_2010_HPP
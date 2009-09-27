//////////////////////////////////////////////////////////////////////////////
// view::doc::readme                                                 //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

erwann.rogard@gmail.com

[ Overview ]

This C++ package provides a row-major matrix view of a vector.

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/boost_1_39_0/ 

[ iterator ]

	row_iterator

[ algorithm ]

	transform_column	

[ History ]

July 2009 :     Revamped row_iterator (previously vector2matrix) and added
                transform_column
March 2009 :    vector2matrix

[ Output ]

main.cpp

-> example_row_iterator
std::distance(b,e) : 0 0 1 1 2 2 3 3 <-
-> example_transform_column <-



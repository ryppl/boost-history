//////////////////////////////////////////////////////////////////////////////
// view::doc::readme                                                        //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////

[ Contact ]

er.ci.2020 at gmail.com

[ Overview ]

This C++ package extends sandbox/boost/view notably by providing a row-major representation of a matrix via a columns or rows iterator.

[ Compiler ]

gcc version i686-apple-darwin9-gcc-4.0.1 (GCC) 4.0.1

[ Dependencies ]

/boost_1_39_0/ 

[ iterator ]

	step_count_down_iterator
	rows_iterator
	columns_iterator


[ History ]

Sep 2009   :	Deleted all files. Replaced them with an interface/implementation closer to boost/view 

July 2009  :    Revamped row_iterator (previously vector2matrix) and added
                transform_column
March 2009 :    vector2matrix


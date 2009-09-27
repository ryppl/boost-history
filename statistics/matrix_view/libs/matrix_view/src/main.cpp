///////////////////////////////////////////////////////////////////////////////
// view::src::main.cpp                                                //  
//                                                                           //
//  (C) Copyright 2009 Erwann Rogard                                         //
//  Use, modification and distribution are subject to the                    //
//  Boost Software License, Version 1.0. (See accompanying file              //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
//#include <libs/matrix_view/example/row_iterator.h>
//#include <libs/matrix_view/example/transform_column.h>
#include <libs/matrix_view/example/step_iterator.h>
#include <libs/matrix_view/example/columns_iterator.h>
#include <libs/matrix_view/example/rows_iterator.h>

int main(){

    //example_row_iterator(std::cout);
    //example_transform_column(std::cout);
    //example_step_iterator(std::cout);
    //example_columns_iterator(std::cout);
    example_rows_iterator(std::cout);

    return 0;
}
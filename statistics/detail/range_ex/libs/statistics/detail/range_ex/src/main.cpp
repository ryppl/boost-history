//////////////////////////////////////////////////////////////////////////////
// range_ex::src::main.cpp                        							//
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <libs/statistics/detail/range_ex/example/nest_chain.h>

int main(){

	// Don't forget for your search paths that 
	// As of January 2010, /boost/range_ex is in the Vault.

	example_nest_chain(std::cout);

	return 0;
}
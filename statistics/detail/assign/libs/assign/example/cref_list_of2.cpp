//////////////////////////////////////////////////////////////////////////////
// example::list_of2.cpp                               						//
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <vector>
#include <boost/assign/list_of.hpp>
#include <boost/assign/cref_list_of2.hpp>
#include <libs/assign/example/cref_list_of2.h>

void example_cref_list_of2(std::ostream& os)
{
	os << "-> example_cref_listof2 : " << std::endl;
	using namespace boost::assign;
    
  	typedef std::vector<int> ints_;
    
    int a=1,b=2;
    
    ints_ ints;
    
    ints = cref_list_of<3>(a)(b)(3);
	BOOST_ASSERT(ints[0] == a);    
	BOOST_ASSERT(ints[1] == b);    
	BOOST_ASSERT(ints[2] == 3);    
    ints.clear();
	ints = cref_list_of2(a)(b)(3);     
	BOOST_ASSERT(ints[0] == a);    
	BOOST_ASSERT(ints[1] == b);    
	BOOST_ASSERT(ints[2] == 3);    
    
	os << "<- " << std::endl;
    
};

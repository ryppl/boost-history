//////////////////////////////////////////////////////////////////////////////
// example::cref_list_of.cpp                               					//
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <ostream>
#include <vector>
#include <algorithm>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/cref_list_of2.hpp>
#include <boost/assign/ref_list_of_rebind.hpp>
#include <boost/assign/list_of.hpp>
#include <libs/assign/example/cref_list_of2.h>

struct Foo
{
    int i;
    bool destroyed;
    Foo(){}
    Foo(int i) : i(i) { destroyed = false; }
    ~Foo() { destroyed = true;}
};
std::ostream & operator<<(std::ostream & os, Foo const & i)
{
    if(i.destroyed) 
        return os << "You have a bug\n";
    return os << i.i;
}


void example_cref_list_of(std::ostream& os)
{
	os << "-> example_cref_listof2 : ";
	using namespace boost::assign;
    
  	typedef std::vector<int> ints_;
    typedef boost::array<int,3> array_;
	array_ array0 = {{-1,-1,-1}};
	array_ array = array0;

	{    
    	int a=1,b=2,c=3;
    	ints_ ints;
    
    	// cref_list_of<int>
    	ints = cref_list_of<3>(a)(b)(3);
		BOOST_ASSERT(ints[0] == a);    
		BOOST_ASSERT(ints[1] == b);    
		BOOST_ASSERT(ints[2] == 3);    
        
        array = array0;
		array = cref_list_of<3>(a)(b)(3);
		BOOST_ASSERT(array[0] == a);    
		BOOST_ASSERT(array[1] == b);    
		BOOST_ASSERT(array[2] == c);    
        
		// cref_copy_list_of
    	ints.clear();
		ints = cref_copy_list_of(a)(b)(3);     
		BOOST_ASSERT(ints[0] == a);    
		BOOST_ASSERT(ints[1] == b);    
		BOOST_ASSERT(ints[2] == c);    
        array = array0;
		array = cref_copy_list_of(a)(b)(3);
		BOOST_ASSERT(array[0] == a);    
		BOOST_ASSERT(array[1] == b);    
		BOOST_ASSERT(array[2] == c);    
		{
    		ints.clear();
        	BOOST_AUTO(
        		tmp,
            	cref_copy_list_of(a)(b)(3)
        	);
        	ints = ints_(boost::begin(tmp),boost::end(tmp));
			BOOST_ASSERT(ints[0] == a);    
			BOOST_ASSERT(ints[1] == b);    
			BOOST_ASSERT(ints[2] == c);    
		}

        // ref_list_of
		{
            int a=1, b=2, c=3;
    		ints.clear();
        	BOOST_AUTO(
                tmp,
                cref_list_of(a)(b)(c)
            );
            {
        		ints = tmp; 
				BOOST_ASSERT(ints[0] == a);    
				BOOST_ASSERT(ints[1] == b);    
				BOOST_ASSERT(ints[2] == c);    
            }
            {
        		ints = ints_(boost::begin(tmp),boost::end(tmp));
				BOOST_ASSERT(ints[0] == a);    
				BOOST_ASSERT(ints[1] == b);    
				BOOST_ASSERT(ints[2] == c);    
            }
            int d = 4;
            std::fill(boost::begin(tmp),boost::end(tmp),d);
            d = 5;
            // Prints 4, 4, 4 instead of 5, 5, 5. However, works with Foo.
            // rvalue/lvalue?
            std::copy(
            	boost::begin(tmp),
                boost::end(tmp), 
                std::ostream_iterator<int>(os, "\n")
            );    
/*
            {
        		ints = ints_(boost::begin(tmp),boost::end(tmp));
				BOOST_ASSERT(ints[0] == d);    
				BOOST_ASSERT(ints[1] == d);    
				BOOST_ASSERT(ints[2] == d);    
            }
            {
            	// Before rev. Feb 9, 2010,  this required a,b,c, respectively
        		ints = tmp;
				BOOST_ASSERT(ints[0] == d);    
				BOOST_ASSERT(ints[1] == d);    
				BOOST_ASSERT(ints[2] == d);    
            }
*/
        }
        
    }

	os << "<- " << std::endl;
    
};

//////////////////////////////////////////////////////////////////////////////
// example::cref_list_of2.cpp                               				//
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
#include <boost/assign/list_of.hpp>
#include <libs/assign/example/cref_list_of2.h>

void example_cref_list_of2(std::ostream& os)
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
    
    	ints = cref_list_of<3>(a)(b)(3);
		BOOST_ASSERT(ints[0] == a);    
		BOOST_ASSERT(ints[1] == b);    
		BOOST_ASSERT(ints[2] == 3);    
        array = array0;
		array = cref_list_of<3>(a)(b)(3);
		BOOST_ASSERT(array[0] == a);    
		BOOST_ASSERT(array[1] == b);    
		BOOST_ASSERT(array[2] == c);    
        
		// cref_list_of2
    	ints.clear();
		ints = cref_list_of2(a)(b)(3);     
		BOOST_ASSERT(ints[0] == a);    
		BOOST_ASSERT(ints[1] == b);    
		BOOST_ASSERT(ints[2] == c);    
        array = array0;
		array = cref_list_of2(a)(b)(3);
		BOOST_ASSERT(array[0] == a);    
		BOOST_ASSERT(array[1] == b);    
		BOOST_ASSERT(array[2] == c);    
		{
    		ints.clear();
        	BOOST_AUTO(
        		tmp,
            	cref_list_of2(a)(b)(3)
        	);
        	ints = ints_(boost::begin(tmp),boost::end(tmp));
			BOOST_ASSERT(ints[0] == a);    
			BOOST_ASSERT(ints[1] == b);    
			BOOST_ASSERT(ints[2] == c);    
		}

		// ref_list_of2
    	ints.clear();
		ints = ref_list_of2(a)(b)(c);     
		BOOST_ASSERT(ints[0] == a);    
		BOOST_ASSERT(ints[1] == b);    
		BOOST_ASSERT(ints[2] == c);    
        array = array0;
		array = ref_list_of2(a)(b)(c);
		BOOST_ASSERT(array[0] == a);    
		BOOST_ASSERT(array[1] == b);    
		BOOST_ASSERT(array[2] == c);    
		{
    		ints.clear();
        	BOOST_AUTO(
                tmp,
                ref_list_of2(a)(b)(c)
            );
        	ints = ints_(boost::begin(tmp),boost::end(tmp));
			BOOST_ASSERT(ints[0] == a);    
			BOOST_ASSERT(ints[1] == b);    
			BOOST_ASSERT(ints[2] == c);    
            std::fill(boost::begin(tmp),boost::end(tmp),0);
			BOOST_ASSERT(a == 0);   
			BOOST_ASSERT(b == 0);   
			BOOST_ASSERT(c == 0);    
        }
		{
            int a=1, b=2, c=3, d = 4;
    		ints.clear();
        	BOOST_AUTO(
                tmp,
                cref_bind_list_of2(a)(b)(c)
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
            std::fill(boost::begin(tmp),boost::end(tmp),d);
            {
        		ints = ints_(boost::begin(tmp),boost::end(tmp));
				BOOST_ASSERT(ints[0] == d);    
				BOOST_ASSERT(ints[1] == d);    
				BOOST_ASSERT(ints[2] == d);    
            }
            {
            	// TODO : resolve inconsistency here
        		ints = tmp;
				BOOST_ASSERT(ints[0] == a);    
				BOOST_ASSERT(ints[1] == b);    
				BOOST_ASSERT(ints[2] == c);    
            }
        }
        
    }

	os << "<- " << std::endl;
    
};

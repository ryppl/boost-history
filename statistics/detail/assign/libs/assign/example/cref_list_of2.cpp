//////////////////////////////////////////////////////////////////////////////
// example::cref_list_of.cpp                                                //
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
#include <boost/assign/auto_size/ref_rebind_list_of.hpp>
#include <boost/assign/auto_size/ref_list_of.hpp>
#include <boost/assign/list_of.hpp>
#include <libs/assign/example/cref_list_of2.h>

void example_cref_list_of(std::ostream& os)
{
	os << "-> example_cref_listof2 : ";
	using namespace boost::assign;
    
  	typedef std::vector<int> ints_;
    typedef boost::array<int,3> array_;
	array_ array;

	// Since operator= calls begin() and end(), no need to test these separately

    {    
        // cref_list_of

    	int a=1, b=2, c=3;
    	ints_ ints;
    
    	{	os <<  "->Testing work in progress : csv " << std::endl;
    		typedef detail::auto_size::result_of_copy<const int,2>::type res_;
            {
            	BOOST_AUTO(tmp, cref_list_of(a)(b)(3));
//            	res_ res1(tmp); //calls copy constructor
//				BOOST_ASSERT(res1[0] == a);	
//				BOOST_ASSERT(res1[1] == b);	
//				BOOST_ASSERT(res1[2] == c);	
            }
            {
//            	res_ res2= ( cref_list_of(a)(b)(3) ); // what constructor?!
//				BOOST_ASSERT(res2[0] == a); 
//				BOOST_ASSERT(res2[1] == b);
//				BOOST_ASSERT(res2[2] == 3);
			}
            {
            	BOOST_AUTO(res3,cref_list_of(a,b,c)); // what contructor is called?!
				BOOST_ASSERT(res3[0] != a); //wrong
				BOOST_ASSERT(res3[1] != b); //wrong
				BOOST_ASSERT(res3[2] == c); //correct
            }
			os << "<-" << std::endl;
    	}
/*    
        {
    		ints.clear();
            ints = cref_list_of(a)(b)(3);     
            BOOST_ASSERT(ints[0] == a);    
            BOOST_ASSERT(ints[1] == b);    
            BOOST_ASSERT(ints[2] == c);    
        }
        {
            array.assign(-1);
            array = cref_list_of(a)(b)(3);
            BOOST_ASSERT(array[0] == a);    
            BOOST_ASSERT(array[1] == b);    
            BOOST_ASSERT(array[2] == c);    
        }
        {
            BOOST_AUTO(tmp,ref_list_of(a)(b)(c));
            std::fill(boost::begin(tmp),boost::end(tmp),0);
            BOOST_ASSERT(a == 0);    
            BOOST_ASSERT(b == 0);    
            BOOST_ASSERT(c == 0);    
        }
    }
    {
        // ref_rebind_list_of
        {
            int a=1, b=2, c=3;
            ints_ ints;
            ints.clear();
            BOOST_AUTO(tmp,cref_rebind_list_of(a)(b)(c));
            {
                ints = tmp; 
                BOOST_ASSERT(ints[0] == a);    
                BOOST_ASSERT(ints[1] == b);    
                BOOST_ASSERT(ints[2] == c);    
            }
            int d = 4;             
            tmp.assign(d);
            d = 5;
            {
                ints = tmp;
                BOOST_ASSERT(ints[0] == d);    
                BOOST_ASSERT(ints[1] == d);    
                BOOST_ASSERT(ints[2] == d);    
            }
        }
*/
    }
    os << "<- " << std::endl;
    
};

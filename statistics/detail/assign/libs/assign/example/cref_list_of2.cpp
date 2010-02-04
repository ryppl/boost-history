//////////////////////////////////////////////////////////////////////////////
// example::list_of2.cpp                               						//
//                                                                          //
//  (C) Copyright 2010 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <boost/timer.hpp>
#include <ostream>
#include <vector>
#include <boost/typeof/typeof.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assign/cref_list_of2.hpp>
#include <libs/assign/example/cref_list_of2.h>

void example_cref_list_of2(std::ostream& os)
{
	os << "-> example_cref_listof2 : " << std::endl;
	using namespace boost::assign;
    
  	typedef std::vector<int> ints_;

	{    
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
        
        BOOST_AUTO(
        	tmp,
            cref_list_of2(a)(b)(3)
        );
        ints = ints_(boost::begin(tmp),boost::end(tmp));
		BOOST_ASSERT(ints[0] == a);    
		BOOST_ASSERT(ints[1] == b);    
		BOOST_ASSERT(ints[2] == 3);    
        
    }
    {
		const int n = 100 * 1000;
        typedef std::vector<ints_> vec_ints_;
        ints_ a( n );
    	ints_ b( n );
		boost::timer t;

		{
			t.restart();    
			vec_ints_ vec_ints = cref_list_of<3>( a )( b )( ints_( n ) );
            double t_val = t.elapsed();
			os << "cref_list_of : t = " << t_val << std::endl;
            BOOST_ASSERT(vec_ints[0] == a);    
            BOOST_ASSERT(vec_ints[1] == b);    
            BOOST_ASSERT(vec_ints[2] == ints_( n ));    
        }
		{
			t.restart();    
			vec_ints_ vec_ints = cref_list_of2( a )( b )( ints_( n ) );
            double t_val = t.elapsed();
			os << "conversion cref_list_of2 : t = " << t_val << std::endl;
            BOOST_ASSERT(vec_ints[0] == a);    
            BOOST_ASSERT(vec_ints[1] == b);    
            BOOST_ASSERT(vec_ints[2] == ints_( n ));    
        }
		{
			t.restart();    
            BOOST_AUTO(
                tmp,
                cref_list_of2( a )( b )( ints_( n ) )
            );
			vec_ints_ vec_ints(boost::begin(tmp),boost::end(tmp));
            double t_val = t.elapsed();
			os << "copy cref_list_of2 : t = " << t_val << std::endl;
            BOOST_ASSERT(vec_ints[0] == a);    
            BOOST_ASSERT(vec_ints[1] == b);    
            BOOST_ASSERT(vec_ints[2] == ints_( n ));    
        }
        
	}        

    
	os << "<- " << std::endl;
    
};

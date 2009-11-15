//////////////////////////////////////////////////////////////////////////////
// multi_array::example::serialize.cpp                        				//
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <vector>
#include <string>
#include <fstream>
#include <boost/multi_array.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/statistics/detail/multi_array/serialization/serialize.hpp>
//#include <boost/serialization/vector.hpp>
#include <libs/statistics/detail/multi_array/example/serialize.h>

void example_multi_array_serialize(std::ostream& os)
{

	os << "multi_array_serialize ->" << std::endl;

    namespace ma = boost::detail::multi_array;

	typedef std::string str_;    
    typedef std::ofstream ofs_;
    typedef std::ifstream ifs_;
	typedef boost::archive::text_oarchive oa_;
	typedef boost::archive::text_iarchive ia_;
    typedef double val_;
	typedef boost::multi_array<val_, 2> array_;
    typedef std::vector<array_> vec_array_;
    typedef boost::array<ma::multi_array_base::index,2> extents_;
    const unsigned n0 = 2;
    const unsigned n1 = 2;
    const str_ path = "./multi_array_serialization";
    {
    	ofs_ ofs(path.c_str());
	    oa_ oa(ofs);
    	extents_ extents = {{n0,n1}};
		array_ a(extents);
		for(unsigned i = 0; i<n0; i++)
    	{
    		for(unsigned j = 0; j<n1; j++)
        	{
				a[i][j] = i * n1 + j; 
        	}
    	}
		boost::serialization::save(oa,a,0);
        
		// Neither of these compiles:
        // boost::serialization::serialize(oa,a,0);
        // oa << a;
    }
    {
    	ifs_ ifs(path.c_str());
    	ia_ ia(ifs);
    	array_ a;
		// Neither of these compiles:
        // boost::serialization::serialize(ia,a,0);
        // ia >> a;
        boost::serialization::load(ia,a,0);
		for(unsigned i = 0; i<n0; i++)
    	{
    		for(unsigned j = 0; j<n1; j++)
        	{
				BOOST_ASSERT(a[i][j] == i * n1 + j); 
                os  << a[i][j] << ' ';
        	}
    	}
    }
	os << "<-" << std::endl;
	
}


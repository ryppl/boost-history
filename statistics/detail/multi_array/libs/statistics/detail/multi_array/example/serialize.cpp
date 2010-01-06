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
#include <boost/mpl/int.hpp>
#include <boost/fusion/include/make_map.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/multi_array.hpp>
#include <boost/statistics/detail/multi_array/serialization/serialize.hpp>
#include <boost/statistics/detail/fusion/serialization/include.hpp>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/vector.hpp>
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
    typedef unsigned val_;
    typedef std::vector<val_> vec_;
    typedef boost::mpl::int_<0> tag_;
    typedef boost::fusion::result_of::make_map<tag_,val_>::type map_;
	typedef boost::multi_array<val_, 2> array_;
	typedef boost::multi_array<vec_,2> array_vec_;
    typedef boost::multi_array<map_,2> array_map_;
    typedef boost::array<ma::multi_array_base::index,2> extents_;
    const unsigned n0 = 2;
    const unsigned n1 = 2;
	{
    	const str_ path = "./multi_array_serialization";
    	{
    		ofs_ ofs(path.c_str());
	    	oa_ oa(ofs);
    		extents_ extents = {{ n0, n1 }};
			array_ a(extents);
        	// vec_ vec;
			for(unsigned i = 0; i<n0; i++)
    		{
    			for(unsigned j = 0; j<n1; j++)
        		{
					a[i][j] = i * n1 + j; 
        		}
    		}
        	oa << a;
    	}
    	{
    		ifs_ ifs(path.c_str());
    		ia_ ia(ifs);
    		array_ a;
        	ia >> a;
			for(unsigned i = 0; i<n0; i++)
    		{
    			for(unsigned j = 0; j<n1; j++)
        		{
					BOOST_ASSERT(a[i][j] == i * n1 + j); 
                	os  << a[i][j] << ' ';
        		}
    		}
    	}
    }
	{
    	const str_ path = "./multi_array_serialization2";
        const unsigned v0 = 0;
        const unsigned v1 = 1;
        const unsigned v2 = 2;
    	{
        
    		ofs_ ofs(path.c_str());
	    	oa_ oa(ofs);
    		extents_ extents = {{ n0, n1 }};
			array_vec_ a(extents);
        	vec_ vec; 
            vec.push_back(v0); 
            vec.push_back(v1);
			for(unsigned i = 0; i<n0; i++)
    		{
    			for(unsigned j = 0; j<n1; j++)
        		{
					a[i][j] = vec; 
                    a[i][j].resize(3); 
                    a[i][j][0] = v0;
                    a[i][j][1] = v1;
                    a[i][j][2] = v2;
        		}
    		}
        	oa << a;
    	}
    	{
    		ifs_ ifs(path.c_str());
    		ia_ ia(ifs);
    		array_vec_ a;
        	ia >> a;
			for(unsigned i = 0; i<n0; i++)
    		{
    			for(unsigned j = 0; j<n1; j++)
        		{
					BOOST_ASSERT(a[i][j][0] == v0); 
					BOOST_ASSERT(a[i][j][1] == v1); 
					BOOST_ASSERT(a[i][j][2] == v2); 
        		}
    		}
    	}
	}
	{
    	const str_ path = "./multi_array_serialization3";
    	{

        
    		ofs_ ofs(path.c_str());
	    	oa_ oa(ofs);
    		extents_ extents = {{ n0, n1 }};
			array_map_ a(extents);
			for(unsigned i = 0; i<n0; i++)
    		{
    			for(unsigned j = 0; j<n1; j++)
        		{
					map_ map = boost::fusion::make_map<tag_>(i * n1 + j);
					a[i][j] = map; 
        		}
    		}
        	oa << a;
    	}
    	{
    		ifs_ ifs(path.c_str());
    		ia_ ia(ifs);
    		array_map_ a;
        	ia >> a;
			for(unsigned i = 0; i<n0; i++)								
    		{
    			for(unsigned j = 0; j<n1; j++)
        		{
                	map_ map =  a[i][j]; 
                	val_ val = boost::fusion::at_key<tag_>(map);
					BOOST_ASSERT(
                    	val == i * n1 + j
                    ); 
                	os  << val << ' ';
        		}
    		}
    	}
	}

	os << "<-" << std::endl;
}


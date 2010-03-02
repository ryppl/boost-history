///////////////////////////////////////////////////////////////////////////////
// statistics::detail::fusion::example::serialize.cpp                  		 //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <vector>
#include <fstream>
#include <string>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/int.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/assert.hpp>
#include <boost/range.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/include/at_c.hpp>
#include <boost/fusion/include/pair.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/make_map.hpp>
#include <boost/fusion/include/make_vector.hpp>

#include <boost/statistics/detail/fusion/serialization/include.hpp>
#include <libs/statistics/detail/fusion/example/serialize.h>

void example_serialize(std::ostream& os){

    os << "example_serialize -> ";

    using namespace boost;
    namespace stat = boost::statistics::detail;
	namespace sf = stat::fusion;

    typedef mpl::int_<0> k0_;
    typedef mpl::int_<1> k1_;
    typedef mpl::int_<2> k2_;
	typedef int val_;
    typedef boost::fusion::vector2<val_,val_> vec2_int_;

    typedef boost::fusion::result_of::make_map<k0_,k1_,val_,val_>::type m01_;
    typedef boost::fusion::result_of::make_map<k2_,val_>::type m2_;
    typedef boost::fusion::vector2<m01_,m2_> vec_m01_m2_;
    typedef std::ofstream                           ofs_;
    typedef boost::archive::text_oarchive           oa_;
    typedef std::ifstream                           ifs_;
    typedef boost::archive::text_iarchive           ia_;
	typedef std::string 							str_;

    const val_ d0 = 0;
    const val_ d1 = 1;
    const val_ d2 = 2;

    const char* path1 = "./fusion_serialize_vec_int";
    const char* path2 = "./fusion_serialize_map";
    const char* path3 = "./fusion_serialize_vec_map";
	{
		vec2_int_ vec2_int(d0,d1);	
    	{
    		ofs_ ofs(path1);
    		oa_ oa(ofs);
        	oa << vec2_int;
            {
                vec2_int_ vec2_int;
                ifs_ ifs(path1);
                ia_ ia(ifs);
                // ia >> vec2_int_cpy;
                //BOOST_ASSERT(
                //    boost::fusion::at_c<0>( vec2_int ) == d0
                //);
                //BOOST_ASSERT(
                //    boost::fusion::at_c<1>( vec2_int ) == d1
                //);        
            }
        }
    }
/*
	{
        const str_ path = "./fusion_serialize_map";
    	m01_ m01 = boost::fusion::make_map<k0_,k1_>(d0,d1);
    	m2_ m2 = boost::fusion::make_map<k2_>(d2);
    	{
    		ofs_ ofs(path.c_str());
    		oa_ oa(ofs);
        	oa << m01;
        	oa << m2;
            {
                m01_ m01;
                m2_ m2;
                
                ifs_ ifs(path.c_str());
                ia_ ia(ifs);
                ia >> m01;
                BOOST_ASSERT(
                	boost::fusion::at_key<k0_>( m01 ) == d0
                );
                BOOST_ASSERT(
                    boost::fusion::at_key<k1_>( m01 ) == d1
                );        
                ia >> m2;
                BOOST_ASSERT(
                    boost::fusion::at_key<k2_>( m2 ) == d2
                );
            }
        }
	}        
	{
        const str_ path = "./fusion_serialize_vec_maps";
    	m01_ m01 = boost::fusion::make_map<k0_,k1_>(d0,d1);
    	m2_ m2 = boost::fusion::make_map<k2_>(d2);
    	vec_m01_m2_ vec_m01_m2(m01,m2);

    	{
    		ofs_ ofs(path.c_str());
    		oa_ oa(ofs);
        	oa << vec_m01_m2;
    	}
    	{
    		vec_m01_m2_ vec;

    		ifs_ ifs(path.c_str());
    		ia_ ia(ifs);
        	ia >> vec;
        	BOOST_ASSERT(
            	boost::fusion::at_key<k0_>( boost::fusion::at_c<0>(vec) ) == d0
    		);        
        	BOOST_ASSERT(
            	boost::fusion::at_key<k1_>( boost::fusion::at_c<0>(vec) ) == d1
    		);        
        	BOOST_ASSERT(
            	boost::fusion::at_key<k2_>( boost::fusion::at_c<1>(vec) ) == d2
    		);        
    	}
    }
*/
	os << "<-" << std::endl;

}

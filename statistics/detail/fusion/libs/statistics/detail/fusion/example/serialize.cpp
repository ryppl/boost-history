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

#include <boost/statistics/detail/fusion/map/serialization/include.hpp>
#include <libs/statistics/detail/fusion/example/serialize.h>

void example_serialize(std::ostream& os){

    os << "example_serialize -> ";

    using namespace boost;
    namespace stat = boost::statistics::detail;
	namespace sf = stat::fusion;

    typedef mpl::int_<0> k0_;
    typedef mpl::int_<1> k1_;
    typedef mpl::int_<2> k2_;
    typedef int d0_;
    typedef int d1_;
    typedef int d2_;
    typedef boost::fusion::result_of::make_map<k0_,k1_,d0_,d1_>::type 	map01_;
    typedef boost::fusion::result_of::make_map<k2_,d2_>::type 			map2_;
    typedef boost::fusion::result_of::make_vector<map01_,map2_>::type  	vec_map01_map2_;
    typedef boost::archive::text_iarchive           ia_;
    typedef boost::archive::text_oarchive           oa_;
    typedef std::ifstream                           ifs_;
    typedef std::ofstream                           ofs_;
	typedef std::string 							str_;

    const str_ path = "./serialize";
    const d0_ d0 = 0;
    const d1_ d1 = 1;
    const d2_ d2 = 2;
    
    map01_ map01(
    	boost::fusion::make_pair<k0_>(d0),
    	boost::fusion::make_pair<k1_>(d1)
    );
    map2_ map2(
    	boost::fusion::make_pair<k2_>(d2)
    );
    vec_map01_map2_ vec_map01_map2(map01,map2);
    {
    	ofs_ ofs(path.c_str());
    	oa_ oa(ofs);
        sf::serialization::make_saver(oa)(vec_map01_map2);
    }
    {
    	vec_map01_map2_ vec;
    	ifs_ ifs(path.c_str());
    	ia_ ia(ifs);
		sf::serialization::make_loader(ia)(vec);
        BOOST_ASSERT(
            boost::fusion::at_key<k0_>(
        		boost::fusion::at_c<0>(vec) 
            )== d0
    	);        
        BOOST_ASSERT(
            boost::fusion::at_key<k1_>(
        		boost::fusion::at_c<0>(vec) 
            )== d1
    	);        
        BOOST_ASSERT(
            boost::fusion::at_key<k2_>(
        		boost::fusion::at_c<1>(vec) 
            )== d2
    	);        
    }

    os << "<-" << std::endl;

}

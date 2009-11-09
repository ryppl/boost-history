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
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/map_fwd.hpp>
#include <boost/fusion/include/make_map.hpp>

#include <boost/statistics/detail/fusion/map/serialization/include.hpp>
#include <libs/statistics/detail/fusion/example/serialize.h>

void example_serialize(std::ostream& os){

    os << "example_serialize -> ";

    using namespace boost;
    namespace stat = boost::statistics::detail;
	namespace sf = stat::fusion;

    typedef mpl::int_<0> k_0_;
    typedef mpl::int_<1> k_1_;
    typedef int d_0_;
    typedef int d_1_;
    typedef fusion::pair<k_0_,d_0_> p_0_;
    typedef fusion::pair<k_1_,d_1_> p_1_;
    typedef fusion::map<p_0_,p_1_>  map_;
    typedef boost::archive::text_iarchive           ia_;
    typedef boost::archive::text_oarchive           oa_;
    typedef std::ifstream                           ifs_;
    typedef std::ofstream                           ofs_;
	typedef std::string 							str_;

    const str_ path = "./serialize";
    const d_0_ d_0 = 0;
    const d_1_ d_1 = 1;
    
    map_ map = boost::fusion::make_map<k_0_,k_1_>(d_0,d_1);
    {
    	ofs_ ofs(path.c_str());
    	oa_ oa(ofs);
		sf::map::save(map,oa);
    }
    {
    	map_ map;
    	ifs_ ifs(path.c_str());
    	ia_ ia(ifs);
		sf::map::load(map,ia);
		BOOST_ASSERT(boost::fusion::at_key<k_0_>(map) == d_0);        
		BOOST_ASSERT(boost::fusion::at_key<k_1_>(map) == d_1);        
    }

    os << "<-" << std::endl;

}

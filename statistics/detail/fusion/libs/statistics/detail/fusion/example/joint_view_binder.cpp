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
#include <boost/typeof/typeof.hpp>
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

#include <boost/statistics/detail/fusion/joint_view/binder.hpp>
//#include <boost/statistics/detail/fusion/serialization/save.hpp>
//#include <boost/statistics/detail/fusion/serialization/load.hpp>
//#include <boost/statistics/detail/fusion/serialization/map.hpp>
//#include <boost/statistics/detail/fusion/serialization/vector.hpp>

#include <libs/statistics/detail/fusion/example/serialize.h>

void example_joint_view_binder(std::ostream& os){

    os << "example_joint_view_binder -> ";
    
    using namespace boost;
    namespace stat = boost::statistics::detail;

    typedef mpl::int_<0> key0_;
    typedef mpl::int_<1> key1_;
    typedef mpl::int_<2> key2_;
    
    typedef unsigned d0_;
    typedef unsigned d1_;
    typedef unsigned d2_;
    typedef fusion::pair<key0_,d0_>       			p0_;
    typedef fusion::pair<key1_,d1_>                 p1_;
    typedef fusion::pair<key2_,d2_>                 p2_;
    typedef fusion::map<p0_>                      	map0_;
    typedef fusion::map<p1_>                      	map1_;
    typedef fusion::map<p2_>                      	map2_;
    
    const d0_ d0 = 0;
    const d1_ d1 = 0;
    const d2_ d2 = 0;
    map0_ map0(boost::fusion::make_pair<key0_>(d0));    
    map1_ map1(boost::fusion::make_pair<key1_>(d1));
    map2_ map2(boost::fusion::make_pair<key2_>(d2));
    {
    	typedef boost::mpl::empty_base left_;
        typedef stat::fusion::joint_view::binder<map0_> binder_;
        binder_ binder(map0);
        BOOST_AUTO(
        	jv,
            binder(map1)
        );
        BOOST_ASSERT(
        	boost::fusion::at_key<key0_>(jv) == d0
        );
        BOOST_ASSERT(
        	boost::fusion::at_key<key1_>(jv) == d1
        );
    }

 	os << "<-";   
}
///////////////////////////////////////////////////////////////////////////////
// statistics::detail::fusion::iterator_at_key_iterator.cpp                  //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <vector>
#include <boost/mpl/int.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/assert.hpp>
#include <boost/range.hpp>
#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/map_fwd.hpp>
#include <boost/statistics/detail/fusion/at_key/functor.hpp>
#include <boost/statistics/detail/fusion/at_key/iterator.hpp>

#include <boost/iterator/iterator_traits.hpp>

#include <libs/statistics/detail/fusion/example/iterator_at_key.h>

void example_iterator_at_key(std::ostream& os){

    os << "example_at_key_iterator -> ";

    using namespace boost;
    namespace stat = boost::statistics::detail;

    typedef mpl::int_<0> key_0_;
    typedef mpl::int_<1> key_1_;
    
    typedef fusion::pair<key_0_,int>                    p_0_;
    typedef fusion::pair<key_1_,int>                    p_1_;
    typedef fusion::map<p_0_,p_1_>                      map_;
    typedef stat::fusion::at_key::functor<key_0_>       get_0_;
    typedef stat::fusion::at_key::functor<key_1_>       get_1_;
    typedef std::vector<map_>                           vec_map_;
    typedef range_iterator<const vec_map_>::type        it_cvec_map_;
    typedef stat::fusion::at_key::meta_iterator<it_cvec_map_,key_0_> m_0_;
    typedef m_0_::type it_0_;
    typedef stat::fusion::at_key::meta_iterator<it_cvec_map_,key_1_> m_1_;
    typedef m_1_::type it_1_;
    
    const unsigned n = 10;
    
    vec_map_ vec_map; vec_map.reserve(n);
    for(unsigned i = 0; i<n; i++){
        map_ map(
            fusion::make_pair<key_0_>( i ),
            fusion::make_pair<key_1_>( i+1 )
        );
        vec_map.push_back(map);
    }

    {
        typedef it_0_   it_;
        typedef key_0_  key_;
    
        it_ b = stat::fusion::at_key::make_iterator<key_>(
            boost::begin(vec_map)
        );
        it_ e = stat::fusion::at_key::make_iterator<key_>(
            boost::end(vec_map)
        );
        unsigned j = 0;
        b<e;
        for(it_ i = b; i<e; i++,j++)
        {
            BOOST_ASSERT(
                *i == fusion::at_key<key_>(
                    vec_map[j]
                )
           );
        }
    }
    {
        typedef it_1_   it_;
        typedef key_1_  key_;

        it_ b = stat::fusion::at_key::make_iterator<key_>(
            boost::begin(vec_map)
        );
        it_ e = stat::fusion::at_key::make_iterator<key_>(
            boost::end(vec_map)
        );
        unsigned j = 0;
        for(it_ i = b; i<e; i++,j++)
        {
            BOOST_ASSERT(
                *i == fusion::at_key<key_>(
                    vec_map[j]
                )
           );
        }
    }
    
    os << "<-" << std::endl;

}




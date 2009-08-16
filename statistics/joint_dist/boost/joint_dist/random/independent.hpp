///////////////////////////////////////////////////////////////////////////////
// joint_dist::random::independent.hpp                                       //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_JOINT_DIST_RANDOM_INDEPENDENT_ER_2009
#define BOOST_JOINT_DIST_RANDOM_INDEPENDENT_ER_2009
#include <algorithm>
#include <boost/foreach.hpp>
#include <boost/concept_check.hpp>
#include <boost/iterator/iterator_concepts.hpp>
#include <boost/dist_random/random/sample.hpp>
#include <boost/joint_dist/distribution/independent.hpp>

namespace boost{

    // Warning: include this file after
    // <boost/dist_random/distributions/D.hpp>

    template<typename It,typename Ds,typename U>
    It sample(It out,const joint_dist::independent<Ds>& dist,U& urng);

    // Implem

    template<typename It,typename Ds,typename U>
    It sample(It out,const joint_dist::independent<Ds>& dist,U& urng){
        BOOST_CONCEPT_ASSERT((boost_concepts::IncrementableIterator<It>));
        BOOST_CONCEPT_ASSERT((boost_concepts::WritableIterator<It>));
        typedef typename joint_dist::independent<Ds> ind_;
        typedef typename ind_::element_type d_;
        // TODO: std::transform
        BOOST_FOREACH(const d_& d,dist.elements()){
            *out = sample(d,urng);
            ++out;
        }
        return out;
    }

}

#endif
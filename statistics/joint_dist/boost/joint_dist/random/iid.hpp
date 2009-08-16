///////////////////////////////////////////////////////////////////////////////
// joint_dist::random::iid.hpp                                               //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
///////////////////////////////////////////////////////////////////////////////
#ifndef BOOST_JOINT_DIST_RANDOM_IID_ER_2009
#define BOOST_JOINT_DIST_RANDOM_IID_ER_2009
#include <algorithm>
#include <boost/dist_random/random/generate_n.hpp>
#include <boost/joint_dist/distribution/iid.hpp>

namespace boost{

    // Warning: include this file after
    // <boost/dist_random/distributions/D.hpp>

    template<typename It,typename D,typename U>
    It sample(It out,const joint_dist::iid<D>& dist,U& urng);

    // Implem

    template<typename It,typename D,typename U>
    It sample(It out,const joint_dist::iid<D>& dist,U& urng){
        return generate_n(
            out, dist.dimension(),dist.element(), urng
        );
    }

}

#endif
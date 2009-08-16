///////////////////////////////////////////////////////////////////////////////
// kernel::example::multi_rp.cpp                                             //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/range.hpp>
#include <boost/foreach.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/math/special_functions/fpclassify.hpp> //needed?
#include <boost/math/tools/precision.hpp>

#include <boost/kernel/kernel/scalar/gaussian.hpp>
#include <boost/kernel/kernel/multi.hpp>
#include <boost/kernel/functional/rp_visitor.hpp>
#include <boost/kernel/functional/estimator.hpp>
#include <libs/kernel/example/scalar_rp.h>

void example_multi_rp(std::ostream& out){
    out << "-> example_multi_rp : ";
    using namespace boost;
    
    // Types
    typedef double                                          val_;
    typedef std::vector<val_>                               vec_;
    typedef std::vector<vec_>                               mat_;
    typedef mt19937                                         urng_;
    typedef normal_distribution<val_>                       norm_;
    typedef variate_generator<urng_&,norm_>                 gen_;
    typedef kernel::scalar::gaussian<val_>                  gauss_k_;

    const unsigned dim = 2;
    typedef kernel::multi<gauss_k_,dim>                     multi_k_;
    // NB const vec_&, not vec_
    typedef kernel::rp_visitor<multi_k_,const vec_&>        rp_visitor_;
    
    // Constants
    const val_ bandwidth = 0.5;
    const val_ eps = math::tools::epsilon<val_>();
    const unsigned n = 10;
    
    // Generate sample
    mat_ vec_x; vec_x.reserve(n);
    vec_ vec_rp; vec_rp.reserve(n);
    urng_ urng;
    norm_ norm;
    gen_ gen(urng,norm);
    for(unsigned i = 0; i<n; i++){
        vec_ tmp(dim);
        std::generate_n(
            begin(tmp),
            dim,
            gen
        );
        vec_x.push_back( tmp );
    }

    multi_k_ multi_k(bandwidth);
    
    multi_k(vec_x[0],vec_x[1]);
    // Density estimate for each x in vec_x using vec_x as the sample
    BOOST_FOREACH(const vec_& x,vec_x){
        val_ rp = std::for_each(
            begin(vec_x),
            end(vec_x),
            rp_visitor_(bandwidth,x)
        ).estimate();
        vec_rp.push_back(rp);
    } 
    typedef kernel::estimator<
        const mat_&,
        kernel::rp_visitor,
        multi_k_
    > estimator_;
    estimator_ estimator(vec_x,bandwidth);
    vec_ vec_rp2; vec_rp2.reserve(n);

    // Same as previous but calls estimator instead of for_each
    for(unsigned i = 0; i<n; i++){
        vec_ x = vec_x[i];
        val_ rp = vec_rp[i];
        val_ rp2 = estimator(x).estimate();
        BOOST_ASSERT(fabs(rp-rp2)<eps);
    } 
    out << "<-" << std::endl;
}
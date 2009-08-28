///////////////////////////////////////////////////////////////////////////////
// statistics::kernel::example::kernel_mono_rp.cpp                           //
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

#include <boost/statistics/kernel/scalar/gaussian.hpp>
#include <boost/statistics/kernel/joint/kernel_mono.hpp>
#include <boost/statistics/kernel/functional/rp_visitor.hpp>
#include <boost/statistics/kernel/functional/estimator.hpp>
#include <libs/statistics/kernel/example/scalar_rp.h>

void example_kernel_mono_rp(std::ostream& out){
    out << "-> example_kernel_mono_rp : ";
    using namespace boost;
    
    // Types
    typedef double                                          val_;
    typedef std::vector<val_>                               vec_;
    typedef std::vector<vec_>                               mat_;
    typedef mt19937                                         urng_;
    typedef normal_distribution<val_>                       norm_;
    typedef variate_generator<urng_&,norm_>                 gen_;
    typedef statistics::kernel::scalar::gaussian<val_>                  gauss_k_;

    const unsigned dim = 2;
    typedef statistics::kernel::joint::kernel_mono<gauss_k_,dim>               kernel_mono_k_;
    // NB const vec_&, not vec_
    typedef statistics::kernel::rp_visitor<kernel_mono_k_,const vec_&>  rp_visitor_;
    
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

    kernel_mono_k_ kernel_mono_k(bandwidth);
    
    kernel_mono_k(vec_x[0],vec_x[1]);
    // Density estimate for each x in vec_x using vec_x as the sample
    BOOST_FOREACH(const vec_& x,vec_x){
        val_ rp = std::for_each(
            begin(vec_x),
            end(vec_x),
            rp_visitor_(bandwidth,x)
        ).estimate();
        vec_rp.push_back(rp);
    } 
    typedef sub_range<mat_> sub_;
    typedef statistics::kernel::estimator<
        sub_,
        statistics::kernel::rp_visitor,
        kernel_mono_k_
    > estimator_;
    estimator_ estimator(bandwidth); 
    statistics::train(estimator,sub_(vec_x));
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
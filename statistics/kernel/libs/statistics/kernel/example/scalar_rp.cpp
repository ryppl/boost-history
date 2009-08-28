///////////////////////////////////////////////////////////////////////////////
// statistics::kernel::example::scalar_rp.cpp                                //
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
#include <boost/statistics/kernel/functional/rp_visitor.hpp>
#include <boost/statistics/kernel/functional/estimator.hpp>
#include <libs/statistics/kernel/example/scalar_rp.h>

void example_scalar_rp(std::ostream& out){
    out << "-> example_scalar_rp : ";
    using namespace boost;

    //Types
    typedef double                                          val_;
    typedef std::vector<val_>                               vec_;
    typedef mt19937                                         urng_;
    typedef normal_distribution<val_>                       norm_;
    typedef variate_generator<urng_&,norm_>                 gen_;
    typedef statistics::kernel::scalar::gaussian<val_>      gauss_k_;
    typedef statistics::kernel::rp_visitor<gauss_k_,val_>   rp_visitor_;

    // Contants
    const val_ bandwidth = 0.5;
    const val_ eps = math::tools::epsilon<val_>();
    const unsigned n = 10;

    // Initialization
    vec_ vec_x(n);
    vec_ vec_rp; vec_rp.reserve(n);
    urng_ urng;
    norm_ norm;
    gen_ gen(urng,norm);
    std::generate_n(
        begin(vec_x),
        n,
        gen
    );

    // Computes a density estimate for each x in vec_x using vec_x as sample
    BOOST_FOREACH(val_& x,vec_x){
        val_ rp = for_each(
            begin(vec_x),
            end(vec_x),
            rp_visitor_(bandwidth,x)
        ).estimate();
        vec_rp.push_back(rp);
    } 

    std::copy(
        begin(vec_rp),
        end(vec_rp),
        std::ostream_iterator<val_>(out," ")
    );
    
    typedef sub_range<vec_> sub_x_;
    typedef 
        statistics::kernel::estimator<
            sub_x_,
            statistics::kernel::rp_visitor,
            gauss_k_
        > estimator_;
    estimator_ estimator(bandwidth); 
    statistics::train(estimator,sub_x_(vec_x));
    vec_ vec_rp2; vec_rp2.reserve(n);
    
    // Same as previous but calls estimator instead of for_each
    for(unsigned i = 0; i<n; i++){
        val_ x = vec_x[i];
        val_ rp = vec_rp[i];
        val_ rp2 = estimator(x).estimate();
        BOOST_ASSERT(fabs(rp-rp2)<eps);
    } 
            
    out << "<-" << std::endl;
}
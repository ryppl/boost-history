///////////////////////////////////////////////////////////////////////////////
// statistics::detail::kernel::example::scalar_rp.cpp                        //
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
#include <boost/mpl/int.hpp>

#include <boost/statistics/detail/kernel/kernels/scalar/gaussian.hpp>
#include <boost/statistics/detail/kernel/estimation/rp_visitor.hpp>
#include <boost/statistics/detail/kernel/estimation/estimator.hpp>
#include <libs/statistics/detail/kernel/example/scalar_rp.h>

void example_scalar_rp(std::ostream& out){
    out << "-> example_scalar_rp : ";
    using namespace boost;

    // This example shows how to compute a Rosenblatt-Parzen estimate of the 
    // density, p(x). The type used for each data-unit, here, is double

    //Types
    typedef double                                          val_;
    typedef std::vector<val_>                               vec_;
    typedef mt19937                                         urng_;
    typedef normal_distribution<val_>                       norm_;
    typedef variate_generator<urng_&,norm_>                 gen_;
    typedef statistics::detail::kernel::scalar::gaussian_kernel<val_>      gauss_k_;
    typedef statistics::detail::kernel::rp_visitor<gauss_k_,val_>   rp_visitor_;

    // Contants
    const val_ bandwidth = 0.5;
    const val_ eps = math::tools::epsilon<val_>();
    const unsigned n = 10;

    // Initialization
    vec_ dataset(n);
    vec_ vec_rp; vec_rp.reserve(n);
    urng_ urng;
    norm_ norm;
    gen_ gen(urng,norm);
    std::generate_n(
        begin(dataset),
        n,
        gen
    );

    // Computes a density estimate for each x in dataset
    BOOST_FOREACH(val_& x,dataset){
        val_ rp = for_each(
            boost::begin(dataset),
            boost::end(dataset),
            rp_visitor_(bandwidth,x)
        ).estimate();
        vec_rp.push_back(rp);
    } 

    // Same as previous but calls estimator instead of for_each
    typedef sub_range<vec_> sub_x_;
    typedef 
        statistics::detail::kernel::estimator<
            sub_x_,
            statistics::detail::kernel::rp_visitor,
            gauss_k_
        > estimator_;
    estimator_ estimator(bandwidth); 
    sub_x_ sub_x(dataset);
    estimator.train(sub_x);
    vec_ vec_rp2; vec_rp2.reserve(n);
    
    for(unsigned i = 0; i<n; i++){
        val_ x = dataset[i];
        val_ rp = vec_rp[i];
        val_ rp2 = estimator.predict(x);
        BOOST_ASSERT(fabs(rp-rp2)<eps);
    } 
            
    out << "<-" << std::endl;

}
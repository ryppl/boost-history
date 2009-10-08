///////////////////////////////////////////////////////////////////////////////
// kernel::example::mono_bw_kernel_rp.cpp                                    //
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

#include <boost/statistics/detail/kernel/kernels/scalar/gaussian.hpp>
#include <boost/statistics/detail/kernel/kernels/multivariate/mono_bw.hpp>
#include <boost/statistics/detail/kernel/estimation/rp_visitor.hpp>
#include <boost/statistics/detail/kernel/estimation/estimator.hpp>
#include <libs/statistics/detail/kernel/example/scalar_rp.h>

void example_mv_mono_bw_rp(std::ostream& out){
    out << "-> example_mono_bw_kernel_rp : ";


    // This example shows how to compute a Rosenblatt-Parzen estimate of the 
    // density, p(x). The type used for each data-unit, x, is a vector of 
    // doubles, and the kernel uses the same bandwidth throughout all 
    // coordinates

    using namespace boost;
    namespace kernel = boost::statistics::detail::kernel;
    
    // Types
    typedef double                                          val_;
    typedef std::vector<val_>                               vec_;
    typedef vec_                                            x_;
    typedef std::vector<x_>                                 dataset_;
    typedef mt19937                                         urng_;
    typedef normal_distribution<val_>                       norm_;
    typedef variate_generator<urng_&,norm_>                 gen_;
    typedef kernel::scalar::gaussian_kernel<val_>                  gauss_k_;

    const unsigned dim = 2;
    typedef kernel::multivariate::mono_bw_kernel<gauss_k_,dim> mono_bw_kernel_k_;
    // Use of a const reference is not necessary but probably improves speed
    typedef kernel::rp_visitor<mono_bw_kernel_k_,const x_&>  rp_visitor_;
    
    // Constants
    const val_ bandwidth = 0.5;
    const val_ eps = math::tools::epsilon<val_>();
    const unsigned n = 10;
    
    // Generate n samples, each drawn from prod{N(0,1):i=1,...,dim}
    dataset_ dataset; dataset.reserve(n);
    vec_ vec_rp; vec_rp.reserve(n);
    urng_ urng;
    norm_ norm;
    gen_ gen(urng,norm);
    for(unsigned i = 0; i<n; i++){
        vec_ tmp(dim);
        std::generate_n(
            boost::begin(tmp),
            dim,
            gen
        );
        dataset.push_back( tmp );
    }

    // Density estimate for each x in dataset
    BOOST_FOREACH(const x_& x,dataset){
        val_ rp = std::for_each(
            boost::begin(dataset),
            boost::end(dataset),
            rp_visitor_(bandwidth,x)
        ).estimate();
        vec_rp.push_back(rp);
    } 
    typedef sub_range<dataset_> sub_;
    typedef kernel::estimator<
        sub_,
        kernel::rp_visitor,
        mono_bw_kernel_k_
    > estimator_;
    estimator_ estimator(bandwidth); 
    estimator.train(sub_(dataset));
    vec_ vec_rp2; vec_rp2.reserve(n);

    // Same as previous but calls estimator instead of for_each
    for(unsigned i = 0; i<n; i++){
        x_ x = dataset[i];
        val_ rp = vec_rp[i];
        val_ rp2 = estimator.predict(x);
        BOOST_ASSERT(fabs(rp-rp2)<eps);
    } 
    out << "<-" << std::endl;
}

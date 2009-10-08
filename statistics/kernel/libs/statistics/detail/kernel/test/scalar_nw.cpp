///////////////////////////////////////////////////////////////////////////////
// kernel::test::scalar_nw.cpp                                               //
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
#include <boost/math/tools/precision.hpp>

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>

#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/counting_iterator.hpp>

#include <boost/function.hpp>

#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/map_fwd.hpp>

#include <boost/statistics/detail/fusion/functor/at_key.hpp>
#include <boost/statistics/detail/cross_validation/error/sqrt_mse.hpp>
#include <boost/statistics/detail/cross_validation/data/input_output_generator.hpp>

#include <boost/statistics/detail/kernel/kernels/scalar/gaussian.hpp>
#include <boost/statistics/detail/kernel/bandwidth_selection/normal_distribution.hpp>
#include <boost/statistics/detail/kernel/bandwidth_selection/meta_k_fold_nw.hpp>

#include <libs/statistics/detail/kernel/test/scalar_nw.h>

void test_scalar_nw(std::ostream& os){

    os << "-> test_scalar_nw : " << std::endl; 
    os.flush();

    // Runs K-fold cross-validation for a sequence of datasets of
    // increasing size and reports the sqrt mse. Each data unit is 
    // of the form (x,f(y)).

    using namespace boost;

    namespace kernel = boost::statistics::detail::kernel;
    namespace cv = boost::statistics::detail::cross_validation;
    
    // Types
    typedef double                                          val_;
    typedef std::vector<val_>                               vals_;
    typedef boost::function<val_(const val_&)>              x_to_y_;
    typedef cv::data::input_output_generator<
        val_,
        x_to_y_
    > x_to_u_;
    typedef x_to_u_::meta_input_output meta_io_;
    typedef meta_io_::type                                  u_; // data-unit
    typedef meta_io_::input                                 meta_x_;
    typedef meta_io_::output                                meta_y_;
    typedef meta_x_::key_type                               key_x_;
    typedef meta_y_::key_type                               key_y_;
    typedef meta_x_::data_type                              data_x_;
    typedef meta_y_::data_type                              data_y_;
    
    typedef statistics::detail::fusion::functor::at_key<key_x_> at_key_x_;
    typedef statistics::detail::fusion::functor::at_key<key_y_> at_key_y_;

    typedef mt19937                                         urng_;
    typedef normal_distribution<val_>                       norm_;
    typedef variate_generator<urng_&,norm_>                 gen_;
    typedef kernel::scalar::gaussian_kernel<val_>           gauss_k_;
    typedef kernel::bandwidth_selection::normal_distribution<1,val_> bw_sel_;
    typedef kernel::bandwidth_selection::meta_k_fold_nw<
        at_key_x_,
        at_key_y_
    > meta_k_fold_nw_;
    
    typedef meta_k_fold_nw_::apply<
        u_,
        gauss_k_
    >::type k_fold_nw_;
    
    typedef unsigned n_;

    // num_fold * n_per_fold units at each iteration    
          n_ n_per_fold = 1;
    const n_ n_folds    = 10;    
    const n_ n_loops    = 5;
    const n_ n_factor   = 10; // n_per_fold *= n_factor at each iteration
        
    // Initialization
    vals_ pred_nw_s;
    vals_ true_nw_s;

    urng_ urng;
    norm_ norm;
    gen_ gen(urng,norm);
    
    typedef function<u_(const n_&)> index_to_u_;

    x_to_y_ x_to_y =
        boost::lambda::_1 * boost::lambda::_1;

    index_to_u_ index_to_u = boost::lambda::bind( 
        x_to_u_(x_to_y), 
        boost::lambda::bind(
            boost::ref(gen)
        )
    );

    os  << '('  << k_fold_nw_::description_header 
        << ','  << "bandwidth"
        << ','  << "sqrt_mse" 
        << ')'  << std::endl;
        
    for(n_ i = 0; i<n_loops; i++){
        n_ n = n_per_fold * n_folds;
        pred_nw_s.resize( n );
        true_nw_s.resize( n );

        k_fold_nw_ k_fold_nw(
            n_folds,
            make_transform_iterator(
                make_counting_iterator<n_>(0),
                index_to_u
            ),
            make_transform_iterator(
                make_counting_iterator<n_>(n),
                index_to_u
            )
        );
        
        // This bandwidth is not optimal for rp, not nw, but it should not 
        // matter too much
        val_ bandwidth = bw_sel_::rp_bandwidth(n); 
        k_fold_nw.cross_validate(
            bandwidth,
            boost::begin(pred_nw_s),
            boost::begin(true_nw_s)
        );

        val_ smse = cv::error::sqrt_mse(
            boost::begin(pred_nw_s),
            boost::end(pred_nw_s),
            boost::begin(true_nw_s)
        );

        os  << '(' <<   k_fold_nw 
            << ',' <<   bandwidth 
            << ',' <<   smse 
            << ')' <<   std::endl;
        
        n_per_fold *= n_factor;
    }

    os << "<-" << std::endl;
}
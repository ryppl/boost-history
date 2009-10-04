///////////////////////////////////////////////////////////////////////////////
// statistics::kernel::example::benchmark_scalar.cpp                         //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
/*

#include <cmath>
#include <vector>
#include <algorithm>
#include <iterator>
#include <boost/function.hpp>
#include <boost/range.hpp>
#include <boost/foreach.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>
#include <boost/math/special_functions/fpclassify.hpp> //needed?
#include <boost/math/tools/precision.hpp>

// Order of the files matters!
#include <boost/statistics/detail/distribution_toolkit/distributions/normal.hpp>
#include <boost/statistics/detail/distribution_toolkit/fwd_math/cdf.hpp> // ?!


#include <boost/binary_op/data/tuple_range.hpp>

#include <boost/statistics/kernel/include.hpp>

//#include <boost/statistics/kernel/bandwidth/normal_distribution.hpp>
//#include <boost/statistics/kernel/scalar/gaussian.hpp>
//#include <boost/statistics/kernel/functional/nw_visitor_tuple.hpp>
//#include <boost/statistics/kernel/functional/estimator.hpp>

#include <boost/statistics/goodness_of_fit/include.hpp>

#include <libs/statistics/kernel/example/benchmark_scalar.h>

void example_benchmark_scalar(std::ostream& out){
    out << "-> example_benchmark_scalar : ";
    out.flush();
    
    using namespace boost;
    namespace stat = statistics;
    namespace gof = stat::goodness_of_fit;

    // Types
    typedef double                                          val_;
    typedef function<val_(const val_&)>                     y_fun_;
    typedef std::vector<val_>                               vals_;
    typedef std::vector<unsigned>                           uints_;
    typedef mt19937                                         urng_;
    typedef normal_distribution<val_>                       rnorm_;
    typedef variate_generator<urng_&,rnorm_>                gen_;
    typedef statistics::kernel::scalar::gaussian<val_>      gauss_k_;
    typedef statistics::kernel::rp_visitor<gauss_k_,val_>   rp_visitor_;
    typedef math::normal_distribution<val_>                 mnorm_;
    
    // Constants
    const unsigned max_j = 5; // # partitions, k = 2^j
    const unsigned dim = 1;
    typedef statistics::kernel::normal_distribution<dim,val_> opt_;

    const unsigned total_n = 1e4;
    const val_ mu = 0.0;
    const val_ sigma = 2.0;

    // Initialization
    y_fun_ y_fun = fabs;
        
    vals_ vec_x( total_n );
    vals_ vec_y( total_n );
    vals_ vec_pdf( total_n );
    urng_ urng;
    rnorm_  rnorm(mu,sigma);
    gen_    gen(urng,rnorm);
    
    {   // Data generation
        mnorm_ mnorm(mu,sigma); 
        generate_function_n<math::fun_wrap::pdf_>(
            boost::begin(vec_x),
            boost::begin(vec_pdf),
            total_n,
            mnorm,
            urng
        );
        transform(
            boost::begin(vec_x),
            boost::end(vec_x),
            boost::begin(vec_y),
            y_fun
        );
    }
    {   // K-fold NW-estimation
        typedef binary_op::tuple_range<const vals_&,const vals_&> 
                                                        meta_range_xy_;
        typedef meta_range_xy_::type                    range_xy_;
        typedef range_value<range_xy_>::type            xy_ref_cv_;
        typedef binary_op::tuple_remove_ref_cv<
            xy_ref_cv_
        >::type xy_;

        range_xy_ range_xy = meta_range_xy_::make(vec_x,vec_y);
        typedef gof::k_fold_data< xy_ > k_fold_;
        typedef k_fold_::range_train_data_type range_train_;

        typedef statistics::kernel::estimator<
            range_train_,
            statistics::kernel::nw_visitor_tuple,
            gauss_k_
        > estimator_;

        vals_ mse;
        vals_ mae;
        uints_ ns;
        vals_ bandwidths;
        vals_ vec_est_y( total_n );
        unsigned k = 1;
        for(unsigned j = 1; j<max_j; j++){
            k *= 2;
            k_fold_ k_fold(
                k,
                boost::begin(range_xy),
                boost::end(range_xy)
            );
            ns.push_back( k_fold.n() );
            // Warning: This bandwidth is not optimal for nw (but better than
            // a wild guess).
            bandwidths.push_back(
                opt_::rp_bandwidth(sigma, k_fold.n())
            );
            estimator_ e(
                bandwidths.back()
            );
            gof::regression_estimate(
                k_fold,
                e,
                boost::begin( vec_est_y )
            );
            mse.push_back( 
                gof::sqrt_mse( 
                    boost::begin( vec_est_y ), 
                    boost::end( vec_est_y ), 
                    boost::begin( vec_y ) 
                ) 
            );
            mae.push_back( 
                gof::mean_abs_error( 
                    boost::begin( vec_est_y ), 
                    boost::end( vec_est_y ), 
                    boost::begin( vec_y )
                ) 
            );
        }
        
        out << std::endl << "NW estimation";
        out << std::endl <<"bandwidth : ";
        std::copy(
            boost::rbegin( bandwidths ),
            boost::rend( bandwidths ),
            std::ostream_iterator<val_>(out," ")
        );
        out << std::endl <<"n : ";
        std::copy(
            boost::rbegin( ns ),
            boost::rend( ns ),
            std::ostream_iterator<val_>(out," ")
        );
        out << std::endl <<"sqrt_mse : ";
        std::copy(
            boost::rbegin( mse ),
            boost::rend( mse ),
            std::ostream_iterator<val_>(out," ")
        );
        out << std::endl <<"mae : ";
        std::copy(
            boost::rbegin( mae ),
            boost::rend( mae ),
            std::ostream_iterator<val_>(out," ")
        );
            
    }
    {   // K-fold RP-estimation

        typedef gof::k_fold_data< val_ > k_fold_;
        typedef k_fold_::range_train_data_type range_train_;

        typedef statistics::kernel::estimator<
            range_train_,
            statistics::kernel::rp_visitor,
            gauss_k_
        > estimator_;

        vals_ mse;
        vals_ mae;
        uints_ ns;
        vals_ bandwidths;
        vals_ vec_est_pdf( total_n );
        unsigned k = 1;
        for(unsigned j = 1; j<max_j; j++){
            k *= 2;
            k_fold_ k_fold(
                k,
                boost::begin( vec_x ),
                boost::end( vec_x )
            );
            ns.push_back( k_fold.n() );
            // Note: This bandwidth is not optimal for density estimation
            // using a gaussian kernel, under normality of data
            bandwidths.push_back(
                opt_::rp_bandwidth(sigma, k_fold.n())
            );
            estimator_ e(
                bandwidths.back()
            );
            gof::marginal_estimate(
                k_fold,
                e,
                boost::begin( vec_est_pdf )
            );
            mse.push_back( 
                gof::sqrt_mse( 
                    boost::begin( vec_est_pdf ), 
                    boost::end( vec_est_pdf ), 
                    boost::begin( vec_pdf ) 
                ) 
            );
            mae.push_back( 
                gof::mean_abs_error( 
                    boost::begin( vec_est_pdf ), 
                    boost::end( vec_est_pdf ), 
                    boost::begin( vec_pdf )
                ) 
            );
        }
        out << std::endl << "RP estimation ";
        out << std::endl <<"bandwidth : ";
        std::copy(
            boost::rbegin( bandwidths ),
            boost::rend( bandwidths ),
            std::ostream_iterator<val_>(out," ")
        );
        out << std::endl <<"n : ";
        std::copy(
            boost::rbegin( ns ),
            boost::rend( ns ),
            std::ostream_iterator<val_>(out," ")
        );
        out << std::endl <<"sqrt_mse : ";
        std::copy(
            boost::rbegin( mse ),
            boost::rend( mse ),
            std::ostream_iterator<val_>(out," ")
        );
        out << std::endl <<"mae : ";
        std::copy(
            boost::rbegin( mae ),
            boost::rend( mae ),
            std::ostream_iterator<val_>(out," ")
        );
            
    }
    // DO the same for rp
    
    out << "<-" << std::endl;

}
*/




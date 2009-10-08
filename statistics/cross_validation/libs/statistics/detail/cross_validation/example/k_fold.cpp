//////////////////////////////////////////////////////////////////////////////
// cross_validation::example::k_fold.cpp                                    //
//                                                                          //
//  (C) Copyright 2009 Erwann Rogard                                        //
//  Use, modification and distribution are subject to the                   //
//  Boost Software License, Version 1.0. (See accompanying file             //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)        //
//////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <iterator>
#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/normal_distribution.hpp>
#include <boost/random/variate_generator.hpp>

#include <boost/math/tools/precision.hpp>

#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics/mean.hpp>
#include <boost/accumulators/statistics/stats.hpp>

#include <boost/statistics/detail/cross_validation/estimator/adaptor/include.hpp> 
#include <boost/statistics/detail/cross_validation/k_fold/include.hpp> 
#include <boost/statistics/detail/cross_validation/error/include.hpp> 

#include <libs/statistics/detail/cross_validation/example/k_fold.h>

void example_k_fold(std::ostream& os)
{

    os << "-> example_k_fold :" << std::endl;

    // This example shows how to perform K train/predict cycles.
    // The estimator is built upon a Boost.Accumulator for the mean.
    // The input (x) equals the input (y), in this case, and has type double
    
    using namespace boost;
    namespace stat = boost::statistics::detail;
    namespace cv = stat::cross_validation;
    namespace cv_es = cv::estimator;
    namespace cv_ex = cv::extractor;
    namespace cv_kf = cv::k_fold;

    typedef mt19937                                 urng_;
    typedef double                                  val_;
    typedef std::vector<val_>                       vals_;
    typedef range_iterator<vals_>::type             vals_it_;
    typedef boost::normal_distribution<val_>        nd_;
    typedef boost::variate_generator<urng_&,nd_>    vg_;

    // This comes up often, so this metafunction saves a bit of time
    typedef accumulators::tag::mean tag_;
    typedef accumulators::stats<
        tag_
    >  stat_;
    typedef accumulators::accumulator_set<val_,stat_> acc_;

    // Boost.Accumulators cannot be used directly. An adaptor is needed.
    typedef cv_es::adaptor::meta::nullary_predictor<acc_,tag_>      meta_p_;
    typedef cv_es::adaptor::joined<
        cv_es::adaptor::unary_trainer,
        meta_p_::apply,
        acc_
    > joined_;

    typedef cv_kf::partition<val_,cv_ex::identity> kf_p_;

    const unsigned n = 1e1;
    const unsigned k = 5e0;
    BOOST_ASSERT(n % k == 0);

    nd_ nd;
    urng_ urng;
    vg_ vg(urng,nd);

    vals_ vec_x;
    std::generate_n(
        std::back_inserter(vec_x), 
        n,
        vg
    );
    
    // vec_x is duplicated in kf_p, which is intenteded in this example. 
    // example_k_fold in sandbox/kernel shows how to avoid duplication
    
    kf_p_ kf_p( 
        k,
        boost::begin(vec_x),
        boost::end(vec_x)
    );
    
    struct float_{
            
        static bool equal(const val_& a, const val_& b){
            static val_ e = boost::math::tools::epsilon<val_>();;
            return fabs(a-b)< e;
        }
    
    };
    
    // Tests that concatenating the test-data over all k increments, is 
    // identical to vec_x.  
    for(unsigned i = 0; i<2; i++){ 
        if(i>0){
            // 2nd pass to check that resets properly
            kf_p.initialize(); 
        }
    
        vals_it_ vec_x_it  = boost::begin( vec_x );
        while(kf_p.index()<kf_p.n_folds()){
            os << kf_p << std::endl;
            BOOST_FOREACH(const val_& t,kf_p.subset2())
            {
                val_ t1 = *vec_x_it;
                BOOST_ASSERT( float_::equal(t,t1) );
                ++vec_x_it;
            }; 
            kf_p.increment();
        }
    }
    os << std::endl;

    // Cross-Validation #1
    vals_ preds(n);
    vals_it_ preds_b = boost::begin(preds); 
    vals_it_ preds_e; 
    BOOST_ASSERT(n / k > 1);
    kf_p.initialize();
    while(kf_p.index()<kf_p.n_folds()){
        acc_ acc;
        joined_ joined(acc);
        preds_e = train_predict(
            kf_p,
            joined,
            preds_b
        );
        val_ tmp = *preds_b;
        BOOST_AUTO(
            r,
            make_iterator_range(boost::next(preds_b),preds_e)
        );
        BOOST_FOREACH(const val_& v, r)
        {
            // Because we use a nullary_estimator 
            // a.k.a. marginal estimator in statistical terminology.
            BOOST_ASSERT(tmp == v);
        }
        
        preds_b = preds_e;
        kf_p.increment();
    }

    // Cross-Validation #2
    {
        vals_ output(n);
        acc_ acc;
        joined_ joined(acc);
        cross_validate(
            kf_p,
            joined,
            boost::begin(preds),
            boost::begin(output)
        );

        val_ sqrt_mse = cv::error::sqrt_mse(
            boost::begin(preds),
            boost::end(preds),
            boost::begin(output)
        );

        val_ mae = cv::error::mean_abs_error(
            boost::begin(preds),
            boost::end(preds),
            boost::begin(output)
        );
        
        os << "sqrt_mse = " << sqrt_mse << std::endl;
        os << "mae = " << mae << std::endl;

    }

    os << "<-" << std::endl;

}



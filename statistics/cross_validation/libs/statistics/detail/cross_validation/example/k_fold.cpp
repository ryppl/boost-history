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

//#include <boost/math/distributions/normal.hpp>
#include <boost/math/tools/precision.hpp>

//#include <boost/mpl/nested_type.hpp>

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
    using namespace boost;
    namespace stat = boost::statistics::detail;
    namespace cv = stat::cross_validation;
    namespace cv_es = cv::estimator;
    namespace cv_ex = cv::extractor;
    namespace cv_kf = cv::k_fold;

    typedef mt19937                                 urng_;
    typedef double                                  val_;
    typedef std::vector<val_>                       vals_;
    typedef range_iterator<const vals_>::type       const_vals_it_;
    typedef range_iterator<vals_>::type             vals_it_;
    typedef boost::normal_distribution<val_>        nd_;
    typedef boost::variate_generator<urng_&,nd_>    vg_;

    // This comes up often, so this metafunction saves a bit of time
    typedef accumulators::tag::mean tag_;
    typedef accumulators::stats<
        tag_
    >  stat_;
    typedef accumulators::accumulator_set<val_,stat_> acc_;

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

    vals_ vec_x;
//    dist_ dist;
    nd_ nd;
    urng_ urng;
    vg_ vg(urng,nd);

    std::generate_n(
        std::back_inserter(vec_x), 
        n,
        vg
    );

    kf_p_ kf_p(
        k,
        boost::begin(vec_x),
        boost::end(vec_x)
    );

//    os << kf_p;
    
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
        while(kf_p.j()<kf_p.k()){
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
    kf_p.initialize();
    os << std::endl;

    vals_ est_vec_x(n);
    vals_it_ est_vec_b = boost::begin(est_vec_x); 
    vals_it_ est_vec_e; 
    BOOST_ASSERT(n / k > 1);
    while(kf_p.j()<kf_p.k()){
        acc_ acc;
        joined_ joined(acc);
        est_vec_e = train_predict(
            kf_p,
            joined,
            est_vec_b
        );
        val_ tmp = *est_vec_b;
        BOOST_AUTO(
            r,
            make_iterator_range(boost::next(est_vec_b),est_vec_e)
        );
        BOOST_FOREACH(const val_& v, r)
        {
            // Because we use a nullary_estimator 
            // a.k.a. marginal estimator in statistical terminology.
            BOOST_ASSERT(tmp == v);
        }

        val_ sqrt_mse = cv::error::sqrt_mse(
            est_vec_b,
            est_vec_e,
            boost::begin(kf_p.output_range())
        );

        val_ mae = cv::error::mean_abs_error(
            est_vec_b,
            est_vec_e,
            boost::begin(kf_p.output_range())
        );
        
        os << "sqrt_mse = " << sqrt_mse << std::endl;
        os << "mae = " << mae << std::endl;
        
        est_vec_b = est_vec_e;
        kf_p.increment();
    }

    os << "<-" << std::endl;

}



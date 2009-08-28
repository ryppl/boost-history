//////////////////////////////////////////////////////////////////////////////
// statistics::goodness_of_fit::example::k_fold.cpp                        //
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
#include <boost/random/mersenne_twister.hpp>
#include <boost/format.hpp>
#include <boost/arithmetic/equal.hpp>
#include <boost/mpl/nested_type.hpp>
#include <boost/binary_op/include.hpp>
#include <boost/dist_random/distributions/normal.hpp>
#include <boost/standard_distribution/distributions/normal.hpp>
#include <boost/dist_random/random/include.hpp>

#include <libs/statistics/goodness_of_fit/example/average.hpp>
#include <boost/statistics/goodness_of_fit/include.hpp> 

//#include <boost/statistics/goodness_of_fit/data/k_fold_data.hpp> 
//#include <boost/statistics/goodness_of_fit/algorithm/k_fold_regression_estimate.hpp>
//#include <boost/statistics/goodness_of_fit/algorithm/sqrt_mse.hpp>
//#include <boost/statistics/goodness_of_fit/algorithm/mean_abs_error.hpp>

#include <libs/statistics/goodness_of_fit/example/k_fold.h>

void example_k_fold(std::ostream& out)
{

    // This example shows how to generate k-fold regression data (a sequence of
    // (x,y) tuples), and use it to generate a k-fold regression estimates    
    using namespace boost;
    namespace stat = boost::statistics;
    namespace gof = stat::goodness_of_fit;

    typedef mt19937                                 urng_;
    typedef double                                  val_;
    typedef std::vector<val_>                       vals_;
    typedef math::normal_distribution<val_>         dist_;
    typedef binary_op::tuple_range<const vals_&,const vals_&>     
                                                    meta_tuple_range_;
    typedef meta_tuple_range_::type                 tuple_range_;
    typedef range_value<tuple_range_>::type         tuple_ref_cv_;
    typedef binary_op::tuple_remove_ref_cv<
        tuple_ref_cv_
    >::type tuple_;
            
    typedef gof::detail::average<tuple_>            estimator_;
    typedef gof::k_fold_data<tuple_>                k_fold_data_;
    typedef k_fold_data_::test_data_type            test_data_;

    const unsigned n = 1e1;
    const unsigned k = 5e0;
    BOOST_ASSERT(n % k == 0);

    vals_ vec_x;
    vals_ vec_y;
    dist_ dist;
    urng_ urng;

    generate_n(
        std::back_inserter(vec_x), n,dist, urng
    );
    vec_y = vec_x;

    tuple_range_ vec_xy = meta_tuple_range_::make(
        vec_x,
        vec_y
    );

    k_fold_data_ k_fold_data(
        k,
        boost::begin(vec_xy),
        boost::end(vec_xy)
    );

    out << k_fold_data;

    // Tests that concatenating the test-data over all k increments, is 
    // identical to vec_xy.  
    for(unsigned i = 0; i<2; i++){
        if(i>0){ k_fold_data.initialize(); }
    
        typedef range_iterator<tuple_range_>::type tuples_it_;
        tuples_it_ tuples_it = boost::begin( vec_xy );
        while(k_fold_data.j()<k_fold_data.k()){
            typedef range_value<test_data_>::type t_;
            BOOST_FOREACH(const t_& t,k_fold_data.test_data())
            {
                val_ x  = get<0>(t);
                val_ x1 = get<0>(*tuples_it);
                val_ y  = get<1>(t);
                val_ y1 = get<1>(*tuples_it);
                BOOST_ASSERT( arithmetic_tools::equal(x,x1) );
                BOOST_ASSERT( arithmetic_tools::equal(y,y1) );
                ++tuples_it;
            }; 
            k_fold_data.increment();
        }
        out << std::endl << k_fold_data;
    }

    vals_ est_vec_y( size(vec_xy) );
    k_fold_data.initialize();
    out << std::endl;
    gof::regression_estimate(
        k_fold_data,
        estimator_(),
        boost::begin(est_vec_y)
    );

    val_ sqrt_mse = statistics::goodness_of_fit::sqrt_mse(
        boost::begin( est_vec_y ),
        boost::end( est_vec_y ),
        boost::begin( vec_y )
    );
    val_ mae = statistics::goodness_of_fit::mean_abs_error(
        boost::begin( est_vec_y ),
        boost::end( est_vec_y ),
        boost::begin( vec_y )
    );
    
    out << ( format("sqrt_mse = %1%")%sqrt_mse ).str() << std::endl;
    out << ( format("mae = %1%")%mae ).str() << std::endl;
    
    out << "k-fold regression estimates :";
    std::copy(
        boost::begin( est_vec_y ),
        boost::end( est_vec_y ),
        std::ostream_iterator<val_>(out," ")
    ); 
    // Note that average is a marginal estimate, so expect the data to come
    // out as : aabbccdd etc.
}

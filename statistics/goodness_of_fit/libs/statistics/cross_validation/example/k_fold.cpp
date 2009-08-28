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
#include <boost/statistics/goodness_of_fit/data/k_fold_data.hpp> 
#include <boost/statistics/goodness_of_fit/algorithm/k_fold_estimate.hpp>
#include <boost/statistics/goodness_of_fit/algorithm/sqrt_mse.hpp>
#include <boost/statistics/goodness_of_fit/algorithm/mean_abs_error.hpp>

#include <libs/statistics/goodness_of_fit/example/average.hpp>
#include <libs/statistics/goodness_of_fit/example/k_fold.h>

void example_k_fold(std::ostream& out)
{

    using namespace boost;
    namespace stat = boost::statistics;
    namespace cv = stat::goodness_of_fit;

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
            
    typedef cv::detail::average<tuple_>    estimator_;
    typedef cv::k_fold_data<tuple_>       k_fold_data_;
    typedef k_fold_data_::test_data_type                test_data_;

    const unsigned n = 1e1;
    const unsigned k = 5e0;
    BOOST_ASSERT(n % k == 0);

    vals_ vals;
    dist_ dist;
    urng_ urng;

    generate_n(
        std::back_inserter(vals), n,dist, urng
    );

    tuple_range_ tuples = meta_tuple_range_::make(
        vals,
        vals
    );


    k_fold_data_ k_fold_data(
        k,
        boost::begin(tuples),
        boost::end(tuples)
    );

    out << k_fold_data;

    for(unsigned i = 0; i<2; i++){
        if(i>0){ k_fold_data.initialize(); }
    
        typedef range_iterator<tuple_range_>::type tuples_it_;
        tuples_it_ tuples_it = boost::begin( tuples );
        while(k_fold_data.j()<k_fold_data.k()){
            typedef range_value<test_data_>::type t_;
            BOOST_FOREACH(const t_& t,k_fold_data.test_data())
            {
//                out << std::endl;
                val_ x  = get<0>(t);
                val_ x1 = get<0>(*tuples_it);
                val_ y  = get<1>(t);
                val_ y1 = get<1>(*tuples_it);
//                out << '(' << x << ',' << y << ')';
//                out << '(' << x1 << ',' << y1 << ')';
                BOOST_ASSERT( arithmetic_tools::equal(x,x1) );
                BOOST_ASSERT( arithmetic_tools::equal(y,y1) );
                ++tuples_it;
            }; 
            k_fold_data.increment();
        }
        out << std::endl << k_fold_data;
    }

    vals_ ests( size(tuples) );
    k_fold_data.initialize();
    out << std::endl;
    stat::estimate(
        k_fold_data,
        estimator_(),
        boost::begin(ests)
    );

    val_ sqrt_mse = statistics::goodness_of_fit::sqrt_mse(
        boost::begin(ests),
        boost::end(ests),
        boost::begin(vals)
    );
    val_ mae = statistics::goodness_of_fit::mean_abs_error(
        boost::begin(ests),
        boost::end(ests),
        boost::begin(vals)
    );
    
    out << ( format("sqrt_mse = %1%")%sqrt_mse ).str() << std::endl;
    out << ( format("mae = %1%")%mae ).str() << std::endl;
    
    out << "k-fold estimates :";
    std::copy(
        boost::begin(ests),
        boost::end(ests),
        std::ostream_iterator<val_>(out," ")
    );
}

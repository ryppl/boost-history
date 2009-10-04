///////////////////////////////////////////////////////////////////////////////
// statistics::detail::kernel::test::scalar_nw.cpp                           //
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
#include <boost/typeof/typeof.hpp>

#include <boost/lambda/lambda.hpp>
#include <boost/lambda/bind.hpp>
#include <boost/lambda/construct.hpp>

#include <boost/iterator/transform_iterator.hpp>

#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/map_fwd.hpp>

#include <boost/statistics/detail/fusion/functor/at_key.hpp>

#include <boost/statistics/detail/kernel/kernels/scalar/gaussian.hpp>
#include <boost/statistics/detail/kernel/bandwidth_selection/meta_k_fold_nw.hpp>
#include <libs/statistics/detail/kernel/test/scalar_nw.h>

void test_scalar_nw(std::ostream& out){

    out << "-> test_scalar_nw : ";
    using namespace boost;

    namespace kernel = boost::statistics::detail::kernel;
    
    // Types
    typedef double                                          val_;
    typedef std::vector<val_>                               vals_;
    typedef mpl::int_<0>                                    key_x_;
    typedef mpl::int_<1>                                    key_y_;
    typedef fusion::pair<key_x_,val_>                       x_;
    typedef fusion::pair<key_y_,val_>                       y_;
    typedef statistics::detail::fusion::functor::at_key<key_x_> at_key_x_;
    typedef statistics::detail::fusion::functor::at_key<key_y_> at_key_y_;
    typedef fusion::map<x_,y_>                              data_unit_;
    typedef std::vector<data_unit_>                         dataset_;
    // The rationale for data_range_ is it's cheap to copy
    typedef sub_range<dataset_>                             data_range_;
    
    typedef mt19937                                         urng_;
    typedef normal_distribution<val_>                       norm_;
    typedef variate_generator<urng_&,norm_>                 gen_;
    typedef kernel::scalar::gaussian_kernel<val_>           gauss_k_;
    typedef kernel::bandwidth_selection::meta_k_fold_nw<
        at_key_x_,
        at_key_y_
    > meta_k_fold_nw_;
    
    typedef meta_k_fold_nw_::apply<
        data_unit_,
        gauss_k_
    >::type k_fold_nw_;
    
    
    // Constants
    const val_ bandwidth = 0.5;
    const val_ eps = math::tools::epsilon<val_>();
    const unsigned n = 10;
    const unsigned num_fold = 5;
    
    // Initialization
    vals_ pred_nw_s(n);
    vals_ true_nw_s(n);
    dataset_ dataset;
    dataset.reserve(n);

    k_fold_nw_ k_fold_nw;
    {
        urng_ urng;
        norm_ norm;
        gen_ gen(urng,norm);
        val_ one = static_cast<val_>(1);
        for(unsigned i = 0; i<n; i++){
            dataset.push_back(
                data_unit_(
                    fusion::make_pair<key_x_>(gen()),
                    fusion::make_pair<key_y_>(one)
                )
            );
        }
        BOOST_AUTO(
            f,
            boost::lambda::bind(
                boost::lambda::constructor<data_unit_>(),
                boost::lambda::bind(
                    boost::lambda::constructor<x_>(),
                    boost::lambda::bind(
                        gen
                    )
                ),
                fusion::make_pair<key_y_>(one)
            )
        );
        
        
    }


    //k_fold_nw_ k_fold_nw(
    //    num_fold,
    //    make_transform_iterator
    //);

    


    out << "<-" << std::endl;
}
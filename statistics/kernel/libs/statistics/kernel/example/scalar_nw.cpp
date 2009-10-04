///////////////////////////////////////////////////////////////////////////////
// statistics::kernel::example::scalar_nw.cpp                                //
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

#include <boost/fusion/sequence/intrinsic/at_key.hpp>
#include <boost/fusion/include/at_key.hpp>
#include <boost/fusion/container/map.hpp>
#include <boost/fusion/include/map.hpp>
#include <boost/fusion/include/map_fwd.hpp>


#include <boost/statistics/detail/fusion/functor/at_key.hpp>

#include <boost/statistics/kernel/scalar/gaussian.hpp>
#include <boost/statistics/kernel/functional/meta_nw_visitor_unary.hpp>
#include <boost/statistics/kernel/functional/estimator.hpp>
#include <libs/statistics/kernel/example/scalar_nw.h>

void example_scalar_nw(std::ostream& out){

    out << "-> example_scalar_nw : ";
    using namespace boost;

    namespace kernel = boost::statistics::kernel;

    // This example shows how to compute a Nadaraya-Watson estimate of E[y|x]. 
    // The type used for each data-unit, here, is a fusion map whose x and y
    // components are accessed using keys
    
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
    typedef std::vector<data_unit_>                          dataset_;
    // The rationale for data_range_ is it's cheap to copy
    typedef sub_range<dataset_>                             data_range_;
        
    typedef mt19937                                         urng_;
    typedef normal_distribution<val_>                       norm_;
    typedef variate_generator<urng_&,norm_>                 gen_;
    typedef kernel::scalar::gaussian<val_>                  gauss_k_;
    typedef kernel::meta_nw_visitor_unary<
        at_key_x_,
        at_key_y_
    > meta_nw_visitor_u_;
    typedef meta_nw_visitor_u_::apply<
        gauss_k_,
        val_
    >::type  nw_visitor_u_;
    typedef nw_visitor_u_::nw_visitor_type              nw_visitor_;
    typedef nw_visitor_u_::rp_visitor_type              rp_visitor_;
    
    // Constants
    const val_ bandwidth = 0.5;
    const val_ eps = math::tools::epsilon<val_>();
    const unsigned n = 10;
    
    // Initialization
    vals_ vec_rp; vec_rp.reserve(n);
    vals_ vec_nw; vec_nw.reserve(n);
    dataset_ dataset;
    dataset.reserve(n);
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
    }

    // Computes nw = E[y|x] for each x in the dataset. The density (rp) is 
    // obtained as a by-product. Here, y = 1, so we should have 
    // rp = nw (un-normalized).
    BOOST_FOREACH(data_unit_& u,dataset){
        nw_visitor_ nw_visitor = std::for_each(
            boost::begin(dataset),
            boost::end(dataset),
            nw_visitor_u_(
                bandwidth,
                fusion::at_key<key_x_>(u)
            )
        );
        val_ u_nw = nw_visitor.unnormalized_estimate();
        vec_nw.push_back(u_nw);
        rp_visitor_ rp_visitor = nw_visitor.rp_visitor();
        val_ rp = rp_visitor.estimate();
        BOOST_ASSERT(fabs(rp-u_nw)<eps);
    }
    
    // Same as above using estimator
    
    typedef kernel::estimator<
        data_range_,
        meta_nw_visitor_u_::apply,
        gauss_k_
    > estimator_;
    estimator_ estimator(bandwidth);
    estimator.train(
        data_range_(dataset)
    ); // * step 1 *

    
    BOOST_FOREACH(data_unit_& u,dataset){
        // -> these steps are independent of step2, they're just a test
        val_ x = fusion::at_key<key_x_>(u);
        BOOST_AUTO( nw_v , estimator.visit(x) );
        val_ u_nw = nw_v.unnormalized_estimate();
        BOOST_AUTO( rp_v , nw_v.rp_visitor() );
        val_ rp = rp_v.estimate();
        BOOST_ASSERT(fabs(rp-u_nw)<eps);
        // <-
        
        estimator.predict(x); // * step 2 *
    
    } 
    
    out << "<-" << std::endl;
}

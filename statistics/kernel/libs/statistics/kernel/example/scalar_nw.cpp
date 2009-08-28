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

#include <boost/binary_op/data/tuple_range.hpp>
//#include <boost/statistics/estimator_concept/trainable_estimator/concept.hpp>
#include <boost/statistics/kernel/scalar/gaussian.hpp>
#include <boost/statistics/kernel/functional/nw_visitor_tuple.hpp>
#include <boost/statistics/kernel/functional/estimator.hpp>
#include <libs/statistics/kernel/example/scalar_nw.h>

void example_scalar_nw(std::ostream& out){
    out << "-> example_scalar_nw : ";
    using namespace boost;

    // Types
    typedef double                                          val_;
    typedef std::vector<val_>                               vec_;
    typedef mt19937                                         urng_;
    typedef normal_distribution<val_>                       norm_;
    typedef variate_generator<urng_&,norm_>                 gen_;
    typedef statistics::kernel::scalar::gaussian<val_>      gauss_k_;
    typedef statistics::kernel::nw_visitor_tuple<gauss_k_,val_> 
                                                            nw_visitor_tuple_;
    typedef nw_visitor_tuple_::nw_visitor_type              nw_visitor_;
    typedef nw_visitor_tuple_::rp_visitor_type              rp_visitor_;
    
    // Constants
    const val_ bandwidth = 0.5;
    const val_ eps = math::tools::epsilon<val_>();
    const unsigned n = 10;
    
    // Initialization
    vec_ vec_x(n);
    vec_ vec_y(n,static_cast<val_>(1)); 
    vec_ vec_rp; vec_rp.reserve(n);
    vec_ vec_nw; vec_nw.reserve(n);
    urng_ urng;
    norm_ norm;
    gen_ gen(urng,norm);
    std::generate_n(
        begin(vec_x),
        n,
        gen
    );

    // Computes a conditional mean estimate (nw) for each x in vec_x using 
    // a sequence of (x,y) tuples constructed from (vec_x,vec_y) as training
    // sample. The density (rp) is computed as a by-product. 
    // Here, y = 1, so we should have rp = nw (un-normalized).
    BOOST_FOREACH(val_& x,vec_x){
        typedef binary_op::tuple_range<const vec_&,const vec_&> factory_;
        typedef factory_::type range_tuple_;
        range_tuple_ range_tuple = factory_::make(vec_x,vec_y);
        nw_visitor_ nw_visitor = std::for_each(
            begin(range_tuple),
            end(range_tuple),
            nw_visitor_tuple_(bandwidth,x)
        ).nw_visitor();
        val_ u_nw = nw_visitor.unnormalized_estimate();
        vec_nw.push_back(u_nw);
        rp_visitor_ rp_visitor = nw_visitor.rp_visitor();
        val_ rp = rp_visitor.estimate();
        BOOST_ASSERT(fabs(rp-u_nw)<eps);
    } 

    typedef binary_op::tuple_range<const vec_&,const vec_&> factory_;
    typedef factory_::type range_xy_;
    range_xy_ range_xy = factory_::make(vec_x,vec_y);
    // A pair of iterators is cheap to copy so no need to pass it by reference
    typedef statistics::kernel::estimator<
        range_xy_,
        statistics::kernel::nw_visitor_tuple,
        gauss_k_
    > estimator_;
    estimator_ estimator(bandwidth);
    statistics::train(estimator,range_xy);
        
    // Same as previous but calls estimator instead of for_each
    BOOST_FOREACH(val_& x,vec_x){
        // A local definition of nw_visitor_ is needed because x is passed
        // by ref, not by value as in that outside the scope
        typedef estimator_::result<val_>::type result_type;
        typedef result_type::nw_visitor_type nw_visitor_;
        typedef result_type::rp_visitor_type rp_visitor_;
        nw_visitor_ nw_visitor = estimator(x).nw_visitor();
        val_ u_nw = nw_visitor.unnormalized_estimate();
        rp_visitor_ rp_visitor = nw_visitor.rp_visitor();
        val_ rp = rp_visitor.estimate();
        BOOST_ASSERT(fabs(rp-u_nw)<eps);
    } 
    
    // Shorter version of the above
    BOOST_FOREACH(val_& x,vec_x){
        BOOST_AUTO( nw_visitor , estimator(x).nw_visitor() );
        val_ u_nw = nw_visitor.unnormalized_estimate();
        BOOST_AUTO( rp_visitor , nw_visitor.rp_visitor() );
        val_ rp = rp_visitor.estimate();
        BOOST_ASSERT(fabs(rp-u_nw)<eps);
    } 
    
    out << "<-" << std::endl;
}
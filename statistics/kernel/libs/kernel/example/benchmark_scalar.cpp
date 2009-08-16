///////////////////////////////////////////////////////////////////////////////
// kernel::example::benchmark_scalar.cpp                                     //
//                                                                           //
//  Copyright 2009 Erwann Rogard. Distributed under the Boost                //
//  Software License, Version 1.0. (See accompanying file                    //
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)         //
/////////////////////////////////////////////////////////////////////////////// 
#include <vector>
#include <cmath>
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
#include <boost/standard_distribution/distributions/normal.hpp>
#include <boost/scalar_dist/fun_wrap/pdf.hpp>
#include <boost/scalar_dist/meta/delegate.hpp>
#include <boost/scalar_dist/algorithm/transform.hpp>

#include <boost/binary_op/data/tuple_range.hpp>
#include <boost/kernel/bandwidth/normal_distribution.hpp>
#include <boost/kernel/kernel/scalar/gaussian.hpp>
#include <boost/kernel/functional/benchmark_rp.hpp>
#include <boost/kernel/functional/benchmark_nw.hpp>
#include <boost/kernel/functional/nw_visitor_tuple.hpp>
#include <boost/kernel/functional/estimator.hpp>
#include <libs/kernel/example/benchmark_scalar.h>

void example_benchmark_scalar(std::ostream& out){
    out << "-> example_benchmark_scalar : ";

    using namespace boost;

    // Types
    typedef double                                          val_;
    typedef function<val_(const val_&)>                     y_delegate_;
    typedef std::vector<val_>                               vec_;
    typedef mt19937                                         urng_;
    typedef normal_distribution<val_>                       rnorm_;
    typedef variate_generator<urng_&,rnorm_>                gen_;
    typedef kernel::scalar::gaussian<val_>                  gauss_k_;
    typedef kernel::rp_visitor<gauss_k_,val_>               rp_visitor_;
    typedef math::normal_distribution<val_>                 mnorm_;
    typedef math::delegate<mnorm_>                          meta_delegate_;

    typedef const vec_& call_vec_;
    typedef kernel::benchmark_rp<call_vec_,call_vec_,gauss_k_> bench_rp_;
    typedef kernel::benchmark_nw<call_vec_,call_vec_,call_vec_,gauss_k_> 
        bench_nw_;
    
    // Constants
    const val_ bandwidth = 0.5;
    const val_ eps = math::tools::epsilon<val_>();
    const unsigned dim = 1;
    typedef kernel::normal_distribution<dim,val_> opt_;

    const unsigned n_loops = 10;
    const unsigned n_in_factor = 2; 
    unsigned n_in = 2;
    const unsigned n_out = 1e4;
    const val_ mu = 0.0;
    const val_ sigma = 2.0;

    // Initialization
    y_delegate_ y_delegate = fabs;
        
    vec_ vec_x_out(n_out);
    vec_ vec_y_out(n_out);
    vec_ vec_pdf_out(n_out);
        urng_ urng;
        rnorm_ rnorm(mu,sigma);
        gen_ gen(urng,rnorm);
    {

        std::generate_n(begin(vec_x_out),n_out,gen);

        mnorm_ mnorm(mu,sigma); 
        meta_delegate_::type mnorm_delegate
            = meta_delegate_::make<math::fun_wrap::pdf_>();

        math::transform<math::fun_wrap::pdf_>(
            mnorm,
            begin(vec_x_out),
            end(vec_x_out),
            begin(vec_pdf_out)
        );
        std::transform(
            boost::begin(vec_x_out),
            boost::end(vec_x_out),
            boost::begin(vec_y_out),
            y_delegate
        );
    }

    vec_ vec_x_in;     
    vec_ vec_y_in;     

    // Computations
    
    bench_rp_ bench_rp(vec_x_out,vec_pdf_out,gauss_k_(bandwidth));
    bench_nw_ bench_nw(vec_x_out,vec_pdf_out,vec_y_out,gauss_k_(bandwidth));
    out << "m = " << n_out << std::endl;
    out << "rp : " << bench_rp.header;
    out << "nw : " << bench_nw.header << std::endl;
    for(unsigned i = 0; i<n_loops; i++){
        vec_x_in.clear();
        vec_x_in.reserve(n_in);
        opt_::rp_bandwidth(sigma, n_in);
        std::generate_n( std::back_inserter(vec_x_in), n_in, gen);
        bench_rp(vec_x_in);
        
        vec_y_in.clear();
        vec_y_in.reserve(n_in);
        std::transform(
            boost::begin(vec_x_in),
            boost::end(vec_x_in),
            std::back_inserter(vec_y_in),
            y_delegate
        );
        bench_nw(vec_x_in,vec_y_in);

        out << "rp : " << bench_rp << " nw : " << bench_nw << std::endl;
                
        n_in *= n_in_factor;
    }
    
    
    out << "<-" << std::endl;
}




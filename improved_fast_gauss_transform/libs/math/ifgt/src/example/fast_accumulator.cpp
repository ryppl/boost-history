///////////////////////////////////////////////////////////////////////////////
// fast_accumulator.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <algorithm>
#include <boost/math/special_functions/binomial.hpp>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <iostream>
#include <boost/math/ifgt/fast/accumulator.hpp>
#include <boost/math/ifgt/truncation_degree/constant.hpp>
#include <libs/math/ifgt/src/example/fast_accumulator.h>
void example_fast_accumulator(){
    std::cout << "-> example_fast_accumulator" << std::endl;
    namespace math = boost::math;
    namespace ifgt = math::ifgt;
    namespace tag = ifgt::tag;
    namespace mpl = boost::mpl;

    const unsigned int dim = 2;
    const unsigned int wdim = 2;
    typedef std::vector<double>                                 var_type;
    typedef ifgt::truncation_degree::constant<mpl::_1>           trunc_degree;
    typedef ifgt::cluster<dim,wdim,trunc_degree,var_type>       cluster_type;
    typedef ifgt::find_nearest_cluster<mpl::_1, boost::l2_distance_squared>
                                                                find_type;
    typedef ifgt::kwd<>                                         kwd_t;
    typedef  ifgt::fast::accumulator<cluster_type,find_type>     acc_type;

    double bandwidth            = 0.1;
    double max_cluster_radius   = 1.0;
    unsigned degree             = 20;
    var_type    center0;
    var_type    center1;
    var_type    center2;
    var_type    source1;
    var_type    source2;
    var_type    weight;
    {
        using namespace boost::assign;
        center0  += -1.1,  0.1;
        center1  +=  0.2, -0.2;
        center2  +=  0.1,  0.1;
        source1  +=  0.15,-0.15;//nearest to center1
        source2  +=  0.11, 0.11;//nearest to center2
        weight  +=  1.0,   0.1;
    }

    acc_type acc((
            kwd_t::bandwidth = bandwidth,
            kwd_t::max_cluster_radius = max_cluster_radius,
            kwd_t::degree = degree
        )
    );

    acc(center0, weight);
    std::cout
        << "sources_count = " << acc.sources_count()
        << std::endl;
    acc(center1, weight);
    std::cout
        << "sources_count = " << acc.sources_count()
        << std::endl;
    acc(center2, weight);
    std::cout
        << "sources_count = " << acc.sources_count()
        << std::endl;
    acc(source1, weight);
    std::cout
        << "sources_count = " << acc.sources_count()
        << std::endl;
    acc(source2, weight);
    std::cout
        << "sources_count = " << acc.sources_count()
        << std::endl;

//    typedef clusters_type::iterator iter_type;

//    for(
//        iter_type i = boost::begin(clusters);
//        i<boost::end(clusters);
//        i++
//    ){
//        std::cout
//            << "cluster # "
//            << std::distance(boost::begin(clusters),i)
//            << std::endl;

//        {
//            const coeffs_type& sc
//                = i->collection_coefficients()[0];

//            std::cout << "1st weight: coeffs=";
//            copy(
//                boost::begin(sc()),
//                boost::end(sc()),
//                std::ostream_iterator<double>(std::cout," ")
//            ); std::cout << std::endl;

//        }
//        {
//            const coeffs_type& sc
//                = i->collection_coefficients()[1];

//            std::cout << "2nd weight: coeffs=";
//           copy(
//                boost::begin(sc()),
//                boost::end(sc()),
//               std::ostream_iterator<double>(std::cout," ")
//            ); std::cout << std::endl;

//        }
//    }//only cluster #1 should have coefficients


    std::cout << "<-" << std::endl;

}

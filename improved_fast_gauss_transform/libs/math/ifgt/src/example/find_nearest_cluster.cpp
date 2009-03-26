///////////////////////////////////////////////////////////////////////////////
// find_nearest_cluster.cpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#include <algorithm>
#include <vector>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <iostream>
#include <boost/assign/std/vector.hpp>
#include <boost/range.hpp>
#include <boost/math/ifgt/keyword.hpp>
#include <boost/math/ifgt/fast/detail/cluster.hpp>
#include <boost/math/ifgt/fast/find_nearest_cluster.hpp>
#include <libs/math/ifgt/src/example/find_nearest_cluster.h>

void example_find_nearest_cluster(){
    std::cout << "-> find_nearest_cluster" << std::endl;

    namespace mpl = boost::mpl;
    namespace math = boost::math;
    namespace ifgt = math::ifgt;
    namespace tag = ifgt::tag;

    const unsigned dim = 2;
    const unsigned wdim = 2;
    typedef double                                        value_type;
    typedef std::vector<value_type>                       var_type;
    typedef ifgt::truncation_degree::constant<mpl::_1>  trunc_degree;
    typedef ifgt::cluster<dim,wdim,trunc_degree,var_type>  cluster_type;
    typedef std::vector<cluster_type>                     clusters_type;
    typedef boost::l2_distance_squared                    dist_type;
    typedef ifgt::find_nearest_cluster<clusters_type,dist_type> find_nc_type;
    typedef ifgt::kwd<>                                     kwd_t;
    typedef cluster_type::coefficients_type          coeffs_type;
    typedef cluster_type::collection_coefficients_type coll_coeffs_type;

    const value_type bandwidth    =   0.1;
    const value_type max_cluster_radius  = 0.2;
    const unsigned degree = 20;
    var_type    center0;
    var_type    center1;
    var_type    center2;
    var_type    source;
    {
        using namespace boost::assign;
        center0  += -1.1,  0.1;
        center1  +=  0.2, -0.2;
        center2  +=  0.1,  0.1;
        source   +=  0.15,-0.15;
    }

    typedef mpl::apply<trunc_degree,value_type>::type trunc_degree_type;

    clusters_type clusters;
    clusters.push_back(cluster_type(
            center0,
            bandwidth,
            max_cluster_radius,
            trunc_degree_type(degree)));
    clusters.push_back(
        cluster_type((
            kwd_t::center = center1,
            kwd_t::bandwidth = bandwidth,
            kwd_t::max_cluster_radius = max_cluster_radius,
            kwd_t::degree = degree
        )));
    clusters.push_back(
        cluster_type((
            kwd_t::center = center2,
            kwd_t::bandwidth = bandwidth,
            kwd_t::max_cluster_radius = max_cluster_radius,
            kwd_t::degree = degree
        )));

    find_nc_type find_nc(clusters);
    typedef find_nc_type::iter_type iter_type;
    iter_type nc = find_nc(source);
    std::cout
        << "dist to nc = "
        << std::distance(boost::begin(clusters),nc)
        << std::endl;

    std::cout << "<-" << std::endl;
}

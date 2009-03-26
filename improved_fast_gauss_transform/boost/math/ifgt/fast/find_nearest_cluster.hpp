//////////////////////////////////////////////////////////////////////////////
// find_nearest_cluster.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_FIND_NEAREST_CLUSTER_HPP_ER_2009
#define BOOST_MATH_IFGT_FIND_NEAREST_CLUSTER_HPP_ER_2009
#include <cmath>
#include <stdexcept>
#include <boost/function.hpp>
#include <boost/range/value_type.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/algorithm/find_nearest_neighbor.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/math/ifgt/fast/detail/cluster.hpp>
#include <boost/math/ifgt/fast/detail/cluster_call_center.hpp>

namespace boost{namespace math{namespace ifgt{
    template<typename R, typename Distance = l2_distance_squared>
    class find_nearest_cluster{
        typedef range_iterator<R>                            ri_type;
     public:
        typedef typename ri_type::type                       iter_type;
        typedef typename range_value<R>::type                cluster_type;

        find_nearest_cluster(R& clusters):clusters_(clusters){}
        find_nearest_cluster(const find_nearest_cluster& that)
        :clusters_(that.clusters_){}
        template<typename R0>
        iter_type operator()(const R0& source){
            typedef cluster_call_center<cluster_type>             f_type;
            typedef transform_iterator<f_type,iter_type>
                                                                  iter_t_type;
            typedef iterator_range<iter_t_type>                   ir_type;
            f_type f;
            ir_type ir(
                iter_t_type(begin(clusters_),f),
                iter_t_type(end(clusters_),f)
            );

            typedef find_nearest_neighbor<ir_type,Distance> find_type;

            iter_type nn = find_type(ir)(source).base();
            return nn;
        }
     private:
        find_nearest_cluster& operator=(const find_nearest_cluster& that);
        R& clusters_;
    };

}}}
#endif

//////////////////////////////////////////////////////////////////////////////
// ifgt/fast_accumulator.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_FAST_ACCUMULATOR_IMPL_HPP_ER_2009
#define BOOST_MATH_IFGT_FAST_ACCUMULATOR_IMPL_HPP_ER_2009
#include <algorithm>
#include <vector>
#include <functional>
#include <numeric>
#include <boost/range.hpp>
#include <boost/bind.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/apply.hpp>
#include <iostream> //TODO remove
#include <boost/math/ifgt/tag.hpp>
#include <boost/math/ifgt/find_nearest_cluster.hpp>
#include <boost/math/ifgt/truncation_degree_constant.hpp>
#include <boost/math/ifgt/cutoff_radius_none.hpp>
#include <boost/math/ifgt/accumulator_base.hpp>
namespace boost{namespace math{namespace ifgt{


    // fast_accumulator maintains a set of clusters, which is split
    // into an inactive and an active range. each time a (source,weight)
    // is passed to the object, only the active range is updated
    // i.e. either the source is collected at the nearest cluster within
    // that range,
    // or the a new cluster is added to the active range,
    // or the source is ignored if it equals the center of nearest cluster.
    // each time (bandwith,max_radius) is reset, the beginning
    // of the active range is to the end of the clusters range.
    // This (optional) feature is useful in applications where the bandwidth
    // is modified (usually decreased) in a piecewise-constant fashion.
    // The client can also use push_back_cluster to add a center
    // to the active range

    /// apply<FindClusterPolicy,Cluster>::type
    template<
        typename Cluster,
        typename FindClusterPolicy
            = find_nearest_cluster<mpl::_1, l2_distance_squared>
    >
    class fast_accumulator:
     public fast_accumulator_base<
        fast_accumulator<Cluster,FindClusterPolicy> >{
      public:
        typedef Cluster                                      cluster_type;
        typedef typename cluster_type::center_type           center_type;
        typedef typename cluster_type::value_type            value_type;
        typedef typename cluster_type::coefficients_type
                                        coefficients_type;
        typedef typename cluster_type::collection_coefficients_type
                                        collection_coefficients_type;
        static const std::size_t source_size  = cluster_type::source_size;
        static const std::size_t weight_size  = cluster_type::weight_size;
      private:
        typedef std::vector<cluster_type>                    clusters_type;
        typedef typename clusters_type::size_type            size_type;
        static size_type default_clusters_reserve_size(){
            static size_type sz = 1000; return sz; }
        typedef typename clusters_type::difference_type      diff_type;
        typedef range_iterator<clusters_type>               ri_type;
        typedef typename ri_type::type                       iter_type;
        typedef iterator_range<iter_type>    active_range_type;
        typedef typename cluster_type::truncation_degree_policy_type
                                            truncation_degree_policy_type;
      public:
        typedef typename cluster_type::sources_count_type  sources_count_type;

        //Construct
        fast_accumulator();
        template<typename ArgPack>
        fast_accumulator(const ArgPack& args);
        fast_accumulator(const fast_accumulator& that);
        //Assign
        fast_accumulator& operator=(const fast_accumulator& that);

        /// Triggers a new active range
        void set(value_type bandwidth,value_type max_radius);
        void set_truncation_degree_policy(
            const truncation_degree_policy_type& arg);

        //Access
        value_type active_bandwidth()const;
        value_type active_max_radius()const;
        const clusters_type& clusters()const;
        sources_count_type sources_count()const;

        //Update
        /// Used internally, but passing clusters directly also allowed.
        template<typename R>
        void push_back_cluster(const R& center);

        template<typename R0,typename R1>
        bool operator()(const R0& source, const R1& weight);

      private:
        active_range_type active_range();
        diff_type                       active_dist_to_;
        value_type                      active_bandwidth_;
        value_type                      active_max_radius_;
        truncation_degree_policy_type   truncation_degree_policy_;
        clusters_type                   clusters_;
    };

    template<typename Cluster, typename FindClusterPolicy>
        fast_accumulator<Cluster,FindClusterPolicy>::fast_accumulator()
        :   active_dist_to_((diff_type)(0)),
            active_bandwidth_((value_type)(0)),
            active_max_radius_((value_type)(0)),
            //make sure gen comp error if deflt constr inexistant
            truncation_degree_policy_(),
            clusters_()
            { clusters_.reserve(default_clusters_reserve_size()); }

    template<typename Cluster,typename FindClusterPolicy>
        template<typename ArgPack>
        fast_accumulator<Cluster,FindClusterPolicy>::fast_accumulator(
        const ArgPack& args)
        :   active_dist_to_((diff_type)(0)),
            active_bandwidth_(args[tag::bandwidth|(value_type)(0)]),
            active_max_radius_(args[tag::max_cluster_radius|(value_type)(0)]),
            truncation_degree_policy_(args){
                clusters_.reserve(
                    args[tag::clusters_reserve_size
                        |default_clusters_reserve_size()]);
            }

    template<typename Cluster,typename FindClusterPolicy>
        fast_accumulator<Cluster,FindClusterPolicy>::fast_accumulator(
            const fast_accumulator& that)
        :   active_dist_to_(that.active_dist_to_),
            active_bandwidth_(that.active_bandwidth_),
            active_max_radius_(that.active_max_radius_),
            clusters_(that.clusters_){}

    template<typename Cluster,typename FindClusterPolicy>
        typename fast_accumulator<Cluster,
        FindClusterPolicy>::fast_accumulator&
        fast_accumulator<Cluster,FindClusterPolicy>::operator=(
        const fast_accumulator& that){
            if(&that!=this){
                active_dist_to_     = that.active_dist_to_;
                active_bandwidth_   = that.active_bandwidth_;
                active_max_radius_  = that.active_max_radius_;
                clusters_           = that.clusters_;
            }
            return *this;
        }

    template<typename Cluster,typename FindClusterPolicy>
        void fast_accumulator<Cluster,FindClusterPolicy>::set(
        value_type bandwidth,value_type max_radius){
            active_bandwidth_ = bandwidth;
            active_max_radius_ = max_radius;
            active_dist_to_
              = std::distance(begin(clusters_), end(clusters_));
        }

    template<typename Cluster,typename FindClusterPolicy>
        void
        fast_accumulator<Cluster,
            FindClusterPolicy>::set_truncation_degree_policy(
            const truncation_degree_policy_type& truncation_degree_policy){
            truncation_degree_policy_ = truncation_degree_policy;
    }

    template<typename Cluster,typename FindClusterPolicy>
        typename fast_accumulator<Cluster,FindClusterPolicy>::value_type
        fast_accumulator<Cluster,FindClusterPolicy>::active_bandwidth()const{
            return active_bandwidth_; }

    template<typename Cluster,typename FindClusterPolicy>
        typename fast_accumulator<Cluster,FindClusterPolicy>::value_type
        fast_accumulator<Cluster,FindClusterPolicy>::active_max_radius()const{
            return active_max_radius_; }

    template<typename Cluster,typename FindClusterPolicy>
        typename fast_accumulator<Cluster,
        FindClusterPolicy>::active_range_type
        fast_accumulator<Cluster,FindClusterPolicy>::active_range(){
            iter_type i = begin(clusters_);
            std::advance(i,active_dist_to_);
            return active_range_type(i,end(clusters_));
        }

    template<typename Cluster, typename FindClusterPolicy>
        const typename fast_accumulator<Cluster,
        FindClusterPolicy>::clusters_type&
        fast_accumulator<Cluster,FindClusterPolicy>::clusters()const{
            return clusters_; }

    template<typename Cluster,typename FindClusterPolicy>
        typename fast_accumulator<Cluster,
        FindClusterPolicy>::sources_count_type
        fast_accumulator<Cluster,FindClusterPolicy>::sources_count()const{
            return std::accumulate(
                begin(clusters_),
                end(clusters_),
                (sources_count_type)(0),
                bind(
                    std::plus<value_type>(),
                    _1,
                    bind(
                        &cluster_type::sources_count,
                        _2
                    )
                )
            );
        }

    template<typename Cluster, typename FindClusterPolicy>
        template<typename R>
        void fast_accumulator<Cluster,FindClusterPolicy
        >::push_back_cluster(const R& center){
            clusters_.push_back(cluster_type(
                center,
                active_bandwidth(),
                active_max_radius(),
                truncation_degree_policy_
            ));
        }

    template<typename Cluster, typename FindClusterPolicy>
        template<typename R0,typename R1>
        bool fast_accumulator<Cluster,FindClusterPolicy>::operator()(
        const R0& source, const R1& weight){
            static bool is_collected = false;
            static active_range_type local_active_range = active_range();
            if(active_bandwidth()>(value_type)(0)){
                local_active_range = active_range();
                if(empty(local_active_range)){
                    push_back_cluster(source);
                }else{
                    typedef typename mpl::apply<FindClusterPolicy,
                        active_range_type>::type find_nc_type;

                    // TODO assigning local_active_range
                    // and creating a new find_nc on each call maybe costly
                    find_nc_type find_nc(local_active_range);
                    iter_type nn = find_nc(source);
                    is_collected = (*nn)(source,weight);
                    if(nn->source_radius_exceeds_max_radius()){
                        BOOST_ASSERT(!is_collected);
                        push_back_cluster(source);
                    }
                }
            }else{
                throw std::runtime_error("fast_accumulator::collect");
            }
            return is_collected;
        }

}}}
#endif

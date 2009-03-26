//////////////////////////////////////////////////////////////////////////////
// keyword.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_KEYWORD_HPP_ER_2009
#define BOOST_MATH_IFGT_KEYWORD_HPP_ER_2009
#include <boost/parameter/parameters.hpp>
#include <boost/parameter/keyword.hpp>
namespace boost{namespace math{namespace ifgt{
namespace tag{
    template<typename Id> struct bandwidth_{};
    template<typename Id> struct max_cluster_radius_{};
    template<typename Id> struct degree_{};
    template<typename Id> struct clusters_{};
    template<typename Id> struct center_{};
    template<typename Id> struct target_{};
    template<typename Id> struct range_out_{};
    template<typename Id> struct error_tolerance_{};
    template<typename Id> struct max_distance_source_target_{};
    template<typename Id> struct cutoff_radius_policy_{};
    template<typename Id> struct clusters_reserve_size_{};
    template<typename Id> struct contributions_reserve_size_{};
    template<typename Id> struct accumulator_{};
    template<typename Id> struct nadaraya_watson_check_tolerance_{};
    template<typename Id> struct divide_by_{};
}
namespace{
    template<typename Id = mpl::void_>
    struct kwd{

        static
            ::boost::parameter::keyword<tag::bandwidth_<Id> >& bandwidth;
        static
            ::boost::parameter::keyword<tag::max_cluster_radius_<Id> >&
                max_cluster_radius;
        static
            ::boost::parameter::keyword<tag::degree_<Id> >& degree;
        static
            ::boost::parameter::keyword<tag::clusters_<Id> >& clusters;
        static
            ::boost::parameter::keyword<tag::center_<Id> >& center;
        static
            ::boost::parameter::keyword<tag::target_<Id> >& target;
        static
            ::boost::parameter::keyword<tag::range_out_<Id> >& range_out;
        static
            ::boost::parameter::keyword<tag::error_tolerance_<Id> >&
                                                    error_tolerance;
        static
            ::boost::parameter::keyword<
                tag::max_distance_source_target_<Id> >&
             max_distance_source_target;
        static
            ::boost::parameter::keyword<
                tag::cutoff_radius_policy_<Id> >&
             cutoff_radius_policy;
        static
            ::boost::parameter::keyword<tag::clusters_reserve_size_<Id> >&
             clusters_reserve_size;
        static
            ::boost::parameter::keyword<
                tag::contributions_reserve_size_<Id> >&
             contributions_reserve_size;
        static
            ::boost::parameter::keyword<tag::divide_by_<Id> >&
             divide_by;
        static
            ::boost::parameter::keyword<tag::accumulator_<Id> >&
             accumulator;

        static
            ::boost::parameter::keyword<
                tag::nadaraya_watson_check_tolerance_<Id> >&
             nadaraya_watson_check_tolerance;

    };

    template<typename Id>
            ::boost::parameter::keyword<tag::bandwidth_<Id> >&
            kwd<Id>::bandwidth
                =  ::boost::parameter::keyword<tag::bandwidth_<Id> >::get();
    template<typename Id>
            ::boost::parameter::keyword<tag::max_cluster_radius_<Id> >&
                kwd<Id>::max_cluster_radius
                =
                    ::boost::parameter::keyword<
                        tag::max_cluster_radius_<Id> >::get();
    template<typename Id>
            ::boost::parameter::keyword<tag::degree_<Id> >&
            kwd<Id>::degree
                =  ::boost::parameter::keyword<tag::degree_<Id> >::get();
    template<typename Id>
            ::boost::parameter::keyword<tag::clusters_<Id> >&
            kwd<Id>::clusters
                =  ::boost::parameter::keyword<tag::clusters_<Id> >::get();
    template<typename Id>
            ::boost::parameter::keyword<tag::center_<Id> >&
            kwd<Id>::center
                =  ::boost::parameter::keyword<tag::center_<Id> >::get();
    template<typename Id>
            ::boost::parameter::keyword<tag::target_<Id> >&
            kwd<Id>::target
                =  ::boost::parameter::keyword<tag::target_<Id> >::get();
    template<typename Id>
            ::boost::parameter::keyword<tag::range_out_<Id> >&
            kwd<Id>::range_out
                =  ::boost::parameter::keyword<tag::range_out_<Id> >::get();
    template<typename Id>
            ::boost::parameter::keyword<tag::error_tolerance_<Id> >&
            kwd<Id>::error_tolerance
                =  ::boost::parameter::keyword<tag::error_tolerance_<Id>
                    >::get();
    template<typename Id>
            ::boost::parameter::keyword<tag::max_distance_source_target_<Id>
                >&
             kwd<Id>::max_distance_source_target
              =
            ::boost::parameter::keyword<tag::max_distance_source_target_<Id>
            >::get();
    template<typename Id>
            ::boost::parameter::keyword<tag::cutoff_radius_policy_<Id> >&
             kwd<Id>::cutoff_radius_policy
              = ::boost::parameter::keyword<tag::cutoff_radius_policy_<Id>
              >::get();
    template<typename Id>
            ::boost::parameter::keyword<tag::clusters_reserve_size_<Id> >&
             kwd<Id>::clusters_reserve_size
              = ::boost::parameter::keyword<tag::clusters_reserve_size_<Id>
              >::get();
    template<typename Id>
            ::boost::parameter::keyword<tag::contributions_reserve_size_<Id>
            >&
             kwd<Id>::contributions_reserve_size
              =
            ::boost::parameter::keyword<tag::contributions_reserve_size_<Id>
            >::get();
    template<typename Id>
            ::boost::parameter::keyword<tag::divide_by_<Id> >&
             kwd<Id>::divide_by
              = ::boost::parameter::keyword<
                tag::divide_by_<Id> >::get();
    template<typename Id>
            ::boost::parameter::keyword<tag::accumulator_<Id> >&
             kwd<Id>::accumulator
              = ::boost::parameter::keyword<tag::accumulator_<Id> >::get();

    template<typename Id>
            ::boost::parameter::keyword<
                tag::nadaraya_watson_check_tolerance_<Id> >&
             kwd<Id>::nadaraya_watson_check_tolerance
              = ::boost::parameter::keyword<
                tag::nadaraya_watson_check_tolerance_<Id> >::get();
}//anonymous
}//ifgt
}//math
}//boost
#endif

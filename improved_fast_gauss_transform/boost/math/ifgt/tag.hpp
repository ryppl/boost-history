//////////////////////////////////////////////////////////////////////////////
// ifgt/tag.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_TAG_HPP_ER_2009
#define BOOST_MATH_IFGT_TAG_HPP_ER_2009
#include <boost/parameter/parameters.hpp>
#include <boost/parameter/keyword.hpp>
namespace boost{namespace math{namespace ifgt{
namespace{
    struct bandwidth_{};
    struct max_cluster_radius_{};
    struct degree_{};
    struct clusters_{};
    struct center_{};
    struct target_{};
    struct range_out_{};
    struct error_tolerance_{};
    struct max_distance_source_target_{};
    struct cutoff_radius_policy_{};
    struct clusters_reserve_size_{};
    struct contributions_reserve_size_{};
    struct accumulator_{};
    struct nadaraya_watson_check_tolerance_{};
    struct divide_by_{};
}
namespace tag{

        static
            ::boost::parameter::keyword<bandwidth_>& bandwidth
                =  ::boost::parameter::keyword<bandwidth_>::get();
        static
            ::boost::parameter::keyword<max_cluster_radius_>&
                max_cluster_radius
                =
                    ::boost::parameter::keyword<max_cluster_radius_>::get();
        static
            ::boost::parameter::keyword<degree_>& degree
                =  ::boost::parameter::keyword<degree_>::get();
        static
            ::boost::parameter::keyword<clusters_>& clusters
                =  ::boost::parameter::keyword<clusters_>::get();
        static
            ::boost::parameter::keyword<center_>& center
                =  ::boost::parameter::keyword<center_>::get();
        static
            ::boost::parameter::keyword<target_>& target
                =  ::boost::parameter::keyword<target_>::get();
        static
            ::boost::parameter::keyword<range_out_>& range_out
                =  ::boost::parameter::keyword<range_out_>::get();
        static
            ::boost::parameter::keyword<error_tolerance_>& error_tolerance
                =  ::boost::parameter::keyword<error_tolerance_>::get();
        static
            ::boost::parameter::keyword<max_distance_source_target_>&
             max_distance_source_target
              =
            ::boost::parameter::keyword<max_distance_source_target_>::get();
        static
            ::boost::parameter::keyword<cutoff_radius_policy_>&
             cutoff_radius_policy
              = ::boost::parameter::keyword<cutoff_radius_policy_>::get();
        static
            ::boost::parameter::keyword<clusters_reserve_size_>&
             clusters_reserve_size
              = ::boost::parameter::keyword<clusters_reserve_size_>::get();
        static
            ::boost::parameter::keyword<contributions_reserve_size_>&
             contributions_reserve_size
              =
            ::boost::parameter::keyword<contributions_reserve_size_>::get();
        static
            ::boost::parameter::keyword<divide_by_>&
             divide_by
              = ::boost::parameter::keyword<
                divide_by_>::get();
        static
            ::boost::parameter::keyword<accumulator_>&
             accumulator
              = ::boost::parameter::keyword<accumulator_>::get();

        static
            ::boost::parameter::keyword<nadaraya_watson_check_tolerance_>&
             nadaraya_watson_check_tolerance
              = ::boost::parameter::keyword<
                nadaraya_watson_check_tolerance_>::get();
// All this works as is, but
// defining instead a keyword within say struct my_struct{}
// has caused compile error "multiple definitions of tag::my_struct::keyword".
// I have used defs within structs before w/o pbs (though templates) so why?
}
}}}
#endif

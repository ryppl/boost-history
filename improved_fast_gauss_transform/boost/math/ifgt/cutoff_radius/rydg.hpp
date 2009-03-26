//////////////////////////////////////////////////////////////////////////////
// rydg.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_CUTOFF_RADIUS_RYDG_HPP_ER_2009
#define BOOST_MATH_IFGT_CUTOFF_RADIUS_RYDG_HPP_ER_2009
#include <cmath>
#include <boost/numeric/conversion/bounds.hpp>
#include <boost/math/ifgt/keyword.hpp>

namespace boost{
namespace math{
namespace ifgt{
namespace cutoff_radius{

    // Models CutoffRadiusPolicy
    // Raykar2006a stands for Raykar et al. See Section 4.1:
    // Automatically choosing the cut off radius for each cluster
    template<typename RealType>
    class rydg{
    public:
        typedef RealType    result_type;
        // TODO what for?
        typedef RealType    first_argument_type;
        typedef RealType    second_argument_type;
    private:
        static result_type default_max_distance_source_target(){
            static const result_type r
                = numeric::bounds<result_type>::highest();
            return r;
        }
    public:

        /// Suggestion: error_tolerance be in relation to the standard dev
        /// of weight, sigma. Say sigma/1e3.
        template<typename ArgPack>
        rydg(const ArgPack& args)
            :error_tol_(args[kwd<>::error_tolerance]),
            max_dist_source_target_(
                args[kwd<>::max_distance_source_target
                |default_max_distance_source_target()]){}

        rydg(
            result_type error_tol,
            result_type max_dist_source_target
        ):error_tol_(error_tol),
        max_dist_source_target_(max_dist_source_target){}

        // default copy/assignement should work

        /// epsilon (Raykar2006a)
        result_type error_tolerance()const{ return error_tol_; }
        /// R in Raykar2006a
        result_type max_distance_source_target()const{
            return max_dist_source_target_; }

        /// Returns r_y^k (Raykar2006a)
        template<typename Cluster>
        result_type operator()(const Cluster& cluster)const{
              return
                this->operator()(cluster.bandwidth(),cluster.max_radius());
        }

     private:
        /// Arguments: h and r_x^k (Raykar2006a)
        result_type operator()(
            result_type bandwidth,
            result_type max_radius
        )const{
            static result_type c
                = sqrt(log((result_type)(1)/error_tolerance()));

            result_type a = max_distance_source_target();
            result_type b = bandwidth * c;
            result_type d = (a<b)? a : b;

            return max_radius + d;
        }
        rydg();//dont implement
        result_type    error_tol_;
        result_type    max_dist_source_target_;
    };

}//cutoff_radius
}//ifgt
}//math
}//boost
#endif

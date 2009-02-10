//////////////////////////////////////////////////////////////////////////////
// ifgt/cluster_evalutor.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_CLUSTER_EVALUATOR_HPP_ER_2009
#define BOOST_MATH_IFGT_CLUSTER_EVALUATOR_HPP_ER_2009
#include <stdexcept>
#include <algorithm>
#include <boost/range.hpp>
#include <boost/bind.hpp>
#include <boost/mpl/assert.hpp>
#include <boost/mpl/apply.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/utility.hpp>
#include <boost/type_traits/remove_cv.hpp>
#include <boost/type_traits/remove_reference.hpp>
#include <boost/math/ifgt/tag.hpp>
#include <boost/utility/dont_care.hpp>
#include <boost/math/ifgt/coefficients_evaluator.hpp>
#include <boost/iterator/range_same.hpp>

namespace boost{ namespace math{namespace ifgt{

    // cluster_evaluator takes a target and a range_out in its constructor
    // remember that a cluster contains as many coefficients as
    // weight sequences. The job of cluster_evaluator
    // is to apply coefficients_evaluator to each
    // coefficients and *add* the results to range_out

    // \warning adds to range_out
    // \warning The cutoff_radius_policy is passed by reference for efficiency
    // R ForwarRange (that of the target)
    // apply<CutoffRadiusPolicy,X>::type
    template<
        typename R,
        typename RangeOut,
        typename CutoffRadiusPolicy>
    class cluster_evaluator{
     public:
        typedef typename range_value<R>::type       value_type;
        typedef RangeOut&                           ref_range_out_type;
     private:
        typedef typename mpl::apply<CutoffRadiusPolicy,value_type>::type
                                                    cutoff_radius_policy_type;
        typedef const cutoff_radius_policy_type&
                                               cref_cutoff_radius_policy_type;
     public:
        template<typename ArgPack>
        cluster_evaluator(const ArgPack& args)
            :range_out_(args[tag::range_out]),
            coeffs_evaluator_(args[tag::target]),
            cutoff_radius_policy_(args[tag::cutoff_radius_policy]){
            // Do not replace by cutoff_radius_policy_(args)
        }

        //TODO remove
        cluster_evaluator(
            const R& target,
            ref_range_out_type range_out,
            cref_cutoff_radius_policy_type cutoff_radius_policy)
            :range_out_(range_out),
            coeffs_evaluator_(target),
            cutoff_radius_policy_(cutoff_radius_policy){}

        cluster_evaluator(const cluster_evaluator& that)
            :range_out_(that.range_out_),
            coeffs_evaluator_(that.coeffs_evaluator_),
            cutoff_radius_policy_(that.cutoff_radius_policy_){}

        template<typename Cluster>
        void operator()(const Cluster& cluster){
            static range_same policy;
            return this->operator()(cluster, policy);
        }

        template<typename Cluster,typename SubsetRangePolicy>
        void operator()(const Cluster& cluster,
            const SubsetRangePolicy& subset_range_policy){
            typedef typename Cluster::coefficients_type        coeffs_type;
            typedef SubsetRangePolicy             subset_range_policy_type;
            //compile warning:
            //"comparison between signed and unsigned integer expressions|"
            //why? source_size() is also a size_type
            BOOST_ASSERT(
              coeffs_evaluator_.target_size() - size(cluster.center()) == 0 );

            value_type cutoff_radius = cutoff_radius_policy_(cluster);

            typedef typename
                Cluster::result_of_collection_coefficients_type  arg_type;

            typedef typename result_of<
                subset_range_policy_type(arg_type)>::type     subset_type;

            subset_type subset
             = subset_range_policy(cluster.collection_coefficients());

           BOOST_ASSERT(size(subset) ==size(range_out_));

            typename coeffs_type::result_type (coeffs_type::*f)() const =
                &coeffs_type::operator();

            std::transform(
                begin(subset),
                end(subset),
                begin(range_out_),
                begin(range_out_),
                bind(
                    std::plus<value_type>(),
                    bind(
                        ref(coeffs_evaluator_),
                        cutoff_radius,
                        cref(cluster.center()),
                        cluster.bandwidth(),
                        cluster.inverse_normalizing_constant(),
                        bind(
                            f,
                            _1
                        ),
                        bind(
                            &coeffs_type::max_degree,
                            _1
                        )
                    ),
                    _2
                )
            );
        }

     private:
        cluster_evaluator& operator=(const cluster_evaluator& that);
        cluster_evaluator();
        typedef coefficients_evaluator<R> coeffs_evaluator_type;
        ref_range_out_type                  range_out_;
        coeffs_evaluator_type               coeffs_evaluator_;
        cutoff_radius_policy_type           cutoff_radius_policy_;
    };
}}}
#endif

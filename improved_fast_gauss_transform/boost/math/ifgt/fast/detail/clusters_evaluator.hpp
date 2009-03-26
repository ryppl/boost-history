//////////////////////////////////////////////////////////////////////////////
// clusters_evaluator.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_CLUSTERS_EVALUATOR_HPP_ER_2009
#define BOOST_MATH_IFGT_CLUSTERS_EVALUATOR_HPP_ER_2009
#include <algorithm>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <boost/range.hpp>
#include <boost/math/ifgt/fast//detail/cluster_evaluator.hpp>
namespace boost{namespace math{namespace ifgt{

    // Adds to a range the result from evaluating each cluster
    template<
        typename RealType,
        typename CutoffRadiusPolicy
    >
    class clusters_evaluator{
     public:
        typedef RealType                                        value_type;
        typedef typename mpl::apply<CutoffRadiusPolicy,value_type>::type
                                                cluster_radius_policy_type;
        template<typename ArgPack>
        clusters_evaluator(const ArgPack& args):cluster_radius_policy_(args){}

        // make sure policy's default constructor is disabled
        // if necessary
        // so that compile error here
        clusters_evaluator(){}

        clusters_evaluator(const cluster_radius_policy_type&
            cluster_radius_policy)
                : cluster_radius_policy_(cluster_radius_policy){}
        clusters_evaluator(const clusters_evaluator& that)
            :cluster_radius_policy_(that.cluster_radius_policy_){}

        clusters_evaluator& operator=(const clusters_evaluator& that){
            if(&that!=this){
                cluster_radius_policy_ = that.cluster_radius_policy_;
            }
            return *this;
        }

        template<typename R0,typename R1,typename R2>
        void operator()(
            const R0& target,const R1& clusters,R2& range_out)const{
            static range_same policy;
            return this->operator()(
                target, clusters, range_out, policy
            );
        }

        //\warning adds to range_out
        template<typename R0,typename R1,typename R2,
            typename SubsetRangePolicy>
        void operator()(
            const R0& target,const R1& clusters,R2& range_out,
            const SubsetRangePolicy& subset_range_policy)const{
            typedef typename range_value<R0>::type      value0_type;
            typedef typename range_value<R1>::type      cluster_type;
            typedef typename cluster_type::value_type   value1_type;
            typedef typename range_value<R2>::type      value2_type;
            BOOST_MPL_ASSERT((is_same<value0_type,value_type>));
            BOOST_MPL_ASSERT((is_same<value1_type,value_type>));
            BOOST_MPL_ASSERT((is_same<value2_type,value_type>));
            typedef cluster_evaluator<R0,R2,CutoffRadiusPolicy> eval_type;

            eval_type eval(target, range_out, cluster_radius_policy_);

            for_each(
                begin(clusters),
                end(clusters),
                bind<void>(
                    ref(eval),//not cref because eval modifies range_out
                    _1,
                    cref(subset_range_policy)
                )
            );
        };

     private:
        cluster_radius_policy_type cluster_radius_policy_;
    };

}}}
#endif

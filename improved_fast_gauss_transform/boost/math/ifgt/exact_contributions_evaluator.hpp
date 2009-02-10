//////////////////////////////////////////////////////////////////////////////
// ifgt/exact_contributions_evaluator.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_EXACT_CONTRIBUTIONS_EVALUATOR_HPP_ER_2009
#define BOOST_MATH_IFGT_EXACT_CONTRIBUTIONS_EVALUATOR_HPP_ER_2009
#include <algorithm>
#include <boost/range.hpp>
#include <boost/ref.hpp>
#include <boost/range/value_type.hpp>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/math/ifgt/exact_contribution.hpp>

namespace boost{namespace math{namespace ifgt{

    //\warning adds to range_out
    template<typename R0>
    class exact_contributions_evaluator{
        typedef R0                                         contributions_type;
        typedef typename range_value<R0>::type             contribution_type;
     public:
        exact_contributions_evaluator(
            const contributions_type& contributions
        ):contributions_(contributions){}

        exact_contributions_evaluator(
            const exact_contributions_evaluator& that)
            :contributions_(that.contributions_){}

        template<typename R1,typename R2>
        void operator()(const R1& target,R2& range_out)const{
            typedef range_same policy_type;
            static policy_type policy;
            return this->operator()(target,range_out,policy);
        }

        // \warning *adds* to range_out
        template<typename R1,typename R2,typename SubsetRangePolicy>
        void operator()(const R1& target,R2& range_out,
            const SubsetRangePolicy& policy)const{
            void (contribution_type::*f) (
                const R1&,R2&,const SubsetRangePolicy&) const
                = &contribution_type::evaluate;
            for_each(
                begin(contributions_),
                end(contributions_),
                bind(
                    f,
                    _1,
                    cref(target),
                    ref(range_out),
                    cref(policy)
                )
            );
        }

     private:
        exact_contributions_evaluator&
        operator=(const exact_contributions_evaluator& that);
        exact_contributions_evaluator();
        const contributions_type&   contributions_;
    };

    template<typename R0>
    exact_contributions_evaluator<R0>
    make_exact_contributions_evaluator(
        const R0& contributions
    ){
        return exact_contributions_evaluator<R0>(contributions);
    }

}}}
#endif

//////////////////////////////////////////////////////////////////////////////
// ifgt/fast_evaluator.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_FAST_EVALUATOR_HPP_ER_2009
#define BOOST_MATH_IFGT_FAST_EVALUATOR_HPP_ER_2009
#include <stdexcept>
#include <boost/math/ifgt/cutoff_radius_none.hpp>
#include <boost/math/ifgt/clusters_evaluator.hpp>
#include <boost/math/ifgt/evaluator_common.hpp>
#include <boost/math/ifgt/evaluator_base.hpp>
namespace boost{namespace math{namespace ifgt{

    template<
        typename FastAccumulator,
        typename CutoffRadiusPolicy = cutoff_radius_none<mpl::_1>
    >
    class fast_evaluator : public
        evaluator_base<
            evaluator_common<FastAccumulator>,
            fast_evaluator<
                FastAccumulator,
                CutoffRadiusPolicy>
        >{

        typedef fast_evaluator<FastAccumulator,CutoffRadiusPolicy> self_type;
        typedef evaluator_common<FastAccumulator>                 c_type;
        typedef evaluator_base<c_type,self_type>                  base_type;
//        typedef typename base_type::value_type                    value_type;
        typedef clusters_evaluator<typename fast_evaluator::value_type,
            CutoffRadiusPolicy> eval_type;

        public:

        template<typename ArgPack>
        fast_evaluator(const ArgPack& arg) : base_type(arg),eval_(arg){}

        fast_evaluator(const fast_evaluator& that)
        :base_type(that),eval_(that.eval_){}

        //TODO find a way to privatize
        template<typename R0,typename R1,typename SubsetRangePolicy>
        void call_impl(const R0& target, R1& range_out,
            const SubsetRangePolicy& policy)const{
            return eval_(
                target,this->accumulator().clusters(),range_out,policy);
        }

        private:
        fast_evaluator& operator=(const fast_evaluator& that);
        fast_evaluator();
        eval_type eval_;
    };

}}}
#endif

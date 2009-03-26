//////////////////////////////////////////////////////////////////////////////
// evaluator.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_FAST_EVALUATOR_HPP_ER_2009
#define BOOST_MATH_IFGT_FAST_EVALUATOR_HPP_ER_2009
#include <stdexcept>
#include <boost/math/ifgt/cutoff_radius/none.hpp>
#include <boost/math/ifgt/fast/detail/clusters_evaluator.hpp>
#include <boost/math/ifgt/detail/evaluator_common.hpp>
#include <boost/math/ifgt/crtp/evaluator.hpp>
namespace boost{
namespace math{
namespace ifgt{
namespace fast{

        //evaluator_base<
        //    evaluator_common<FastAccumulator>,
        //    fast_evaluator<
        //        FastAccumulator,
        //        CutoffRadiusPolicy>
        //>

    template<
        typename FastAccumulator,
        typename CutoffRadiusPolicy = cutoff_radius::none<mpl::_1>
    >
    class evaluator :
        public evaluator_common<FastAccumulator>,
        public crtp::evaluator<
            evaluator<FastAccumulator,CutoffRadiusPolicy>
        >
        {
        typedef evaluator_common<FastAccumulator>               super_t;
        typedef clusters_evaluator<
            typename super_t::value_type,
            CutoffRadiusPolicy
        >                                                       eval_type;

        public:

        template<typename ArgPack>
        evaluator(const ArgPack& arg) : super_t(arg),eval_(arg){}

        evaluator(const evaluator& that)
        :super_t(that),eval_(that.eval_){}

        //TODO find a way to privatize
        template<typename R0,typename R1,typename SubsetRangePolicy>
        void call_impl(const R0& target, R1& range_out,
            const SubsetRangePolicy& policy)const{
            return eval_(
                target,this->accumulator().clusters(),range_out,policy);
        }

        private:
        evaluator& operator=(const evaluator& that);
        evaluator();
        eval_type eval_;
    };

}//fast
}//ifgt
}//math
}//boost
#endif

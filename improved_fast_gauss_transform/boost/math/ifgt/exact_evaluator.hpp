//////////////////////////////////////////////////////////////////////////////
// ifgt/exact_evaluator.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_EXACT_EVALUATOR_HPP_ER_2009
#define BOOST_MATH_IFGT_EXACT_EVALUATOR_HPP_ER_2009
#include <boost/math/ifgt/evaluator_common.hpp>
#include <boost/math/ifgt/evaluator_base.hpp>
#include <boost/math/ifgt/exact_contributions_evaluator.hpp>
namespace boost{namespace math{namespace ifgt{

template<typename ExactAccumulator>
class exact_evaluator : public evaluator_base<
        evaluator_common<ExactAccumulator>,
        exact_evaluator<ExactAccumulator>
    >{
    typedef exact_evaluator<ExactAccumulator>                   self_type;
    typedef evaluator_common<ExactAccumulator>                  c_type;
    typedef evaluator_base<c_type,self_type>                    base_type;
    //typedef typename base_type::value_type                      value_type;
    public:

    template<typename ArgPack>
    exact_evaluator(const ArgPack& arg) :
        base_type(arg){}

    exact_evaluator(const exact_evaluator& that) :
     base_type(that){}

    // TODO privatize
    template<typename R0,typename R1,typename SubsetRangePolicy>
    void call_impl(const R0& target,R1& range_out,
        const SubsetRangePolicy& policy)const{
        make_exact_contributions_evaluator(
            (this->accumulator()).contributions()
        )(target, range_out,policy);
    }

    private:
    exact_evaluator& operator=(const exact_evaluator& that);
    exact_evaluator();
};

}}}
#endif

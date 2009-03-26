//////////////////////////////////////////////////////////////////////////////
// evaluator.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_EXACT_EVALUATOR_HPP_ER_2009
#define BOOST_MATH_IFGT_EXACT_EVALUATOR_HPP_ER_2009
#include <boost/math/ifgt/detail/evaluator_common.hpp>
#include <boost/math/ifgt/crtp/evaluator.hpp>
#include <boost/math/ifgt/exact/detail/contributions_evaluator.hpp>
namespace boost{
namespace math{
namespace ifgt{
namespace exact{
//template<typename ExactAccumulator>
//class exact_evaluator : public evaluator_base<
//        evaluator_common<ExactAccumulator>,
//        exact_evaluator<ExactAccumulator>
//    >{
template<typename ExactAccumulator>
class evaluator :
    public evaluator_common<ExactAccumulator>,
    public crtp::evaluator<evaluator<ExactAccumulator> >
    {

    typedef evaluator_common<ExactAccumulator>                  super_t;
    public:

    template<typename ArgPack>
    evaluator(const ArgPack& arg) :
        super_t(arg){}

    evaluator(const evaluator& that) :
     super_t(that){}

    // TODO privatize
    template<typename R0,typename R1,typename SubsetRangePolicy>
    void call_impl(const R0& target,R1& range_out,
        const SubsetRangePolicy& policy)const{
        make_contributions_evaluator(
            (this->accumulator()).contributions()
        )(target, range_out,policy);
    }

    private:
    evaluator& operator=(const evaluator& that);
    evaluator();
};

}//exact
}//ifgt
}//math
}//boost
#endif

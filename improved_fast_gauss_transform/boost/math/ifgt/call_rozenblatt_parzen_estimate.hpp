//////////////////////////////////////////////////////////////////////////////
// ifgt/call_rozenblatt_parzen_estimate.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_CALL_ROZENBLATT_PARZEN_HPP_ER_2009
#define BOOST_MATH_IFGT_CALL_ROZENBLATT_PARZEN_HPP_ER_2009
#include <stdexcept>
#include <boost/math/ifgt/evaluator_base.hpp>
namespace boost{namespace math{namespace ifgt{

template<typename Evaluator>
struct rozenblatt_parzen_estimate{
    typedef typename Evaluator::value_type value_type;
    public:
    template<typename Common>
    rozenblatt_parzen_estimate(const evaluator_base<Common,Evaluator>& e)
    :eval_(static_cast<const Evaluator&>(e)){}

    rozenblatt_parzen_estimate(
        const rozenblatt_parzen_estimate& that)
        : eval_(that.eval_){}

    template<typename R0>
    value_type operator()(const R0& target)const{
        return eval_.rozenblatt_parzen_estimate(target);
    };

    private:
    rozenblatt_parzen_estimate& operator=(
        const rozenblatt_parzen_estimate& that);
    const Evaluator& eval_;
};

template<typename C,typename E>
rozenblatt_parzen_estimate<E>
make_rozenblatt_parzen_estimate(const evaluator_base<C,E>& e){
    typedef rozenblatt_parzen_estimate<E> result_type;
    return result_type(e);
}

}}}
#endif

//////////////////////////////////////////////////////////////////////////////
// ifgt/call_nadaraya_watson_estimate.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_CALL_NADARAYA_WATSON_ESTIMATE_HPP_ER_2009
#define BOOST_MATH_IFGT_CALL_NADARAYA_WATSON_ESTIMATE_HPP_ER_2009
#include <stdexcept>
#include <boost/mpl/size_t.hpp>
#include <boost/math/ifgt/evaluator_base.hpp>
namespace boost{namespace math{namespace ifgt{

template<typename Evaluator>
class nadaraya_watson_estimate{
    public:
       typedef Evaluator                                    evaluator_type;
       struct stride_traits{
        typedef mpl::size_t<evaluator_type::source_size>     stride0_type;
        typedef mpl::size_t<evaluator_type::weight_size-1>   stride1_type;
       };
       typedef typename evaluator_type::value_type          value_type;

       template<typename Common>
       nadaraya_watson_estimate(const evaluator_base<Common,Evaluator>& e)
       :eval_(static_cast<const Evaluator&>(e)){}
       nadaraya_watson_estimate(const nadaraya_watson_estimate& that)
        : eval_(that.eval){}

       template<typename R0,typename R1>
       void operator()(const R0& targets,R1 ranges_out)const{
            eval_.nadaraya_watson_estimate(targets,ranges_out);
       };

    private:
       nadaraya_watson_estimate& operator=(
        const nadaraya_watson_estimate& that);
       const Evaluator& eval_;
};

template<typename C,typename E>
nadaraya_watson_estimate<E>
make_nadaraya_watson_estimate(const evaluator_base<C,E>& e){
    typedef nadaraya_watson_estimate<E> result_type;
    return result_type(e);
}

}}}
#endif

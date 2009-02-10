//////////////////////////////////////////////////////////////////////////////
// ifgt/evaluator_common.hpp
//  (C) Copyright 2009 Erwann Rogard
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_MATH_IFGT_EVALUATOR_COMMON_HPP_ER_2009
#define BOOST_MATH_IFGT_EVALUATOR_COMMON_HPP_ER_2009
#include <boost/math/ifgt/tag.hpp>
namespace boost{namespace math{namespace ifgt{

template<typename Accumulator>
class evaluator_common{
    public:
    typedef Accumulator                                 accumulator_type;
    typedef typename accumulator_type::value_type       value_type;
    typedef typename accumulator_type::sources_count_type
                                                        sources_count_type;
    typedef const accumulator_type&             result_of_accumulator_type;

    static const std::size_t source_size = accumulator_type::source_size;
    static const std::size_t weight_size = accumulator_type::weight_size;

    template<typename ArgPack>
    evaluator_common(const ArgPack& arg):acc_(arg[tag::accumulator]){}

    evaluator_common(const evaluator_common& that):acc_(that.acc_){}

    sources_count_type sources_count()const{
        return this->acc_.sources_count(); }
    result_of_accumulator_type accumulator()const{ return this->acc_; }
    private:
        evaluator_common& operator=(const evaluator_common& that);
        evaluator_common();
        const accumulator_type& acc_;
};

}}}
#endif

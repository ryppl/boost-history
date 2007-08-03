// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_PHOENIX_COMPONENT_PRODUCER_ACCUMULATOR_HPP
#define BOOST_DATAFLOW_PHOENIX_COMPONENT_PRODUCER_ACCUMULATOR_HPP

#include <boost/fusion/algorithm/iteration/accumulate.hpp>

#include <functional>

namespace boost { namespace phoenix {

namespace detail {

    template<typename Op>
    struct function_op : public Op
    {
        template<typename Function>
        typename Op::result_type
            operator()(Function f, typename Op::second_argument_type operand)
        {
            return Op::operator()(f(), operand);
        }
    };

}

namespace extension
{
    template<typename T>
    struct identity_element;
    
    template<typename T>
    struct identity_element<std::plus<T> >
    {
        typedef T type;
        T operator()()
        {
            return 0;
        }
    };
}

template<typename AccumulateOp, typename Operations>
class producer_accumulator : public Operations // should be a fusion sequence
{
public:
//    typedef boost::dataflow::fusion_group_consumer consumer_category;
    
    producer_accumulator(const Operations &op) : Operations(op) {}
    producer_accumulator() {}
    
    typename extension::identity_element<AccumulateOp>::type operator()()
    {
        return boost::fusion::accumulate(*this,
            extension::identity_element<AccumulateOp>()(),
            detail::function_op<AccumulateOp>());
    }
};

} } // namespace boost::phoenix

#endif // BOOST_DATAFLOW_PHOENIX_COMPONENT_PRODUCER_ACCUMULATOR_HPP

// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_PHOENIX_COMPONENT_PRODUCER_WRAPPER_HPP
#define BOOST_DATAFLOW_PHOENIX_COMPONENT_PRODUCER_WRAPPER_HPP

#include <boost/dataflow/phoenix/support.hpp>

namespace boost { namespace phoenix {
    
template<typename T, typename Function> 
class producer_wrapper
{
public:
    typedef boost::dataflow::phoenix_producer producer_category;
    typedef T produced_type;
    typedef T value_type;
    
    producer_wrapper(T &value) : value(value) {}
    
    void operator()()
    {
        value = Function()();
    }
    
    template<typename T1>
    void operator()(T1 &t1)
    {
        value = Function()(t1);
    }
    
    template<typename T1, typename T2>
    void operator()(T1 &t1, T2 &t2)
    {
            value = Function()(t1, t2);
    }
    
    template<typename T1, typename T2, typename T3>
    void operator()(T1 &t1, T2 &t2, T3 &t3)
    {
            value = Function()(t1, t2, t3);
    }

    T &value;
};

template<typename T, typename F>
producer_wrapper<T, F> wrap_producer(T &t, const F &f)
{
    return producer_wrapper<T, F>(t);
}

template<typename F, typename T>
producer_wrapper<T, F> wrap_producer(T &t)
{
    return producer_wrapper<T, F>(t);
}

} } // namespace boost::phoenix

#endif // BOOST_DATAFLOW_PHOENIX_COMPONENT_PRODUCER_WRAPPER_HPP


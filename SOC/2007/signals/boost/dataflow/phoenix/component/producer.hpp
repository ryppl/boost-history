// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_PHOENIX_COMPONENT_PRODUCER_HPP
#define BOOST_DATAFLOW_PHOENIX_COMPONENT_PRODUCER_HPP

#include <boost/dataflow/phoenix/support.hpp>

namespace boost { namespace phoenix {

    template<typename T, typename Function> 
    class producer : public Function
    {
    public:
        typedef boost::dataflow::phoenix_producer producer_category;
        typedef T produced_type;
        typedef T value_type;
        
        producer(const Function &f) : Function(f) {}
        producer() {}

        void operator()()
        {
            value = Function::operator()();
        }

        template<typename T1>
        void operator()(T1 &t1)
        {
            value = Function::operator()(t1);
        }

        T value;
    };
    
    template<typename T, typename Function>
    const producer<T, Function> *operator&(const producer<T, Function> &op2)
    {
        return boost::addressof(op2);
    }

    template<typename T, typename Function>
    producer<T, Function> *operator&(producer<T, Function> &op2)
    {
        return boost::addressof(op2);
    }

    template<typename T, typename Function>
    volatile producer<T, Function> *operator&(volatile producer<T, Function> &op2)
    {
        return boost::addressof(op2);
    }

    template<typename T, typename Function>
    const volatile producer<T, Function> *operator&(const volatile producer<T, Function> &op2)
    {
        return boost::addressof(op2);
    }
    
    template<typename T, typename Function> 
    class static_producer
    {
    public:
        typedef boost::dataflow::phoenix_producer producer_category;
        typedef T produced_type;
        typedef T value_type;
        
        void operator()()
        {
            value = Function::operator()();
        }
        
        template<typename T1>
        void operator()(T1 &t1)
        {
            value = Function()(t1);
        }
        
        T value;
    };
    

} } // namespace boost::phoenix

#endif // BOOST_DATAFLOW_PHOENIX_COMPONENT_PRODUCER_HPP

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
        
        producer(const Function &f) : Function(f) {}
        producer() {}

        void operator()()
        {
            value = Function::operator()();
        }

        T value;
    };
    
    template<typename T, typename Function> 
    class producer2
    {
    public:
        typedef boost::dataflow::phoenix_producer producer_category;
        typedef T produced_type;
        
        void operator()()
        {
            value = Function()();
        }
        
        template<typename T1>
        void operator()(T1 &t1)
        {
            value = Function()(t1);
        }
        
        T value;
        
        operator T&(){return value;}
    };

} } // namespace boost::phoenix

#endif // BOOST_DATAFLOW_PHOENIX_COMPONENT_PRODUCER_HPP

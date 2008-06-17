// Copyright 2007 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//[ signal_function_connect

#include <boost/dataflow/signals/support.hpp>
#include <iostream>
#include <string>

void consumer_function(const std::string &data)
{
    std::cout << data << std::endl;
}

void signal_function_example()
{
    boost::signal<void(const std::string &)> producer;
    boost::function<void(const std::string &)> consumer(consumer_function);
    
    connect(producer, consumer); // make a connection between the two.
    producer("Hello World"); // signal goes to the function.
}

//]

//[ signal_mem_fn_connect

#include <boost/dataflow/utility/bind_mem_fn.hpp>

class consumer_class
{
public:
    void consumer_mem_fn(const std::string &data)
    {
        std::cout << data << std::endl;
    }
};

void signal_mem_fn_example()
{
    using boost::dataflow::utility::bind_mem_fn;
    
    boost::signal<void(const std::string &)> producer;
    consumer_class consumer;

    // make a connection between the producer and consumer.
    connect(producer, bind_mem_fn(&consumer_class::consumer_mem_fn, consumer));
    producer("Hello World"); // signal goes to the member function.
}

//]

//[ component_component_example

#include <boost/dataflow/signals/component/filter.hpp>

using namespace boost;

// Inheriting from the filter class gives us a default output signal of
// a specified signature.
class producer_component
    : public signals::filter<producer_component, void(const std::string &)>
{
public:
    void invoke()
    {
        // out is the default output signal.
        out("Hello World");
    }
};

// Inheriting from signals::filter or signals::consumer allows us to implement
// signal consumer ports as overloads of operator() - later you will see that
// that allows us to access them more easily.
class consumer_component
    : public signals::consumer<consumer_component>
{
public:
    // This is our signal consumer.
    void operator()(const std::string &data)
    {
        std::cout << data << std::endl;
    }
};

void component_component_example()
{
    producer_component producer;
    consumer_component consumer;
    
    // Because we inherited from filter/consumer, connecting is easy.
    connect(producer, consumer);
    producer.invoke(); // producer sends "Hello World" to consumer.
};

//]

//[ component_component_component_example

#include <boost/dataflow/signals/connection/operators.hpp>

class filter_component
    : public signals::filter<filter_component, void(const std::string &)>
{
public:
    // This is our signal consumer.  It will also produce a signal when called.
    void operator()(const std::string &data)
    {
        out(data + "!");
    }
};

void component_component_component_example()
{
    producer_component producer;
    filter_component filter;
    consumer_component consumer;
    
    // The following is equivalent to:
    // connect(producer, filter);
    // connect(filter, consumer);
    producer >>= filter >>= consumer;
    // producer sends "Hello World" to filter, filter sends "Hello World!".
    producer.invoke();
}

//]

int main()
{
    signal_function_example();
    signal_mem_fn_example();
    component_component_example();
    component_component_component_example();
    return 0;
}

//[ threadpool_component_example_implementation

// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include <boost/future/future.hpp>
#include <boost/fusion/include/fused.hpp>
#include <boost/fusion/include/join.hpp>
#include <boost/fusion/include/make_vector.hpp>

#define signalslib signals
#define signals signals

#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/component/function.hpp>
#include <boost/dataflow/signals/connection/operators.hpp>
#include <boost/dataflow/utility/bind_mem_fn.hpp>
#include <boost/dataflow/utility/bind_functor.hpp>

#include <boost/tp/info.hpp>
#include "boost/tp/fifo.hpp"
#include "boost/tp/lazy.hpp"
#include "boost/tp/pool.hpp"
#include "boost/tp/poolsize.hpp"
#include "boost/tp/bounded_channel.hpp"
#include "boost/tp/priority.hpp"


namespace tp = boost::tp;

template<typename Threadpool, typename Component>
class async_component;

namespace detail {

    // async_component_impl_base will contain the implementation details
    // shared between the priority and non-priority versions    
    template<typename Threadpool, typename Component>
    class async_component_impl_base
    // filter_base is like filter, but it doesn't come with it's own signal.
    // instead, the derived class must provide a default_signal member function
    // that refers to the default signal.  We will use this to return the
    // default signal of the underlying component.
         : public boost::signals::filter_base<async_component<Threadpool, Component>, typename Component::signal_type >
    {
    public:
        typedef typename Component::signature_type signature_type;
        typedef typename Component::signal_type signal_type;
        typedef void result_type;
        
        // the constructors will just initialize the threadpool reference,
        // the underlying component, and m_component_function
        template<typename T0>
        async_component_impl_base(Threadpool &threadpool, const T0 &t0)
            : m_component(t0), m_threadpool(threadpool)
        {
            init_m_component_function();
        }

        template<typename T0, typename T1>
        async_component_impl_base(Threadpool &threadpool, const T0 &t0, const T1 &t1)
            : m_component(t0, t1), m_threadpool(threadpool)
        {
            init_m_component_function();
        }
        
        // with the following, anything that connects to the async_component's default
        // signal will actually connect to the default signal of the underlying
        // component
        typename Component::signal_type &default_signal()
        {
            namespace dataflow = boost::dataflow;
            return dataflow::get_default_port<
                    dataflow::args::left,
                    dataflow::signals::connect_mechanism, 
                    dataflow::signals::tag
                > (m_component);
        }

    protected:
        Component m_component;
        Threadpool &m_threadpool;
        boost::function<signature_type> m_component_function;
        
    private:
        // record the appropriate operator() overload of Component into m_component_function,
        // so we can submit it as a task later
        void init_m_component_function()
        {
            typedef typename boost::dataflow::utility::slot_type<signature_type, Component>::type mem_fn_type;

            m_component_function = boost::dataflow::utility::bind_mem_fn<mem_fn_type, Component>
                (static_cast<mem_fn_type>(&Component::operator()), m_component);
        }
    };
    
    template<typename Threadpool, typename Component, typename Enable=void>
    class async_component_impl;
    
    // the non-priority version (for non-priority thread pools)
    template<typename Threadpool, typename Component>
    class async_component_impl<Threadpool, Component,
        typename boost::disable_if<tp::has_priority<Threadpool> >::type>
        : public async_component_impl_base<Threadpool, Component>
    {
    public:
        // forwarding constructors
        template<typename T0>
        async_component_impl(Threadpool &threadpool, const T0 &t0)
            : async_component_impl_base<Threadpool, Component>(threadpool, t0)
        {}

        template<typename T0, typename T1>
        async_component_impl(Threadpool &threadpool, const T0 &t0, const T1 &t1)
            : async_component_impl_base<Threadpool, Component>(threadpool, t0, t1)
        {}
        
        // when receiving the signal (which we get in a Fusion sequence)
        // we will submit the task
        template <class Seq>
        void operator()(const Seq &vec_par) const
        {
            // add the next function as a task in the pool
            std::cout << "adding task" << std::endl;
            // bind_functor is just a function object that calls bind
            // we create a fused version so we can call it with a fusion sequence
            boost::fusion::fused<boost::dataflow::utility::bind_functor> fused_bind;
            
            // submit the task (the first parameter to bind is the function,
            // and the rest are the bound function arguments).
            boost::tp::task< void > t(
                async_component_impl::m_threadpool.submit(
                    fused_bind(
                        boost::fusion::join(
                            boost::fusion::make_vector(async_component_impl::m_component_function),
                            vec_par
                    )   )
                    ));
        }

    };

    template<typename Threadpool, typename Component>
    class async_component_impl<Threadpool, Component,
        typename boost::enable_if<tp::has_priority<Threadpool> >::type>
        : public async_component_impl_base<Threadpool, Component>
    {
        typedef typename tp::priority_type<Threadpool>::type priority_type;
    public:
        // constructor stores the priority, and forwards the rest
        template<typename T0>
        async_component_impl(Threadpool &threadpool, const priority_type &priority, const T0 &t0)
            : async_component_impl_base<Threadpool, Component>(threadpool, t0)
            , m_priority(priority)
        {}
        
        // when receiving the signal (which we get in a Fusion sequence)
        // we will submit the task with the stored priority.
        template <class Seq>
        void operator()(const Seq &vec_par) const
        {
            // add the next function as a task in the pool
            std::cout << "adding task" << std::endl;
            // bind_functor is just a function object that calls bind
            // we create a fused version so we can call it with a fusion sequence
            boost::fusion::fused<boost::dataflow::utility::bind_functor> fused_bind;
            
            // submit the task (the first parameter to bind is the function,
            // and the rest are the bound function arguments).
            boost::tp::task< void > t(
                async_component_impl::m_threadpool.submit(
                    fused_bind(
                        boost::fusion::join(
                            boost::fusion::make_vector(async_component_impl::m_component_function),
                            vec_par
                    )   ),
                    m_priority
                    ));
        }
    private:
        priority_type m_priority;
    };

}


// our new async_component class - it will create a new task for the underlying
// component when it's operator() is called.
// unfused_inherited is an adaptor provided by Dataflow.Signals,
// which will allow the above implementation (which uses fused signals)
// to work with unfused signal signatures (fused / unfused in the Boost.Fusion
// sense)
template<typename Threadpool, typename Component>
class async_component : public boost::fusion::unfused_inherited<
    detail::async_component_impl<Threadpool,Component>,
    typename boost::function_types::parameter_types<typename Component::signature_type>::type >
{
    typedef boost::fusion::unfused_inherited<
        detail::async_component_impl<Threadpool,Component>,
        typename boost::function_types::parameter_types<typename Component::signature_type>::type>
        base_type;
public:
    // just forwarding constructors
    template<typename T0>
    async_component(Threadpool &threadpool, const T0 &t0)
        : base_type(threadpool, t0)
    {}
    template<typename T0, typename T1>
    async_component(Threadpool &threadpool, const T0 &t0, const T1 &t1)
        : base_type(threadpool, t0, t1)
    {}
};

// a function to submit the first task
template<typename Threadpool, typename Component>
void submit(Threadpool &threadpool, Component &component)
{
    tp::task< void > task(
        threadpool.submit(
            boost::bind(&Component::send, boost::ref(component))));
}

// a function to submit the first task with a priority
template<typename Threadpool, typename Component>
void submit(
    Threadpool &threadpool,
    const typename tp::priority_type<Threadpool>::type priority,
    Component &component)
{
    tp::task< void > task(
        threadpool.submit(
            boost::bind(&Component::send, boost::ref(component)), priority));
}

//]

//[ threadpool_component_example_use_inc_fn

// just an operation to work with
int inc_fn(int x)
{
    std::cout << "filter: " << x+1 << std::endl;
    return x+1;
}

//]

//[ threadpool_component_example_use_printer

// a component that displays text, waits an optional period, and then forwards the signal
class printer : public boost::signals::filter<printer, void()>
{
public:
    printer(const std::string &text, unsigned wait_milliseconds=0)
        : m_text(text), m_wait_milliseconds(wait_milliseconds)
    {}
    void operator()()
    {
        std::cout << m_text << std::endl;
        boost::this_thread::sleep(boost::posix_time::milliseconds(m_wait_milliseconds));
        out();
    }
private:
    std::string m_text;
    unsigned m_wait_milliseconds;
};

//]

int main( int argc, char *argv[])
{
//[ threadpool_component_example_use_simple
    // our Threadpool type - simple FIFO thread pool.
    typedef
        tp::pool<
            tp::fixed,
            tp::unbounded_channel< tp::fifo >
        > threadpool_type;
    
    // we limit the number of threads to 5
    threadpool_type pool(tp::max_poolsize(5));

    // the component types:
    // The source will store an int
    typedef boost::signals::storage<void(int)> source_type;
    // A filter will process the int
    typedef boost::signals::function<void(int), int(int)> filter_type;
    // This filter will process the int asynchronously
    typedef async_component<threadpool_type, filter_type> async_filter_type;

    // our components:
    // We start with a 0
    source_type source(0);
    
    // an assortment of synchronous and asynchronous increase-filters
    async_filter_type increase1(pool, inc_fn);
    filter_type increase2(inc_fn);
    filter_type increase3(inc_fn);
    async_filter_type increase4(pool, inc_fn);
    async_filter_type increase5(pool, inc_fn);
    
    // our network
    //  increase1 >>= increase2 >>= increase3 will be in its own thread
    //  increase4 will be in its own thread
    //  increase5 will be in its own thread
    source
        | (increase1 >>= increase2 >>= increase3)
        | (increase4 >>= increase5);
    // this was equivalent to:
    // connect(source, increase1);
    // connect(increase1, increase2);
    // connect(increase2, increase3);
    // connect(source, increase4);
    // connect(increase4, increase5);

    // submit the first task
    submit(pool, source);
//]
    // wait a little
    boost::this_thread::sleep(boost::posix_time::seconds(1));

    // --------------------------------------------------------------
    std::cout << "------------------------------------" << std::endl;

//[ threadpool_component_example_use_priority

    // a pool that uses priorities for tasks
    typedef 
        tp::pool<
            tp::fixed,
            tp::unbounded_channel< tp::priority< int > >
        > priority_threadpool_type;
    
    // we limit the pool to one thread
    priority_threadpool_type priority_pool(tp::max_poolsize(1));
    
    // components:
    typedef boost::signals::storage<void()> print_starter_type;
    typedef async_component<priority_threadpool_type, printer> async_printer_type;
    
    // this will start the execution
    print_starter_type print_starter;
    // this will print the text - we assign priorities so that the print
    // order is "Hello","<>","World!","!":
    async_printer_type print1(priority_pool, 3, "World");
    async_printer_type print2(priority_pool, 2, "<>");
    async_printer_type print3(priority_pool, 1, "Hello");
    async_printer_type print4(priority_pool, 4, "!");
    
    // our network:
    print_starter
        | print1
        | print2
        | print3
        | print4;
    // this was equivalent to:
    // connect(print_starter, print1);
    // connect(print_starter, print2);
    // connect(print_starter, print3);
    // connect(print_starter, print4);

        
    // submit the first task
    submit(priority_pool, 1, print_starter);
//]
    // wait a little
    boost::this_thread::sleep(boost::posix_time::seconds(1));
        
    // --------------------------------------------------------------
    std::cout << "------------------------------------" << std::endl;

//[ threadpool_component_example_use_cyclic

    // our components:
    typedef boost::signals::storage<void()> tick_starter_type;
    typedef async_component<threadpool_type, printer> ticker_type;
    
    // this will start the execution
    tick_starter_type tick_starter;
    // each component will print a line and wait a second before
    // forwarding the signal
    ticker_type
        ticker1(pool, "tick 1...", 1000),
        ticker2(pool, "tick 2...", 1000),
        ticker3(pool, "tick 3...", 1000);

    // our network (note the ticker1, ticker2, ticker3 cycle)
    tick_starter >>= ticker1 >>= ticker2 >>= ticker3 >>= ticker1;
    // this was equivalent to:
    // connect(tick_starter, ticker1);
    // connect(ticker1, ticker2);
    // connect(ticker2, ticker3);
    // connect(ticker3, ticker1);
    
    // submit the first task
    submit(pool, tick_starter);

    // wait a little
    boost::this_thread::sleep(boost::posix_time::seconds(5));

    // shutdown the pool (no more new tasks will be processed)
    pool.shutdown();
    
    //]
    return 0;
}


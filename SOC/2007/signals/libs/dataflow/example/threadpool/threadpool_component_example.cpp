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
#include <boost/thread_safe_signal.hpp>
//#include <boost/signals.hpp>

#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/component/function.hpp>
#include <boost/dataflow/signals/connection/operators.hpp>
#include <boost/dataflow/utility/bind_mem_fn.hpp>
#include <boost/dataflow/utility/bind_functor.hpp>

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

    template<typename Threadpool, typename Component>
    class async_component_impl
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
        
        template<typename T0>
        async_component_impl(Threadpool &threadpool, const T0 &t0)
            : m_component(t0), m_threadpool(threadpool)
        {
            // record the appropriate operator() overload of Component into m_component_function,
            // so we can submit it as a task later
            typedef typename boost::dataflow::utility::slot_type<signature_type, Component>::type mem_fn_type;

            m_component_function = boost::dataflow::utility::bind_mem_fn<mem_fn_type, Component>
                (static_cast<mem_fn_type>(&Component::operator()), m_component);
        }
        
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
                m_threadpool.submit(
                    fused_bind(
                        boost::fusion::join(
                            boost::fusion::make_vector(m_component_function),
                            vec_par
                    )   )
                    ));
        }
        // with this, anything that connects to the async_component's default
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

    private:
        Component m_component;
        Threadpool &m_threadpool;
        boost::function<signature_type> m_component_function;
    };
}


// our new async_component class - it will create a new task for the underlying
// component when it's operator() is called.
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
    template<typename T0>
    async_component(Threadpool &threadpool, const T0 &t0)
        : base_type(threadpool, t0)
    {}
};

// just an operation to work with
int inc_fn(int x)
{
    std::cout << "filter: " << x+1 << std::endl;
    return x+1;
}

// a component that displays text, waits a second, and then sends a signal
class printer : public boost::signals::filter<printer, void()>
{
public:
    printer(const std::string &text)
        : m_text(text)
    {}
    void operator()()
    {
        std::cout << m_text << std::endl;
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        out();
    }
private:
    std::string m_text;
};

// a function to submit the first task
template<typename Threadpool, typename Next>
void submit(Threadpool &threadpool, Next &next)
{
    tp::task< void > task(
        threadpool.submit(
            boost::bind(&Next::send, boost::ref(next))));
}

int main( int argc, char *argv[])
{
   typedef 
        tp::pool<
            tp::fixed,
            tp::unbounded_channel< tp::fifo >
        > threadpool_type;
        
    threadpool_type pool(tp::max_poolsize(5));

    typedef boost::signals::storage<void(int)> source_type;
    typedef boost::signals::function<void(int), int(int)> filter_type;
    typedef async_component<threadpool_type, filter_type> async_filter_type;

    // our components
    source_type source(1);
    async_filter_type increase1(pool, inc_fn);
    filter_type increase2(inc_fn);
    filter_type increase3(inc_fn);
    async_filter_type increase4(pool, inc_fn);
    async_filter_type increase5(pool, inc_fn);
    
    // our network
    //  increase1 >>= increase2 >>= increase3 will be in its own thread
    //  increase3 will be in its own thread
    //  increase4 will be in its own thread
    source
        | (increase1 >>= increase2 >>= increase3)
        | (increase4 >>= increase5);

    // submit the first task
    submit(pool, source);
           
    // wait a little
    boost::this_thread::sleep(boost::posix_time::seconds(1));

    // --------------------------------------------------------------

/*
   typedef 
        tp::pool<
            tp::fixed,
            tp::unbounded_channel< tp::priority< int > >
        > priority_threadpool_type;
        
    priority_threadpool_type priority_pool(tp::max_poolsize(5));
    
    typedef boost::signals::storage<void()> print_starter_type;
    typedef async_component<priority_threadpool_type, printer> async_printer_type;
    
    print_starter_type print_starter;
    async_printer_type print1(priority_pool, "World");
    async_printer_type print2(priority_pool, " ");
    async_printer_type print3(priority_pool, "Hello");
    async_printer_type print4(priority_pool, "!");
    
    print_starter
        | print1
        | print2
        | print3
        | print4;
        
    // submit the first task
    submit(priority_pool, print_starter);

    // wait a little
    boost::this_thread::sleep(boost::posix_time::seconds(1));
*/
        
    // --------------------------------------------------------------

    typedef boost::signals::storage<void()> tick_starter_type;
    typedef async_component<threadpool_type, printer> ticker_type;
    
    // our components
    tick_starter_type tick_starter;
    ticker_type ticker1(pool, "tick 1..."), ticker2(pool, "tick 2..."), ticker3(pool, "tick 3...");

    // our network
    tick_starter >>= ticker1 >>= ticker2 >>= ticker3 >>= ticker1;
    
    // submit the first task
    submit(pool, tick_starter);

    // wait a little
    boost::this_thread::sleep(boost::posix_time::seconds(5));

    pool.shutdown();
    return 0;
}

// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <cstdlib>
#include <stdexcept>

#include <boost/bind.hpp>
#include <boost/bind/placeholders.hpp>
#include <boost/future/future.hpp>

//#define signalslib signals
//#define signals signals
//#include <boost/thread_safe_signal.hpp>
#include <boost/signals.hpp>

#include <boost/dataflow/signals/component/storage.hpp>
#include <boost/dataflow/signals/component/function.hpp>
#include <boost/dataflow/signals/connection/operators.hpp>

#include "boost/tp/fifo.hpp"
#include "boost/tp/lazy.hpp"
#include "boost/tp/pool.hpp"
#include "boost/tp/poolsize.hpp"
#include "boost/tp/bounded_channel.hpp"

namespace boost { namespace dataflow {

namespace signals {

    // a PortTraits type for the new delay filter
    struct delay_filter
        : public port_traits<ports::producer_consumer, tag>
    {};

} // namespace signals

namespace extension {

    // connecting a producer to a delay
    template<>
    struct binary_operation_impl<signals::producer<void(int)>, signals::delay_filter, operations::connect>
    {
        typedef void result_type;
        template<typename Producer, typename Consumer>
        void operator()(Producer &producer, Consumer &consumer)
        {
            // the connection will store a copy of the delay filter in the signal
            producer.connect(boost::bind(&Consumer::operator(), consumer, _1));
        }
    };

    // getting the right operator() overload from a consumer
    template<typename SignatureSequence>
    struct get_keyed_port_impl<signals::call_consumer<SignatureSequence>, signals::delay_filter >
    {
        typedef const boost::function<void(int)> result_type;
        
        // return the void(int) overload of operator()
        template<typename ConsumerPort>
        result_type operator()(ConsumerPort &consumer)
        {
            return get_keyed_port_impl<signals::call_consumer<SignatureSequence>, signals::producer<void(int)> >()(consumer);
        };
    };

    // connecting a delay to a consumer
    template<>
    struct binary_operation_impl<signals::delay_filter, signals::consumer<void(int)>, operations::connect>
    {
        typedef void result_type;
        template<typename Producer, typename Consumer>
        void operator()(Producer &producer, Consumer &consumer)
        {
            // connect the underlying delayed component to the consumer
            connect(producer.next(), consumer);
        }
    };
    
    // connecting a delay to a delay
    template<>
    struct binary_operation_impl<signals::delay_filter, signals::delay_filter, operations::connect>
    {
        typedef void result_type;
        template<typename Producer, typename Consumer>
        void operator()(Producer &producer, Consumer &consumer)
        {
            // connect the underlying delayed component to the target delay
            connect(producer.next(), consumer);
        }
    };
    
} // namespace extension

} } // namespace boost::dataflow


namespace tp = boost::tp;

// our new delay class - it will create a new task for the next component
// when it's operator() is called.
template<typename Threadpool, typename Next>
class delay
{
public:
    typedef boost::dataflow::signals::delay_filter dataflow_traits;

    delay(Threadpool &threadpool, Next &next)
        : m_threadpool(threadpool), m_next(next)
    {
        // record the appropriate operator() overload of Next into m_next_function
        typedef typename boost::dataflow::utility::slot_type<void(int), Next>::type mem_fn_type;

        m_next_function = boost::dataflow::utility::bind_mem_fn<mem_fn_type, Next>
            (static_cast<mem_fn_type>(&Next::operator()), next);
    }
    
    void operator()(int x)
    {
        // add the next function as a task in the pool
        std::cout << "adding task" << std::endl;
        boost::tp::task< void > t(
            m_threadpool.submit(
                boost::bind(m_next_function, x)));        
    }
    
    Next &next() const
    {   return m_next; }
    
private:
    Threadpool &m_threadpool;
    Next &m_next;
    boost::function<void(int)> m_next_function;
};

// a make function for delay
template<typename Threadpool, typename Next>
delay<Threadpool, Next> make_delay(Threadpool &threadpool, Next &next)
{
    return delay<Threadpool, Next>(threadpool, next);
}

// just an operation to work with
int inc_fn(int x)
{
    std::cout << "filter: " << x+1 << std::endl;
    return x+1;
}

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

    // our components
    source_type source(1);
    filter_type increase(inc_fn);
    filter_type increase2(inc_fn);
    filter_type increase3(inc_fn);
    filter_type increase4(inc_fn);
    
    // our network
    //  increase >>= increase2 will be in its own thread
    //  increase3 will be in its own thread
    //  increase4 will be in its own thread
    source
        | (make_delay(pool, increase) >>= increase2)
        | (make_delay(pool, increase3) >>= make_delay(pool, increase4));
        
    // submit the first task    
    submit(pool, source);

    // wait a little
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += 3;
    boost::thread::sleep(xt);

    return 0;
}

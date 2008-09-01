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

#include "boost/tp/fifo.hpp"
#include "boost/tp/lazy.hpp"
#include "boost/tp/pool.hpp"
#include "boost/tp/poolsize.hpp"
#include "boost/tp/bounded_channel.hpp"

namespace boost { namespace dataflow {

namespace signals {

    // a PortTraits type for the new async filter
    template<typename T>
    struct async_filter
        : public port_traits<ports::producer_consumer, tag>
    {};

} // namespace signals

namespace extension {

    // connecting a producer to a async
    template<typename Signature>
    struct binary_operation_impl<signals::producer<Signature>, signals::async_filter<Signature>, operations::connect>
    {
        typedef void result_type;
        template<typename Producer, typename Consumer>
        void operator()(Producer &producer, Consumer &consumer)
        {
            typedef typename boost::dataflow::utility::slot_type<Signature, Consumer>::type mem_fn_type;
             
            connect(producer, boost::dataflow::utility::bind_mem_fn_flexible<mem_fn_type>
            (static_cast<mem_fn_type>(&Consumer::operator()), consumer));
        }
    };

    // getting the right operator() overload from a consumer
    template<typename SignatureSequence, typename Signature>
    struct get_keyed_port_impl<signals::call_consumer<SignatureSequence>, signals::async_filter<Signature> >
    {
        typedef const boost::function<Signature> result_type;
        
        // return the correct overload of operator()
        template<typename ConsumerPort>
        result_type operator()(ConsumerPort &consumer)
        {
            return get_keyed_port_impl<signals::call_consumer<SignatureSequence>, signals::producer<Signature> >()(consumer);
        };
    };

    // connecting a async to a consumer
    template<typename Signature>
    struct binary_operation_impl<signals::async_filter<Signature>, signals::consumer<Signature>, operations::connect>
    {
        typedef void result_type;
        template<typename Producer, typename Consumer>
        void operator()(Producer &producer, Consumer &consumer)
        {
            // connect the underlying asynced component to the consumer
            connect(producer.next(), consumer);
        }
    };
    
    // connecting a async to a async
    template<typename Signature>
    struct binary_operation_impl<signals::async_filter<Signature>, signals::async_filter<Signature>, operations::connect>
    {
        typedef void result_type;
        template<typename Producer, typename Consumer>
        void operator()(Producer &producer, Consumer &consumer)
        {
            // connect the underlying asynced component to the target async
            connect(producer.next(), consumer);
        }
    };
    
} // namespace extension

} } // namespace boost::dataflow


namespace tp = boost::tp;

namespace detail {

    struct bind_functor
    {
        typedef boost::function<void()> result_type;
        
        template<typename T0, typename T1>
        result_type operator()(const T0 &t0, const T1 &t1)
        {
            return boost::bind(t0, t1);
        }

        template<typename T0, typename T1, typename T2>
        result_type operator()(const T0 &t0, const T1 &t1, const T2 &t2)
        {
            return boost::bind(t0, t1, t2);
        }

        template<typename T0, typename T1, typename T2, typename T3>
        result_type operator()(const T0 &t0, const T1 &t1, const T2 &t2, const T3 &t3)
        {
            return boost::bind(t0, t1, t2, t3);
        }
        
        //...
    };

    template<typename Threadpool, typename Next, typename Signature>
    class async_impl
    {
    public:
        typedef void result_type;
        async_impl(Threadpool &threadpool, Next &next)
            : m_threadpool(threadpool), m_next(next)
        {
            // record the appropriate operator() overload of Next into m_next_function
            typedef typename boost::dataflow::utility::slot_type<Signature, Next>::type mem_fn_type;

            m_next_function = boost::dataflow::utility::bind_mem_fn<mem_fn_type, Next>
                (static_cast<mem_fn_type>(&Next::operator()), next);
        }
        
        template <class Seq>
        void operator()(const Seq &vec_par) const
        {
            // add the next function as a task in the pool
            std::cout << "adding task" << std::endl;
            boost::fusion::fused<bind_functor> fused_bind;
            
            boost::tp::task< void > t(
            m_threadpool.submit(fused_bind(boost::fusion::join(
                boost::fusion::make_vector(m_next_function), vec_par))));
        }
        Next &next() const
        {   return m_next; }
        
    private:
        Threadpool &m_threadpool;
        Next &m_next;
        boost::function<Signature> m_next_function;
    };
}

// our new async class - it will create a new task for the next component
// when it's operator() is called.
template<typename Threadpool, typename Next, typename Signature>
class async : public boost::fusion::unfused_inherited<
    detail::async_impl<Threadpool,Next,Signature>,
    typename boost::function_types::parameter_types<Signature>::type >
{
    typedef boost::fusion::unfused_inherited<
        detail::async_impl<Threadpool,Next,Signature>,
        typename boost::function_types::parameter_types<Signature>::type>
        base_type;

public:
    typedef boost::dataflow::signals::async_filter<Signature> dataflow_traits;

    async(Threadpool &threadpool, Next &next)
        : base_type(threadpool, next)
    {}
};

// a make function for async
template<typename Signature, typename Threadpool, typename Next>
async<Threadpool, Next, Signature> make_async(Threadpool &threadpool, Next &next)
{
    return async<Threadpool, Next, Signature>(threadpool, next);
}

// just an operation to work with
int inc_fn(int x)
{
    std::cout << "filter: " << x+1 << std::endl;
    return x+1;
}

// a component to work with
class binary_filter : public boost::signals::filter<binary_filter, void(double, int)>
{
public:
    void operator()(double d, int i)
    {
        std::cout << "binary filter: " << d*2 << ", " << i+1 << std::endl;
        out(d*2, i+1);
    }
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
        | (make_async<void(int)>(pool, increase) >>= increase2)
        | (make_async<void(int)>(pool, increase3) >>= make_async<void(int)>(pool, increase4));
        

    // second network
    typedef boost::signals::storage<void(double, int)> binary_source_type;

    // our components
    binary_source_type binary_source(boost::fusion::make_vector(1.0, 1));
    binary_filter duplicate1, duplicate2, duplicate3;
    
    // our network
    binary_source
        >>= make_async<void(double, int)>(pool, duplicate1)
        >>= make_async<void(double, int)>(pool, duplicate2);
        
    // submit the first tasks 
    submit(pool, source);   
    submit(pool, binary_source);

    // wait a little
    boost::xtime xt;
    boost::xtime_get(&xt, boost::TIME_UTC);
    xt.sec += 3;
    boost::thread::sleep(xt);

    return 0;
}

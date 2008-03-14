// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/signals/component/storage.hpp>

#include <boost/foreach.hpp>

#include <iostream>

namespace boost { namespace dataflow {

namespace signals {

    // we're making a new PortTraits for a operator() consumer that can track
    struct tracking_call_consumer
        : public port_traits<ports::consumer, tag>
    {};

} // namespace signals

namespace extension {

    // here we specialize the connect operation
    template<typename T>
    struct binary_operation_impl<signals::producer<T>, signals::tracking_call_consumer, operations::connect>
    {
        typedef boost::signals::connection result_type;
        
        template<typename Producer, typename Consumer>
        result_type operator()(Producer &producer, Consumer &consumer)
        {
            // object_type is so that a port_adapter can be used on the consumer
            typedef typename get_object_type<Consumer>::type object_type;
            typedef typename boost::dataflow::utility::slot_type<T, object_type>::type mem_fn_type;
            
            // grab the correct overload of operator()
            boost::function<T> mem_fn = boost::dataflow::utility::bind_mem_fn<mem_fn_type, object_type>
                (static_cast<mem_fn_type>(&object_type::operator()), get_object(consumer));

            // make the connection, register it with the consumer
            result_type connection = producer.connect(mem_fn);
            get_object(consumer).register_connection(connection);
            return connection;
        }
    };
    
} // namespace extension

} } // namespace boost::dataflow

using namespace boost;
namespace df = boost::dataflow;

// simple consumer that tracks its connections - when it receives a specified
// number of signals, it will disconnect itself from all producers
class trackable_consumer
    : public df::port<df::signals::tracking_call_consumer>
{
public:
    trackable_consumer(unsigned max_count)
        : m_count(0), m_max_count(max_count)
    {}
    
    void operator()()
    {
        m_count++;
        if (m_count >= m_max_count)
            disconnect_all();
    }
    
    void register_connection(const signals::connection &connection)
    {   m_connections.push_back(connection); }
    
    unsigned count()
    {   return m_count; }
    
private:
    void disconnect_all()
    {
        BOOST_FOREACH(signals::connection &c, m_connections)
            c.disconnect();
        m_connections.clear();
    }
        
    typedef std::vector<signals::connection> connections_type;
    connections_type m_connections;
    unsigned m_count, m_max_count;
};

int main(int, char* [])
{
    signals::storage<void()> producer;
    trackable_consumer c1(1), c3(3), c5(5);
    
    connect(producer, c1);
    connect(producer, c3);
    connect(producer, c5);

    // send out 5 signals.  c1 should disconnect after 1, c3 after 3, c5 after 5
    for(int i=0; i<5; i++)
        producer();
        
    std::cout << c1.count() << std::endl; // 1
    std::cout << c3.count() << std::endl; // 3
    std::cout << c5.count() << std::endl; // 5
    
    // get_port_c<0> will retreive the signal - there are no more consumers connected
    std::cout << df::get_port_c<0, df::signals::tag>(producer).num_slots() << std::endl; // 0

    return 0;
}

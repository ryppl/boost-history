// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef SIGNAL_NETWORK_SIGNAL_SUPPORT_HPP
#define SIGNAL_NETWORK_SIGNAL_SUPPORT_HPP

#include <boost/dataflow/support.hpp>
#include <boost/dataflow/signal/connection/detail/bind_object.hpp>
#include <boost/signal.hpp>


namespace boost { namespace dataflow {

namespace signals {

struct mechanism;

template<typename T>
struct producer
    : public port_traits<mechanism, ports::producer, concepts::port>
{
    typedef T produced_signature_type;
};

struct call_consumer
    : public port_traits<mechanism, ports::consumer, concepts::keyed_port>
{};

template<typename T>
struct consumer
    : public port_traits<mechanism, ports::consumer, concepts::port>
{
    typedef T consumed_signature_type;
};

} // namespace signals

template<typename Signature, typename Combiner, typename Group, typename GroupCompare>
struct port_traits_of<signals::mechanism, ports::producer, boost::signal<Signature, Combiner, Group, GroupCompare> >
{
    typedef signals::producer<Signature> type;
};

template<typename Signature>
struct port_traits_of<signals::mechanism, ports::consumer, boost::function<Signature> >
{
    typedef signals::consumer<Signature> type;
};

namespace extension
{
    template<typename Signature>
    struct get_keyed_port_impl<signals::call_consumer, signals::producer<Signature> >
    {
        template<typename ConsumerPort, typename ProducerPort>
        struct apply
        {
            typedef const boost::function<Signature> type;
            
            static type call(ConsumerPort &consumer, ProducerPort &)
            {
                return boost::signals::detail::bind_object<Signature, ConsumerPort>()
                (static_cast<typename boost::signals::detail::slot_type<Signature, ConsumerPort>::type>(&ConsumerPort::operator()), consumer);
            };
        };
    };
    
    template<typename T>
    struct binary_operation_impl<operations::connect, signals::producer<T>, signals::consumer<T> >
    {
        template<typename Producer, typename Consumer>
        struct apply
        {
            static void call(Producer &producer, Consumer &consumer)
            {
                producer.connect(consumer);
            }
        };
    };
}

} } // namespace boost::dataflow

namespace boost { namespace signals {

#define DATAFLOW_TEMPLATE_MECHANISM boost::dataflow::signals::mechanism
#define DATAFLOW_TEMPLATE_BINARY_OPERATION connect
#include <boost/dataflow/templates/binary_operation.hpp>
#undef DATAFLOW_TEMPLATE_BINARY_OPERATION
#undef DATAFLOW_TEMPLATE_MECHANISM

template<typename Component>
inline void invoke(Component &component)
{
    boost::dataflow::component_operation<boost::dataflow::operations::invoke, boost::dataflow::signals::mechanism>(component);
}

} } // namespace boost::phoenix

#endif // SIGNAL_NETWORK_SIGNAL_SUPPORT_HPP
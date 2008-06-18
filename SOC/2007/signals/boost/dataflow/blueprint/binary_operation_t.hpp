// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_CONNECTION_T_HPP
#define BOOST_DATAFLOW_BLUEPRINT_CONNECTION_T_HPP

#include <boost/dataflow/blueprint/binary_operation.hpp>
#include <boost/dataflow/support/binary_operation.hpp>
#include <boost/dataflow/support/keyed_port.hpp>


#include <iostream>

namespace boost { namespace dataflow { namespace blueprint {

namespace detail
{    
    template <typename ProducerPort, typename ConsumerPort, typename Operation, typename Tag>
    typename disable_if<
            dataflow::are_binary_operable<
                ProducerPort,
                ConsumerPort,
                Operation,
                Tag                
            >
        >::type
    call_binary_operation(port &producer, port &consumer)
    {
        throw std::exception();
    }
    
    template <typename ProducerPort, typename ConsumerPort, typename Operation, typename Tag>
    typename enable_if<
            dataflow::are_binary_operable<
                ProducerPort,
                ConsumerPort,
                Operation,
                Tag
            >
        >::type
    call_binary_operation(port &producer, port &consumer)
    {
        typedef typename mpl::if_<
            is_keyed_port<ProducerPort>,
            typename remove_reference<
                typename result_of::get_keyed_port<
                    ProducerPort,
                    typename traits_of<ConsumerPort, Tag>::type,
                    Tag>::type>::type,
            ProducerPort>::type * resolved_producer_type;
            
        typedef typename mpl::if_<
            is_keyed_port<ConsumerPort>,            
            typename remove_reference<
                typename result_of::get_keyed_port<
                    ConsumerPort,
                    typename traits_of<ProducerPort, Tag>::type,
                    Tag>::type>::type,
            ConsumerPort>::type * resolved_consumer_type;

        any any_producer = producer.resolve_key(consumer).get(),
            any_consumer = consumer.resolve_key(producer).get();
                
        dataflow::binary_operation<Operation, Tag>(
            *any_cast<resolved_producer_type>(any_producer),
            *any_cast<resolved_consumer_type>(any_consumer));
    }
}

template <typename ProducerPort, typename ConsumerPort, typename Operation, typename Tag, typename Enable=void>
class binary_operation_t : public binary_operation<Operation>
{
public:
    void invoke(port &producer, port &consumer)
    {
        detail::call_binary_operation<ProducerPort, ConsumerPort, Operation, Tag>(
            producer, consumer);
    }
    virtual std::auto_ptr<binary_operation<Operation> > copy() const
    {
        return std::auto_ptr<binary_operation<Operation> >(new binary_operation_t(*this));
    }
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST_DATAFLOW_BLUEPRINT_CONNECTION_HPP

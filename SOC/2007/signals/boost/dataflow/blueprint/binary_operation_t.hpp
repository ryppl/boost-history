// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_CONNECTION_T_HPP
#define BOOST_DATAFLOW_BLUEPRINT_CONNECTION_T_HPP

#include <boost/dataflow/blueprint/binary_operation.hpp>
#include <boost/dataflow/support/binary_operation.hpp>

namespace boost { namespace dataflow { namespace blueprint {

namespace detail
{    
    template <typename Operation, typename Mechanism, typename ProducerPort, typename ConsumerPort>
    typename disable_if<
            dataflow::are_binary_operable<
                Operation,
                Mechanism,
                ProducerPort,
                ConsumerPort
            >
        >::type
    call_binary_operation(port &producer, port &consumer)
    {
        throw std::exception();
    }
    
    template <typename Operation, typename Mechanism, typename ProducerPort, typename ConsumerPort>
    typename enable_if<
            dataflow::are_binary_operable<
                Operation,
                Mechanism,
                ProducerPort,
                ConsumerPort
            >
        >::type
    call_binary_operation(port &producer, port &consumer)
    {
            dataflow::binary_operation<Operation, Mechanism>(
            any_cast<ProducerPort &>(producer.get()),
            any_cast<ConsumerPort &>(consumer.get()));
    }
}

template <typename Operation, typename Mechanism, typename ProducerPort, typename ConsumerPort, typename Enable=void>
class binary_operation_t : public binary_operation<Operation>
{
public:
    void invoke(port &producer, port &consumer)
    {
        detail::call_binary_operation<Operation, Mechanism, ProducerPort, ConsumerPort>(
            producer, consumer);
    }
    virtual std::auto_ptr<binary_operation<Operation> > copy() const
    {
        return std::auto_ptr<binary_operation<Operation> >(new binary_operation_t(*this));
    }
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST_DATAFLOW_BLUEPRINT_CONNECTION_HPP
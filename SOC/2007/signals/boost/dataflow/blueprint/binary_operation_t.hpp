// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_CONNECTION_T_HPP
#define BOOST_DATAFLOW_BLUEPRINT_CONNECTION_T_HPP

#include <boost/dataflow/blueprint/connection.hpp>
#include <boost/dataflow/support/binary_operation.hpp>

#include <boost/type_traits/remove_reference.hpp>

namespace boost { namespace dataflow { namespace blueprint {

template <typename Mechanism, typename ProducerPort, typename ConsumerPort>
class connection_t : public connection
{
public:
    virtual void connect(port &producer, port &consumer)
    {
        dataflow::binary_operation<operations::connect, Mechanism>(
            *(typename remove_reference<ProducerPort>::type *) producer.get(),
            *(typename remove_reference<ConsumerPort>::type *) consumer.get());
    }
    virtual std::auto_ptr<connection> copy() const
    {
        return std::auto_ptr<connection>(new connection_t(*this));
    }
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST_DATAFLOW_BLUEPRINT_CONNECTION_HPP
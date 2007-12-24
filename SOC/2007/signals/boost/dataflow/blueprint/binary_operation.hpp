// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_CONNECTION_HPP
#define BOOST_DATAFLOW_BLUEPRINT_CONNECTION_HPP

#include <boost/dataflow/blueprint/port.hpp>

namespace boost { namespace dataflow { namespace blueprint {

template<typename Operation>
class binary_operation
{
public:
    virtual void invoke(port &producer, port &consumer)=0;
    virtual std::auto_ptr<binary_operation> copy() const=0;
    virtual ~binary_operation() {};
};

template<typename Operation>
bool are_binary_operable(port &outgoing, port &incoming)
{    
    if (outgoing.is_complemented_port())
    {
        port &resolved_incoming = incoming.resolve_key(outgoing);

        return outgoing.as<complemented_port>().template is_operable_port_to_complement<Operation>()
            && (outgoing.as<complemented_port>().complement_type_info()
                == resolved_incoming.port_type_info());
    }

    if (incoming.is_complemented_port())
    {
        port &resolved_outgoing = outgoing.resolve_key(outgoing);
        
        return incoming.as<complemented_port>().template is_operable_complement_to_port<Operation>()
            && (incoming.as<complemented_port>().complement_type_info()
            == resolved_outgoing.port_type_info());
    }

    return false;
}

template<typename Operation>
shared_ptr<binary_operation<Operation> > get_binary_operation(port & producer, port & consumer)
{
    if(!are_binary_operable<Operation>(producer, consumer))
        throw std::exception();
    if (producer.is_complemented_port())
        return static_cast<complemented_port &>(producer).port_to_complement_operation<Operation>();
    if (consumer.is_complemented_port())
        return static_cast<complemented_port &>(consumer).complement_to_port_operation<Operation>();
    return shared_ptr<binary_operation<Operation> >();
}

inline void extract(port & producer, port & consumer)
{
    get_binary_operation<operations::extract>(producer, consumer)->invoke(producer, consumer);
}



} } } // namespace boost::dataflow::blueprint

#endif // BOOST_DATAFLOW_BLUEPRINT_CONNECTION_HPP
/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__PORT_BINARY_OPERATION_ADAPTER_HPP
#define BOOST__DATAFLOW__BLUEPRINT__PORT_BINARY_OPERATION_ADAPTER_HPP


#include <boost/dataflow/blueprint/port_binary_operation_implementation.hpp>
#include <boost/dataflow/generic/port_binary_operation.hpp>
#include <boost/dataflow/generic/framework_entity/traits_of.hpp>


namespace boost { namespace dataflow { namespace blueprint {

template<typename Port1, typename Port2, typename Operation, typename BlueprintFramework>
class port_binary_operation_implementation_adapter
    : public port_binary_operation_implementation<BlueprintFramework>
{
    typedef typename BlueprintFramework::framework_type framework_type;
    typedef port<BlueprintFramework> port_type;
public:
    virtual void invoke(port_type &port1, port_type &port2, const operation &op)
    {
        if(!check_types(port1, port2, op))
            return;
    }
    virtual bool will_succeed(port_type &port1, port_type &port2, const operation &op)
    {
        if(!check_types(port1, port2, op))
            return false;

        return dataflow::port_binary_operation_will_succeed_framework
            <Operation, framework_type, framework_type>(
                port1.template get_as<Port1>(),
                port2.template get_as<Port2>(),
                op.template get_as<Operation>());
    }
private:
    bool check_types(port_type &port1, port_type &port2, const operation &op)
    {
        return ((port1.type_info() == typeid(Port1))
          && (port2.type_info() == typeid(Port2))
          && (op.type_info() == typeid(Operation)));
    }
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__PORT_BINARY_OPERATION_ADAPTER_HPP

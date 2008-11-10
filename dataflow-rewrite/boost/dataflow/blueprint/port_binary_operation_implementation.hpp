/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__PORT_BINARY_OPERATION_IMPLEMENTATION_HPP
#define BOOST__DATAFLOW__BLUEPRINT__PORT_BINARY_OPERATION_IMPLEMENTATION_HPP


#include <boost/dataflow/generic/port_binary_operation.hpp>
#include <boost/dataflow/generic/framework_entity/traits_of.hpp>
#include <boost/dataflow/blueprint/framework.hpp>
#include <boost/dataflow/blueprint/port.hpp>
#include <boost/dataflow/blueprint/operation.hpp>
#include <boost/any.hpp>

namespace boost { namespace dataflow { namespace blueprint {

template<typename BlueprintFramework>
class port_binary_operation_implementation
{
    typedef port<BlueprintFramework> port_type;
public:
    virtual boost::any invoke(port_type &port1, port_type &port2, const operation &op)=0;
    virtual bool will_succeed(port_type &port1, port_type &port2, const operation &op)=0;
    virtual ~port_binary_operation_implementation() {};
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__PORT_BINARY_OPERATION_IMPLEMENTATION_HPP

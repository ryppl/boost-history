/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__PORT_BINARY_OPERATION_HPP
#define BOOST__DATAFLOW__BLUEPRINT__PORT_BINARY_OPERATION_HPP


#include <boost/dataflow/generic/port_binary_operation.hpp>
#include <boost/dataflow/generic/framework_entity/traits_of.hpp>
#include <boost/dataflow/blueprint/framework.hpp>
#include <boost/dataflow/blueprint/framework_context.hpp>
#include <boost/dataflow/blueprint/operation.hpp>
#include <boost/assert.hpp>

namespace boost { namespace dataflow { namespace extension {

template<typename Traits1, typename Traits2>
struct port_binary_operation_impl<Traits1, Traits2, blueprint::operation,
    typename enable_if<
        mpl::and_<
            blueprint::is_blueprint_framework<typename Traits1::framework>,
            blueprint::is_blueprint_framework<typename Traits2::framework>
        >
    >::type>
{
    typedef boost::any result_type;

    template<typename Port1, typename Port2>
    boost::any operator()(Port1 &port1, Port2 &port2, const blueprint::operation &operation)
    {
        BOOST_ASSERT(port1.framework_context() == port2.framework_context());
        return port1.framework_context().port_binary_operation(port1, port2, operation);
    }
};

template<typename Traits1, typename Traits2>
struct port_binary_operation_will_succeed_impl<Traits1, Traits2, blueprint::operation,
    typename enable_if<
        mpl::and_<
            blueprint::is_blueprint_framework<typename Traits1::framework>,
            blueprint::is_blueprint_framework<typename Traits2::framework>
        >
    >::type>
{
    typedef bool result_type;

    template<typename Port1, typename Port2>
    bool operator()(Port1 &port1, Port2 &port2, const blueprint::operation &operation)
    {
        BOOST_ASSERT(port1.framework_context() == port2.framework_context());
        return port1.framework_context().port_binary_operation_will_succeed(port1, port2, operation);
    }
};

} } } // namespace boost::dataflow::extension

#endif // BOOST__DATAFLOW__BLUEPRINT__PORT_BINARY_OPERATION_HPP

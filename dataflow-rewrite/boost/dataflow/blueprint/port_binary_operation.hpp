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
#include <boost/dataflow/blueprint/operation.hpp>


namespace boost { namespace dataflow { namespace extension {

template<typename Traits1, typename Traits2>
struct port_binary_operation_impl<Traits1, Traits2, blueprint::operation,
    typename enable_if<
        mpl::and_<
            blueprint::is_blueprint_framework<typename traits_of<Traits1>::type::framework>,
            blueprint::is_blueprint_framework<typename traits_of<Traits2>::type::framework>
        >
    >::type>
{
    typedef void result_type;

    template<typename Port1, typename Port2>
    void operator()(Port1 &port1, Port2 &port2, const blueprint::operation &operation)
    {}
};

template<typename Traits1, typename Traits2>
struct port_binary_operation_will_succeed_impl<Traits1, Traits2, blueprint::operation,
    typename enable_if<
        mpl::and_<
            blueprint::is_blueprint_framework<typename traits_of<Traits1>::type::framework>,
            blueprint::is_blueprint_framework<typename traits_of<Traits2>::type::framework>
        >
    >::type>
{
    typedef bool result_type;

    template<typename Port1, typename Port2>
    bool operator()(Port1 &port1, Port2 &port2, const blueprint::operation &operation)
    {
        return false;
    }
};

} } } // namespace boost::dataflow::extension

#endif // BOOST__DATAFLOW__BLUEPRINT__PORT_BINARY_OPERATION_HPP

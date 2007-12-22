// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_KEYED_PORT_T_HPP
#define BOOST_DATAFLOW_BLUEPRINT_KEYED_PORT_T_HPP

#include <boost/dataflow/blueprint/vector_port_t.hpp>
#include <boost/dataflow/support/keyed_port.hpp>


namespace boost { namespace dataflow { namespace blueprint {

template<typename PortOrRef, typename Tag>
class port_t<
    PortOrRef,
    Tag,
    typename enable_if<
        is_keyed_port<
            typename remove_reference<PortOrRef>::type,
            Tag>
    >::type >
    : public detail::vector_port_impl<keyed_port, PortOrRef, Tag>
{
public:
    port_t(typename call_traits<PortOrRef>::param_type p)
        : detail::vector_port_impl<keyed_port, PortOrRef, Tag>(p)
    {}
    
    virtual port *clone() const
    {
        return new port_t(*this);
    };

};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST_DATAFLOW_BLUEPRINT_KEYED_PORT_T_HPP
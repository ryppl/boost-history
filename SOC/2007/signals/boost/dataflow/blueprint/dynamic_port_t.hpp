// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_DYNAMIC_PORT_T_HPP
#define BOOST_DATAFLOW_BLUEPRINT_DYNAMIC_PORT_T_HPP

#include <boost/dataflow/blueprint/port_t.hpp>
#include <boost/dataflow/support/dynamic_port.hpp>

#include <boost/ptr_container/ptr_vector.hpp>


namespace boost { namespace dataflow { namespace blueprint {

namespace detail {
    
    template<typename Base, typename PortOrRef, typename Tag>
    class dynamic_port_impl
        : public port_t_base<Base, PortOrRef, Tag>
    {
    protected:
        typedef typename dynamic_port_impl::port_type port_type;
        typedef typename dynamic_port_impl::port_traits_type port_traits_type;

    public:
        dynamic_port_impl(typename call_traits<PortOrRef>::param_type p)
            : port_t_base<Base, PortOrRef, Tag>(p)
        {
        }

        size_t num_ports() const
        {
            return get_dynamic_port_size<Tag>(dynamic_port_impl::p);
        }
        port & get_port(int port_num)
        {
            return m_ports[port_num];
        }

    protected:
        ptr_vector<port> m_ports;
    private:
        void port_t_()
        {
            for(size_t i=0; i<num_ports(); i++)
                m_ports.push_back(new port_t<result_of::get_subport<port_type, Tag> >(get_subport<Tag>(dynamic_port_impl::p, i)));
        }
    };
}

template<typename PortOrRef, typename Tag>
class port_t<
    PortOrRef,
    Tag,
    typename enable_if<
        is_dynamic_port<typename remove_reference<PortOrRef>::type,Tag>
    >::type >
    : public detail::dynamic_port_impl<vector_port, PortOrRef, Tag>
{
public:
    port_t(typename call_traits<PortOrRef>::param_type p)
        : detail::dynamic_port_impl<vector_port, PortOrRef, Tag>(p)
    {}
    virtual port *clone() const
    {
        return new port_t(*this);
    };
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST_DATAFLOW_BLUEPRINT_DYNAMIC_PORT_T_HPP
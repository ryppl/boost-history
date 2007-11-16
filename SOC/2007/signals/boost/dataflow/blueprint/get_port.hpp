// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_GET_PORT_HPP
#define BOOST_DATAFLOW_BLUEPRINT_GET_PORT_HPP

#include <boost/dataflow/blueprint/port_t.hpp>
#include <boost/dataflow/support/reflective_component.hpp>

#include <memory>

namespace boost { namespace dataflow { namespace blueprint {

namespace detail {

template<typename Mechanism, typename Component, int Ports>
struct get_port_impl;

template<typename Mechanism, typename Component>
struct get_port_impl<Mechanism, Component, 2>
{
    std::auto_ptr<port> operator()(Component &c, int port_num)
    {
        std::auto_ptr<port> ret;
        switch(port_num)
        {
        case 0:
            ret.reset(new port_t<
                typename mpl::at_c<
                    typename component_traits_of<Mechanism, Component>::type::ports,
                    0
                >::type,
                typename get_component_port_result_type<
                    Mechanism,
                    mpl::int_<0>,
                    Component
                >::type
            >(get_component_port<Mechanism, mpl::int_<0>, Component >(c)));
            break;
        case 1:
            ret.reset(new port_t<
                typename mpl::at_c<
                    typename component_traits_of<Mechanism, Component>::type::ports,
                    1
                >::type,
                typename get_component_port_result_type<
                    Mechanism,
                    mpl::int_<1>,
                    Component
                >::type
            >(get_component_port<Mechanism, mpl::int_<1>, Component>(c)));
        }
        return ret;
    }
};

} // namespace detail

template<typename Mechanism, typename Component>
std::auto_ptr<port> get_port(Component &c, int port_num)
{
    return detail::get_port_impl<
        Mechanism,
        Component,
        mpl::size<typename component_traits_of<Mechanism, Component>::type::ports>::value
    >()(c, port_num);
}

} } } // namespace boost::dataflow::blueprint

    

#endif // BOOST_DATAFLOW_BLUEPRINT_GET_PORT_HPP
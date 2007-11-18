// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_GET_PORT_HPP
#define BOOST_DATAFLOW_BLUEPRINT_GET_PORT_HPP

#include <boost/dataflow/blueprint/port_t.hpp>
#include <boost/dataflow/support/reflective_component.hpp>

#include <boost/mpl/range_c.hpp>
#include <boost/switch.hpp>

#include <memory>

namespace boost { namespace dataflow { namespace blueprint {

namespace detail {

template<typename Mechanism, typename Component>
struct get_port_case
{
    typedef void result_type;
    template<class Case>
    void operator()(Case) const
    {
        ret.reset(new port_t<
            typename mpl::at_c<
                typename component_traits_of<Mechanism, Component>::type::ports,
                Case::value
            >::type,
            typename get_component_port_result_type<
                Mechanism,
                mpl::int_<Case::value>,
                Component
            >::type
        >(get_component_port<Mechanism, mpl::int_<Case::value>, Component >(c))); \
    }
    get_port_case(Component& c, std::auto_ptr<port> &ret) : c(c), ret(ret)
    {}
    Component &c;
    std::auto_ptr<port> &ret;
};

} // namespace detail

template<typename Mechanism, typename Component>
std::auto_ptr<port> get_port(Component &c, int port_num)
{
    std::auto_ptr<port> ret;
    typedef mpl::range_c<
        int,
        0,
        mpl::size<
            typename component_traits_of<Mechanism, Component>::type::ports>
        ::value> range;
    boost::switch_<range>(port_num, detail::get_port_case<Mechanism, Component>(c, ret));
    return ret;
}

} } } // namespace boost::dataflow::blueprint

    

#endif // BOOST_DATAFLOW_BLUEPRINT_GET_PORT_HPP
// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_COMPONENT_HPP
#define BOOST_DATAFLOW_BLUEPRINT_COMPONENT_HPP

#include <boost/dataflow/blueprint/get_port.hpp>
#include <boost/dataflow/support/reflective_component.hpp>
#include <boost/dataflow/support/component_operation.hpp>
#include <boost/dataflow/support/binary_operation.hpp>

#include <boost/mpl/size.hpp>
#include <boost/shared_ptr.hpp>

namespace boost { namespace dataflow { namespace blueprint {

struct mechanism;

class component
{
public:
    virtual void invoke()=0;
    virtual int num_ports() const=0;
    virtual std::auto_ptr<port> get_port(int port_num)=0;
    virtual ~component() {};
};

template<typename Mechanism, typename Component>
class component_t : public component
{
public:
    component_t() {}
    template<typename T0>
    component_t(const T0 &t0) : component(t0) {}
    
    void invoke()
    {
        component_operation<operations::invoke, Mechanism>(component);
    }
    int num_ports() const
    {
        return mpl::size<typename component_traits_of<Mechanism, Component>::type::ports>::value;
    }
    std::auto_ptr<port> get_port(int port_num)
    {
        return blueprint::get_port<Mechanism, Component>(component, port_num);
    }
    Component &get() {return component;}
private:
    Component component;
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST_DATAFLOW_BLUEPRINT_COMPONENT_HPP


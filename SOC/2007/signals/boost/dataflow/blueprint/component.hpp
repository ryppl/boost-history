// Copyright Stjepan Rajko 2007. Use, modification and
// distribution is subject to the Boost Software License, Version
// 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_BLUEPRINT_COMPONENT_HPP
#define BOOST_DATAFLOW_BLUEPRINT_COMPONENT_HPP

#include <boost/dataflow/blueprint/port_t.hpp>
#include <boost/dataflow/support/component.hpp>
#include <boost/dataflow/support/component_operation.hpp>
#include <boost/dataflow/support/binary_operation.hpp>

#include <boost/mpl/size.hpp>
#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/shared_ptr.hpp>


namespace boost { namespace dataflow { namespace blueprint {

/// Base class for all blueprint component types.  Run-time analogue of the Component concept. 
class component
{
public:
    virtual void invoke()=0;
    virtual bool is_invocable()=0;
    virtual size_t num_ports() const=0;
    virtual port & get_port(int port_num)=0;

    virtual std::auto_ptr<component> copy() const=0;
    virtual ~component() {};
};

template<typename Base>
class based_component : public component
{
public:
    virtual Base *get_pointer()=0;
};

namespace detail
{
    template<typename T, typename Tag, typename Enable=void>
    struct try_invoke
    {
        void operator()(T &) const
        {
            throw std::exception();
        }
    };
    
    template<typename T, typename Tag>
    struct try_invoke<
        T, Tag,
        typename enable_if<
            is_component_operable<T, operations::invoke, Tag>
        >::type >
    {
        void operator()(T &c) const
        {
            component_operation<operations::invoke, Tag>(c);
        }
    };
}

template<typename Component, typename Tag=default_tag>
class component_t : public based_component<typename runtime_base_class<Component>::type>
{
    typedef typename runtime_base_class<Component>::type runtime_base_class_type;
public:
    component_t() {component_t_();}
    template<typename T0>
    component_t(const T0 &t0) : c(t0) {component_t_();}
    template<typename T0>
    component_t(T0 &t0) : c(t0) {component_t_();}
    component_t(const component_t &rhs) : c(rhs.c)
    {   component_t_(); }
    
    void invoke()
    {
        detail::try_invoke<Component, Tag>()(c);
    }
    virtual bool is_invocable()
    {
        return is_component_operable<Component, operations::invoke, Tag>::type::value;
    };
    size_t num_ports() const
    {
        return mpl::size<typename traits_of<Component, Tag>::type::ports>::value;
    }
    port & get_port(int port_num)
    {
        return ports[port_num];
    }
    virtual std::auto_ptr<component> copy() const
    {
        return std::auto_ptr<component>(new component_t<Component, Tag>(*this));
    }
    Component &get() {return c;}
    virtual runtime_base_class_type *get_pointer()
    {   return static_cast<runtime_base_class_type *>(&c); };
private:
    void component_t_()
    {
        for(size_t i=0; i<num_ports(); i++)
            ports.push_back(blueprint::get_port<Tag>(c, i));
    }
    Component c;
    ptr_vector<port> ports;
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST_DATAFLOW_BLUEPRINT_COMPONENT_HPP


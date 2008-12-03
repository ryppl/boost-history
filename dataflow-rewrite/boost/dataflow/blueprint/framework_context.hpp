/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_CONTEXT_HPP
#define BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_CONTEXT_HPP

#include <map>
#include <typeinfo>
#include <boost/dataflow/blueprint/framework_context_fwd.hpp>
#include <boost/dataflow/blueprint/port_binary_operation_implementation_adapter.hpp>
#include <boost/shared_ptr.hpp>

namespace boost { namespace dataflow { namespace blueprint {

struct port_binary_operation_key
{
protected:
    port_binary_operation_key()
    {}
public:
    template<typename Port1, typename Port2>
    port_binary_operation_key(Port1 &port1, Port2 &port2, const operation &op)
    {
        m_type_info[0] = &port1.type_info();
        m_type_info[1] = &port2.type_info();
        m_type_info[2] = &op.type_info();
    }
public:
    bool operator == (const port_binary_operation_key &rhs) const
    {
        for(int i=0; i<3; i++)
            if(*m_type_info[i] != *rhs.m_type_info[i])
                return false;
        return true;
    }
    bool operator < (const port_binary_operation_key &rhs) const
    {
        for(int i=0; i<3; i++)
        {
            if(m_type_info[i]->before(*rhs.m_type_info[i]))
                return true;
            if(!(*m_type_info[i] == *rhs.m_type_info[i]))
                return false;
        }
        return false;
    }
protected:
    const std::type_info *m_type_info[3];
};

template<typename Port1, typename Port2, typename Operation>
class construct_port_binary_operation_key : public port_binary_operation_key
{
public:
    construct_port_binary_operation_key()
    {
        m_type_info[0] = &typeid(Port1);
        m_type_info[1] = &typeid(Port2);
        m_type_info[2] = &typeid(Operation);
    }
};

template<typename BlueprintFramework>
class framework_context_base
{
    BOOST_MPL_ASSERT((is_blueprint_framework<BlueprintFramework>));

    typedef shared_ptr<port_binary_operation_implementation<BlueprintFramework> > implementation_ptr;
    typedef port<BlueprintFramework> port_type;
public:
    bool operator == (const framework_context_base &rhs) const
    {
        return this == &rhs;
    }
    template<typename Port1, typename Port2, typename Operation>
    void register_operation()
    {
        port_binary_operation_key key = construct_port_binary_operation_key<Port1,Port2,Operation>();
        m_operation_implementations[key] = implementation_ptr(
            new port_binary_operation_implementation_adapter<Port1,Port2,Operation,BlueprintFramework>);
    }
    template<typename Port1, typename Port2, typename Operation>
    bool has_registered_operation() const
    {
        port_binary_operation_key key = construct_port_binary_operation_key<Port1,Port2,Operation>();
        return m_operation_implementations.find(key) != m_operation_implementations.end();
    }
    boost::any port_binary_operation(port_type &port1, port_type &port2, const blueprint::operation &op)
    {
        port_binary_operation_key key(port1, port2, op);
        typename operation_implementations_type::iterator it = m_operation_implementations.find(key);
        if (it == m_operation_implementations.end())
            return boost::any();
        return it->second->invoke(port1, port2, op);
    }
    bool port_binary_operation_will_succeed(port_type &port1, port_type &port2, const blueprint::operation &op)
    {
        port_binary_operation_key key(port1, port2, op);
        typename operation_implementations_type::iterator it = m_operation_implementations.find(key);
        if (it == m_operation_implementations.end())
            return false;
        return it->second->will_succeed(port1, port2, op);
    }
private:
    typedef std::map<port_binary_operation_key, implementation_ptr> operation_implementations_type;
    std::map<port_binary_operation_key, implementation_ptr> m_operation_implementations;
};


template<typename BlueprintFramework, typename Enable>
class framework_context
    : public framework_context_base<BlueprintFramework>
{};

template<typename BlueprintFramework>
class framework_context<BlueprintFramework,
    typename boost::enable_if<has_framework_context<typename BlueprintFramework::framework_type> >::type>
    : public framework_context_base<BlueprintFramework>
{
    typedef typename BlueprintFramework::framework_type::framework_context_type object_type;
public:
    object_type &object()
    {   return m_object; }
    const object_type &object() const
    {   return m_object; }
private:
    object_type m_object;
};


} } } // namespace boost::dataflow::blueprint


#endif // BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_CONTEXT_HPP

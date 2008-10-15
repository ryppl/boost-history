/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_OBJECT_HPP
#define BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_OBJECT_HPP

#include <map>
#include <typeinfo>

namespace boost { namespace dataflow { namespace blueprint {

struct port_binary_operation_key
{
protected:
    port_binary_operation_key()
    {}
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
class framework_object
{
public:
    template<typename Port1, typename Port2, typename Operation>
    void register_operation()
    {
        port_binary_operation_key key = construct_port_binary_operation_key<Port1,Port2,Operation>();
        m_operation_implementations[key] = 1;
    }
    template<typename Port1, typename Port2, typename Operation>
    bool has_registered_operation() const
    {
        port_binary_operation_key key = construct_port_binary_operation_key<Port1,Port2,Operation>();
        return m_operation_implementations.find(key) != m_operation_implementations.end();
    }
private:
    std::map<port_binary_operation_key, int> m_operation_implementations;
};

} } } // namespace boost::dataflow::blueprint


#endif // BOOST__DATAFLOW__BLUEPRINT__FRAMEWORK_OBJECT_HPP

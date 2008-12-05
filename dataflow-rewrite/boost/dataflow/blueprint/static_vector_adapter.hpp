/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__STATIC_VECTOR_ADAPTER_HPP
#define BOOST__DATAFLOW__BLUEPRINT__STATIC_VECTOR_ADAPTER_HPP


#include <boost/dataflow/blueprint/framework_entity_adapter.hpp>
#include <boost/dataflow/blueprint/port_adapter.hpp>
#include <boost/dataflow/blueprint/vector.hpp>
#include <boost/dataflow/generic/entities.hpp>

#include <boost/fusion/include/for_each.hpp>
#include <boost/ptr_container/ptr_vector.hpp>


namespace boost { namespace dataflow { namespace blueprint {

template<typename BlueprintFramework>
class static_port_iterator
  : public port_iterator_base<BlueprintFramework>
{
public:
    static_port_iterator(ptr_vector<port<BlueprintFramework> > &ports, unsigned index = 0)
        : m_ports(ports)
        , m_index(index)
    {}
private:
    virtual void increment()
    {
        m_index++;
    }
    virtual bool equal(port_iterator_base<BlueprintFramework> const& other) const
    {
        const static_port_iterator *other_iterator = dynamic_cast<const static_port_iterator *> (other.get_ptr());
        return
        (
            other_iterator
         && (&m_ports == &other_iterator->m_ports)
         && (m_index == other_iterator->m_index)
        );
    }
    virtual port<BlueprintFramework>& dereference() const
    {
        return m_ports[m_index];
    }
    virtual port_iterator_base<BlueprintFramework> * clone() const
    {
        return new static_port_iterator(*this);
    }
    virtual const port_iterator_base<BlueprintFramework> * get_ptr() const
    {
        return this;
    }

    ptr_vector<port<BlueprintFramework> > &m_ports;
    unsigned m_index;
};


namespace detail
{
    template<typename BlueprintFramework>
    struct add_ports
    {
        add_ports(ptr_vector<port<BlueprintFramework> > &ports, framework_context<BlueprintFramework> &fo)
            : m_ports(ports)
            , m_fo(fo)
        {}
        template<typename Entity>
        void operator()(Entity &entity) const
        {
            m_ports.push_back(new port_adapter<BlueprintFramework, Entity &>(m_fo, entity));
        }
        ptr_vector<port<BlueprintFramework> > &m_ports;
        framework_context<BlueprintFramework> &m_fo;
    };
}


template<typename BlueprintFramework, typename VectorOrRef, typename Base=vector<BlueprintFramework> >
class static_vector_adapter : public framework_entity_adapter<BlueprintFramework, VectorOrRef, Base>
{
    typedef framework_entity_adapter<BlueprintFramework, VectorOrRef, Base> base_type;
public:
    static_vector_adapter(blueprint::framework_context<BlueprintFramework> &fo)
        : base_type(fo)
    {
        initialize_ports();
    }
    template<typename T>
    static_vector_adapter(blueprint::framework_context<BlueprintFramework> &fo, const T &t)
        : base_type(fo, t)
    {
        initialize_ports();
    }
    template<typename T>
    static_vector_adapter(blueprint::framework_context<BlueprintFramework> &fo, T &t)
        : base_type(fo, t)
    {
        initialize_ports();
    }

    virtual port_iterator<BlueprintFramework> begin()
    {
        return new static_port_iterator<BlueprintFramework>(m_ports);
    }
    virtual port_iterator<BlueprintFramework> end()
    {
        return new static_port_iterator<BlueprintFramework>(m_ports, m_ports.size());
    }
private:
    void initialize_ports()
    {
        fusion::for_each
        (
            entities_framework<typename BlueprintFramework::framework_type>(base_type::entity()),
            detail::add_ports<BlueprintFramework>(m_ports, base_type::framework_context())
        );
    }

    ptr_vector<port<BlueprintFramework> > m_ports;
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__STATIC_VECTOR_ADAPTER_HPP

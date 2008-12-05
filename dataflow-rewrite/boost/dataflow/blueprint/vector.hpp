/*=================================---------------------------------------------
    Copyright 2007,2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifndef BOOST__DATAFLOW__BLUEPRINT__VECTOR_HPP
#define BOOST__DATAFLOW__BLUEPRINT__VECTOR_HPP


#include <boost/dataflow/blueprint/port.hpp>
#include <boost/iterator/iterator_facade.hpp>

namespace boost { namespace dataflow { namespace blueprint {

template<typename BlueprintFramework>
class port_iterator_base
  : public boost::iterator_facade<
        port_iterator_base<BlueprintFramework>
      , port<BlueprintFramework>
      , boost::forward_traversal_tag>
{
public:
    virtual ~port_iterator_base(){}
    friend class boost::iterator_core_access;

    virtual void increment()=0;
    virtual bool equal(port_iterator_base const& other) const =0;
    virtual port<BlueprintFramework>& dereference() const =0;
    virtual port_iterator_base * clone() const =0;
    virtual const port_iterator_base * get_ptr() const =0;
};

template<typename BlueprintFramework>
class port_iterator
  : public port_iterator_base<BlueprintFramework>
{
public:
    port_iterator(port_iterator_base<BlueprintFramework> *ptr)
        : m_ptr(ptr)
    {}
    virtual void increment()
    {
        if(m_ptr.use_count()!=1)
            m_ptr.reset(m_ptr->clone());
        m_ptr->increment();
    }
    virtual bool equal(port_iterator_base<BlueprintFramework> const& other) const
    {
        return m_ptr->equal(other);
    }
    virtual port<BlueprintFramework>& dereference() const
    {
        return m_ptr->dereference();
    }
    virtual port_iterator_base<BlueprintFramework> * clone() const
    {
        return m_ptr->clone();
    };
    bool operator==(port_iterator_base<BlueprintFramework> const& other) const
    {
        return this->equal(other);
    }
    virtual const port_iterator_base<BlueprintFramework> * get_ptr() const
    {
        return m_ptr->get_ptr();
    }
private:
    shared_ptr<port_iterator_base<BlueprintFramework> > m_ptr;
};


template<typename BlueprintFramework>
class vector : public framework_entity<BlueprintFramework>
{
    typedef framework_entity<BlueprintFramework> base_type;
public:
    vector(blueprint::framework_context<BlueprintFramework> &fo, const std::type_info &ti)
        : base_type(fo, ti)
    {}
    struct dataflow_traits : public base_type::dataflow_traits
    {
        typedef port<BlueprintFramework> entity;
    };
    virtual port_iterator<BlueprintFramework> begin()=0;
    virtual port_iterator<BlueprintFramework> end()=0;
};

} } } // namespace boost::dataflow::blueprint

#endif // BOOST__DATAFLOW__BLUEPRINT__VECTOR_HPP

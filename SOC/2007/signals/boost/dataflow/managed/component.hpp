// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_MANAGED_COMPONENT_HPP
#define BOOST_DATAFLOW_MANAGED_COMPONENT_HPP

namespace boost { namespace dataflow { namespace managed {

class network;

class component
{
public:
    component(network &network_context);
    virtual ~component(){}
    network &network_context()
    {   return m_network_context; }
    virtual void invoke(){};
    void topological_sort_index(unsigned index)
    {
        m_topological_sort_index = index;
    }
    unsigned topological_sort_index() const
    {
        return m_topological_sort_index;
    }
private:
    network &m_network_context;
    unsigned m_topological_sort_index;
};

}}}

#endif // BOOST_DATAFLOW_MANAGED_COMPONENT_HPP
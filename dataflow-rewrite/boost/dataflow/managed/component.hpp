// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_MANAGED_COMPONENT_HPP
#define BOOST_DATAFLOW_MANAGED_COMPONENT_HPP

#include <boost/dataflow/utility/export_symbols.hpp>


namespace boost { namespace dataflow { namespace managed {

class network;
class port_base;

class component
{
public:
    BOOST_EXPORT_SYMBOLS component(network &network_context);
    BOOST_EXPORT_SYMBOLS component(const component &other);
    virtual BOOST_EXPORT_SYMBOLS ~component();
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
    void BOOST_EXPORT_SYMBOLS claim_port(port_base &p);
private:
    network &m_network_context;
    unsigned m_topological_sort_index;
};

}}}

#endif // BOOST_DATAFLOW_MANAGED_COMPONENT_HPP

// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/dataflow/managed/component.hpp>
#include <boost/dataflow/managed/network.hpp>

namespace boost { namespace dataflow { namespace managed {

component::component(network &network_context) : m_network_context(network_context), m_topological_sort_index(0)
{
    network_context.register_component(this);
};

component::component(const component &other) : m_network_context(other.m_network_context), m_topological_sort_index(0)
{
    m_network_context.register_component(this);
}

component::~component()
{
    m_network_context.unregister_component(this);
}


}}}


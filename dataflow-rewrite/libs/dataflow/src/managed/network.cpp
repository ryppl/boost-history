/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/


#include <boost/dataflow/managed/network.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/foreach.hpp>

#include <iostream>

namespace boost { namespace dataflow { namespace managed {

void network::register_component(component *c)
{
    graph_type::vertex_descriptor v = add_vertex(m_graph);
    std::cout << this << " registering " << c << " with descriptor " << v << std::endl;
    m_graph[v].ptr = c;
    m_descriptor_map[c]=v;
}

void network::unregister_component(component *c)
{
    BOOST_ASSERT(m_descriptor_map.find(c) != m_descriptor_map.end());
    graph_type::vertex_descriptor v = m_descriptor_map[c];
    std::cout << this << " unregistering " << c << " with descriptor " << v << std::endl;
    remove_vertex(v, m_graph);
    m_descriptor_map.erase(c);
    // vertex_descriptors have been adjusted
    BOOST_FOREACH(descriptor_map_type::value_type &pair, m_descriptor_map)
        if(pair.second >= v)
            pair.second--;
}

void network::notify_connect(component &producer, component &consumer)
{
    std::cout << this << " connecting " << &producer << " and " << &consumer << std::endl;
    std::cout << m_descriptor_map.size() << std::endl;
    add_edge(m_descriptor_map[&producer], m_descriptor_map[&consumer], m_graph);
    update_topological_sort();
}

void network::update_topological_sort()
{
    m_changed.clear();
    std::vector<graph_type::vertex_descriptor> topological_sort;
    boost::topological_sort(m_graph, std::back_inserter(topological_sort));
    std::reverse(topological_sort.begin(), topological_sort.end());
    for(std::vector<graph_type::vertex_descriptor>::iterator it=topological_sort.begin(); it!=topological_sort.end(); it++)
        m_graph[*it].ptr->topological_sort_index(it - topological_sort.begin());
    for(std::vector<graph_type::vertex_descriptor>::iterator it=topological_sort.begin(); it!=topological_sort.end(); it++)
        m_changed.insert(m_graph[*it].ptr);
}

void network::update()
{
    while(!m_changed.empty())
    {
        component *next = *m_changed.begin();
        m_changed.erase(m_changed.begin());
        next->invoke();
    }
}

void network::post_async(const boost::function<void()> &f)
{
    m_io_service->post(f);
}

}}}


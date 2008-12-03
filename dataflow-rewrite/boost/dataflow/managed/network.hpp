// Copyright 2008 Stjepan Rajko.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_DATAFLOW_MANAGED_NETWORK_HPP
#define BOOST_DATAFLOW_MANAGED_NETWORK_HPP

#include <vector>
#include <set>

#include <iostream>
#include <boost/dataflow/managed/component.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/topological_sort.hpp>

namespace boost { namespace dataflow { namespace managed {


namespace detail {
    struct component_ptr_cmp
    {
        bool operator()(const component * const lhs, const component * const rhs) const
        {
            return lhs->topological_sort_index() < rhs->topological_sort_index();
        }
    };
}

class network
{
    struct node_t
    {
        component *ptr;
    };
    typedef boost::adjacency_list<
        boost::vecS, boost::vecS, boost::bidirectionalS, node_t> graph_type;

public:
    void register_component(component *c)
    {
        std::cout << this << " registering " << c << std::endl;
        graph_type::vertex_descriptor v = add_vertex(m_graph);
        m_graph[v].ptr = c;
        std::cout << m_descriptor_map.size() << std::endl;
        m_descriptor_map[c]=v;
        std::cout << m_descriptor_map.size() << std::endl;
    }
    void unregister_component(component *c)
    {
        std::cout << this << " unregistering " << c << std::endl;
        BOOST_ASSERT(m_descriptor_map.find(c) != m_descriptor_map.end());
        graph_type::vertex_descriptor v = m_descriptor_map[c];
        remove_vertex(v, m_graph);
    }
    void notify_connect(component &producer, component &consumer)
    {
        std::cout << this << " connecting " << &producer << " and " << &consumer << std::endl;
        std::cout << m_descriptor_map.size() << std::endl;
        add_edge(m_descriptor_map[&producer], m_descriptor_map[&consumer], m_graph);
        update_topological_sort();
    }
    void notify_change(component &changed)
    {
        m_changed.insert(&changed);
    }
    typedef std::set<component *, detail::component_ptr_cmp> changed_type;
    const changed_type &changed_components()
    {
        return m_changed;
    }
    typedef std::vector<component *> topological_sort_type;
    void topological_sort(const topological_sort_type &topological_sort)
    {
        m_changed.clear();
        for(topological_sort_type::const_iterator it=topological_sort.begin(); it!=topological_sort.end(); it++)
            (*it)->topological_sort_index(it - topological_sort.begin());
        for(topological_sort_type::const_iterator it=topological_sort.begin(); it!=topological_sort.end(); it++)
            m_changed.insert(*it);
    }
    void update_topological_sort()
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
    void update()
    {
        while(!m_changed.empty())
        {
            component *next = *m_changed.begin();
            m_changed.erase(m_changed.begin());
            next->invoke();
        }
    }
private:
    graph_type m_graph;
    std::map<component *, graph_type::vertex_descriptor> m_descriptor_map;
    changed_type m_changed;
};

inline void notify_change(component &c)
{
    c.network_context().notify_change(c);
}

} } }

#endif // BOOST_DATAFLOW_MANAGED_PORT_HPP

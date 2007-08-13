// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_EXAMPLE_HELPER_HPP
#define BOOST_GRAPH_EXAMPLE_HELPER_HPP

#include <algorithm>

//[add_named_vertex
template <typename Graph, typename VertexMap>
typename boost::graph_traits<Graph>::vertex_descriptor
add_named_vertex(Graph& g, const std::string& name, VertexMap& vm)
{
    typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
    typedef typename VertexMap::iterator Iterator;

    Vertex v;
    Iterator iter;
    bool inserted;
    tie(iter, inserted) = vm.insert(make_pair(name, Vertex()));
    if(inserted) {
        // The name was unique so we need to add a vertex to the
        // graph and associate it with this name.
        v = add_vertex(g);
        g[v].name = name;
        iter->second = v;
    }
    else {
        // We had alread inserted this name so we can return
        // the associated vertex.
        v = iter->second;
    }
    return v;
}
//]

//[property_comparator
template <typename PropertyMap>
struct property_greater
{
    typedef typename boost::property_traits<PropertyMap>::key_type Key;
    property_greater(PropertyMap pm)
        : m_prop(pm)
    { }

    bool operator ()(Key a, Key b) const
    {
        return get(m_prop, a) > get(m_prop, b);
    }

    PropertyMap m_prop;
};
//]

//[sort_vertices
template <typename VertexVector, typename Graph, typename PropertyMap>
void
sort_vertices(VertexVector& v, const Graph& g, PropertyMap pm)
{
    BOOST_ASSERT(v.size() == num_vertices(g));
    size_t x = 0;
    typename boost::graph_traits<Graph>::vertex_iterator i, end;
    for(boost::tie(i, end) = boost::vertices(g); i != end; ++i) {
        v[x++] = *i;
    }

    std::sort(v.begin(), v.end(), property_greater<PropertyMap>(pm));
}
//]

#endif

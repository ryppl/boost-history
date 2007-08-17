// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_EXAMPLE_HELPER_HPP
#define BOOST_GRAPH_EXAMPLE_HELPER_HPP

#include <string>
#include <map>
#include <algorithm>

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

template <typename Graph, typename InputStream>
inline std::map<
        std::string,
        typename boost::graph_traits<Graph>::vertex_descriptor>
read_graph(Graph& g, InputStream& is)
{
    typedef typename boost::graph_traits<Graph>::vertex_descriptor Vertex;
    std::map<std::string, Vertex> verts;
    for(std::string line; std::getline(is, line); ) {
        if(line.empty()) continue;
        std::size_t index = line.find_first_of(',');
        std::string first(line, 0, index);
        std::string second(line, index + 1);

        Vertex u = add_named_vertex(g, first, verts);
        Vertex v = add_named_vertex(g, second, verts);
        add_edge(u, v, g);
    }
    return verts;
}



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

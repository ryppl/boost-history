// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_EXAMPLE_HELPER_HPP
#define BOOST_GRAPH_EXAMPLE_HELPER_HPP

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

#endif

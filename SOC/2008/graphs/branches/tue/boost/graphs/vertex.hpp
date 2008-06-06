
#ifndef VERTEX_HPP
#define VERTEX_HPP

// A vertex, at least for an undirected graph, is simply an repository 
// for the vertex' properties and an interface for the its incidence
// list.
//
// For directed graphs, it's basically the same, but has an out edge
// list and/or an in edge list, and the edge properties are stored on
// the out edge list.

template <typename VertexProps, typename IncStore>
class vertex
{
    typedef IncStore incidence_store;
public:
    typedef VertexProps vertex_properties;
    typedef typename incidence_store::vertex_descriptor vertex_descriptor;
    typedef typename incidence_store::property_descriptor property_descriptor;

    vertex();
    vertex(vertex_properties const& vp);

    void connect(vertex_descriptor, property_descriptor);

private:
    vertex_properties _props;
    incidence_store _edges;
};

template <typename VP, typename IS>
vertex<VP,IS>::vertex()
    : _props()
    , _edges()
{ }

template <typename VP, typename IS>
vertex<VP,IS>::vertex(vertex_properties const& vp)
    : _props(vp)
    , _edges()
{ }

template <typename VP, typename IS>
void
vertex<VP,IS>::connect(vertex_descriptor v, property_descriptor p)
{
    _edges.add(make_pair(v, p));
}

#endif


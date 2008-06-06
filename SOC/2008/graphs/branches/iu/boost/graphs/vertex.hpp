
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

    typedef typename incidence_store::size_type incidence_size_type;

    inline vertex();
    inline vertex(vertex_properties const& vp);

    inline void connect(vertex_descriptor, property_descriptor);
    inline void disconnect(vertex_descriptor, property_descriptor);
    template <typename Eraser> inline void disconnect(vertex_descriptor, Eraser);

    inline incidence_size_type degree() const;

    inline vertex_properties& properties();

    inline bool operator<(vertex const&) const;

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

/**
 * Connect this vertex to the vertex v with edge properties p.
 */
template <typename VP, typename IS>
void
vertex<VP,IS>::connect(vertex_descriptor v, property_descriptor p)
{
    _edges.add(make_pair(v, p));
}

/**
 * Disconnect the incidedent edge given by the vertex v with edge properties p.
 */
template <typename VP, typename IS>
void
vertex<VP,IS>::disconnect(vertex_descriptor v, property_descriptor p)
{
    _edges.remove(make_pair(v, p));
}

template <typename VP, typename IS>
template <typename Eraser>
void
vertex<VP,IS>::disconnect(vertex_descriptor v, Eraser erase)
{
    _edges.remove(v, erase);
}

/**
 * Return the degree (number of incident edges) of this vertex.
 */
template <typename VP, typename IS>
typename vertex<VP,IS>::incidence_size_type
vertex<VP,IS>::degree() const
{
    return _edges.size();
}

/**
 * Return the properties associated with this vertex (if any).
 */
template <typename VP, typename IS>
typename vertex<VP,IS>::vertex_properties&
vertex<VP,IS>::properties()
{
    return _props;
}

/**
 * The default comparison of vertices always delegates the comparison to the
 * stored vertex properties. This allows developers to express custom
 * comparitors with respect to the properties and have the vertex sets or other
 * vertex ordering operations work as they might expect.
 *
 * @requires LessThanComparable<vertex_properties>.
 */
template <typename VP, typename IS>
bool
vertex<VP,IS>::operator<(vertex const& v) const
{
    return _props < v._props;
}

#endif


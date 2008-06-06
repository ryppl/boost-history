
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_EDGE_VECTOR_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_EDGE_VECTOR_HPP

#include <vector>

#include <boost/graphs/adjacency_list/es/indexed_edge_iterator.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The edge vector implements a trivial multiset of edges for a graph (i.e., a
 * multigraph). Note that the underlying store does not permit the removal of
 * edges from the graph.
 */
template <typename Edge, template <typename> class Alloc>
class basic_edge_vector
{
public:
    typedef Edge edge_type;
    typedef typename edge_type::descriptor_type edge_descriptor;
    typedef typename edge_type::vertex_descriptor vertex_descriptor;
    typedef typename edge_type::properties_type edge_properties;

    typedef std::vector<edge_type, Alloc<edge_type> > edge_store;
    typedef indexed_edge_iterator<edge_store> edge_iterator;
    typedef typename edge_store::size_type edges_size_type;

    // FIXME:
    typedef indexed_property_map_tag edge_property_map_category;

    basic_edge_vector();

    // Add edge
    edge_descriptor add_edge(vertex_descriptor u, vertex_descriptor v);
    edge_descriptor add_edge(vertex_descriptor u, vertex_descriptor v, edge_properties const& ep);

    // Number of edges.
    edges_size_type num_edges() const;

    // Edge iterator access.
    std::pair<edge_iterator, edge_iterator> edges() const;
    edge_iterator begin_edges() const;
    edge_iterator end_edges() const;

private:
    edge_store _edges;
};

/**
 * Specialize storage traits for all types of edge vectors to use indexed
 * descriptors rather than memory descriptors.
 *
 * @todo This don't work.
 */
template <typename Edge, template <typename> class Alloc>
struct storage_traits< basic_edge_vector<Edge, Alloc> >
{
    typedef std::size_t descriptor_type;
};

/**
 * The default specialization uses the standard allocator.
 */
template <typename Edge>
struct edge_vector : basic_edge_vector<Edge, std::allocator> { };

/**
 * Specialize the storage traits for the default specialization. We wouldn't
 * have to do this if partial specializations over template template parameters
 * worked the way I thought they did.
 */
template <typename Edge>
struct storage_traits< edge_vector<Edge> >
{
    typedef std::size_t descriptor_type;
};

// Functions

template <typename E, template <typename> class A>
basic_edge_vector<E,A>::basic_edge_vector()
    : _edges()
{ }

/**
 * Add an edge to the store with no or default properties. Note that adding
 * an edge using this method does not manage the endpoints of each vertex.
 * Basically, this class is not much more than a simple edge list.
 *
 * Adding an edge does not invalidate any descriptors.
 *
 * @complexity O(1) amortized
 */
template <typename E, template <typename> class A>
typename basic_edge_vector<E,A>::edge_descriptor
basic_edge_vector<E,A>::add_edge(vertex_descriptor u, vertex_descriptor v)
{
    return add_edge(u, v, edge_properties());
}

/**
 * Add an edge to the store with the given properties. Note that adding an edge
 * to a graph using this method does not manage the endpoints of each vertex.
 * Basically, this class is not much more than a simple edge list.
 *
 * Adding an edge does not invalidate any descriptors.
 *
 * @complexity O(1) amortized
 */
template <typename E, template <typename> class A>
typename basic_edge_vector<E,A>::edge_descriptor
basic_edge_vector<E,A>::add_edge(vertex_descriptor u,
                                 vertex_descriptor v,
                                 edge_properties const& ep)
{
    // Add the edge object. Its descriptor is its index.
    _edges.push_back(edge_type(u, v, ep));
    return _edges.size() - 1;
}

/**
 * Get the number of edges.
 *
 * @complexity O(1)
 */
template <typename E, template <typename> class A>
typename basic_edge_vector<E,A>::edges_size_type
basic_edge_vector<E,A>::num_edges() const
{
    return _edges.size();
}

/**
 * Get an iterator range over the edges in the edge set.
 */
template <typename E, template <typename> class A>
std::pair<
    typename basic_edge_vector<E,A>::edge_iterator,
    typename basic_edge_vector<E,A>::edge_iterator
>
basic_edge_vector<E,A>::edges() const
{
    return make_pair(begin_edges(), end_edges());
}

/**
 * Get an iterator to the first edge.
 */
template <typename E, template <typename> class A>
typename basic_edge_vector<E,A>::edge_iterator
basic_edge_vector<E,A>::begin_edges() const
{
    return edge_iterator(_edges, _edges.begin());
}

template <typename E, template <typename> class A>
typename basic_edge_vector<E,A>::edge_iterator
basic_edge_vector<E,A>::end_edges() const
{
    return edge_iterator(_edges, _edges.end());
}

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

#endif

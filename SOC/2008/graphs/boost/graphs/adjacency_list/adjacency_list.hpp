
#ifndef BOOST_GRAPH_ADJACENCY_LIST_HPP
#define BOOST_GRAPH_ADJACENCY_LIST_HPP

#include <boost/assert.hpp>

#include <boost/graphs/utility.hpp>
#include <boost/graphs/adjacency_list/types.hpp>
#include <boost/graphs/adjacency_list/storage.hpp>
#include <boost/graphs/adjacency_list/policy.hpp>

// I think that if we concept-ize the internals of the adjacency list, then
// we can filter the external interface. This would be a great place to use
// constrained members.
//
// requires AssociativeStore<vertex_store> pair<...> insert_vertex();

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * The adjacency_list template is the basic implementation of an adjacency
 * list graph structure. The adjacency list stores sets of both vertices and
 * edges, with each vertex being associated with a set of incident edges.
 *
 * The adjacency list actually inherits most of its storage facilities from
 * the vertex and edge storage types. These classes expose public interfaces
 * for specific forms of vertex and edge management.
 */
template <
    template <typename, typename, typename, typename, template <typename> class > class Type,
    typename VertexProps = none,
    typename EdgeProps = none,
    template <typename> class VertexStore = vertex_vector,
    template <typename> class EdgeStore = edge_vector,
    template <typename> class VertexEdgeStore = vertex_edge_list,
    template <typename> class AllowEdgePolicy = allow_loops_policy
>
class adjacency_list
    : public VertexStore<
            typename Type< // Instantiate the type over dummy stores
                    VertexProps,
                    EdgeProps,
                    VertexStore<none>,
                    EdgeStore<none>,
                    VertexEdgeStore
                >::vertex_type
        >
    , public EdgeStore<
            typename Type< // Instantiate the type over dummy stores
                    VertexProps,
                    EdgeProps,
                    VertexStore<none>,
                    EdgeStore<none>,
                    VertexEdgeStore
                >::edge_type
        >
{
    typedef Type<
            VertexProps, EdgeProps, VertexStore<none>, EdgeStore<none>, VertexEdgeStore
        > graph_type;

public:
    typedef VertexStore<typename graph_type::vertex_type> vertex_store;
    typedef EdgeStore<typename graph_type::edge_type> edge_store;

    typedef typename vertex_store::vertex_type vertex_type;
    typedef typename vertex_store::vertex_descriptor vertex_descriptor;
    typedef typename vertex_type::properties_type vertex_properties;

    typedef typename edge_store::edge_type edge_type;
    typedef typename edge_store::edge_descriptor edge_descriptor;
    typedef typename edge_type::properties_type edge_properties;

    adjacency_list();

    // Add edge
    // requires HasAdd<vertex_store>
    edge_descriptor add_edge(vertex_descriptor u, vertex_descriptor v);
    edge_descriptor add_edge(vertex_descriptor u, vertex_descriptor v, edge_properties const& ep);

    // requires HasAdd<vertex_store> && UniqueComparableVertex<vertex_type>
    edge_descriptor add_edge(vertex_properties const& u, vertex_properties const& v);
    edge_descriptor add_edge(vertex_properties const& u, vertex_properties const& v, edge_properties const& ep);

    // requires HasAdd<vertex_store> && UniqueMappedVertex<vertex_type> &&
    //          SameType<Key, typename vertex_store::key_type>
    template <typename Key> edge_descriptor add_edge(Key const& u, Key const& v);
    template <typename Key> edge_descriptor add_edge(Key const& u, Key const& v, edge_properties const& ep);


    // Insert edge (do we need these functions? - descriptors are evaluable).
    // requires HasInsert<vertex_store>
    std::pair<edge_descriptor, bool> insert_edge(vertex_descriptor u, vertex_descriptor v);
    std::pair<edge_descriptor, bool> insert_edge(vertex_descriptor u, vertex_descriptor v, edge_properties const& ep);

    // Vertex disconnect
    // Requires HasRemove<edge_store> && ???
    void disconnect_vertex(vertex_descriptor v);

    // Vertex removal
    // requires HasRemove<vertex_store>
    void remove_vertex(vertex_descriptor v);

    // Edge removal
    // requires HasRemove<edge_store>
    void remove_edge(edge_descriptor e);
    void remove_edge(vertex_descriptor u, vertex_descriptor v);


    // Descriptor accessor. Use these functions to get descriptors...
    vertex_descriptor descriptor(vertex_type const& v) const;
    edge_descriptor descriptor(edge_type const& e) const;

    // Edge and vertex accessors
    vertex_properties& properties(vertex_descriptor v);
    vertex_properties const& properties(vertex_descriptor v) const;
    edge_properties& properties(edge_descriptor e);
    edge_properties const& properties(edge_descriptor e) const;

};

// Functions

#define BOOST_GRAPH_ADJLIST_PARAMS \
    template <typename, typename, typename, typename, template <typename> class> class T, \
    typename VP, \
    typename EP, \
    template <typename> class VS, \
    template <typename> class ES, \
    template <typename> class VES, \
    template <typename> class AEP

/**
 * Add a vertex to the graph with no or default properties.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::adjacency_list()
    : vertex_store()
    , edge_store()
{ }

/**
 * Add a vertex to the graph with no or default properties.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
typename adjacency_list<T,VP,EP,VS,ES,VES,AEP>::edge_descriptor
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::add_edge(
        vertex_descriptor u,
        vertex_descriptor v)
{
    return add_edge(u, v, edge_properties());
}

/**
 * Add a vertex to the graph with the given properties. The edge will not be
 * added (no action taken) if the endpoints form a loop.
 *
 * @todo What if the user intentially passes null vertices? We should add a
 * loose or unconnected edge... but how does one actually manage these things?
 * For now, operate on the assumption that the vertices are valid.
 *
 * @todo Similar to above, how do we want to handle loops. It's currently built
 * as a simple boolean value that either allows the addition or not.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
typename adjacency_list<T,VP,EP,VS,ES,VES,AEP>::edge_descriptor
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::add_edge(
        vertex_descriptor u,
        vertex_descriptor v,
        edge_properties const& ep)
{
    typedef adjacency_list<T,VP,EP,VS,ES,VES,AEP> this_type;

    edge_descriptor e;
    if(AEP<this_type>::allow(u, v, ep)) {
        e = edge_store::add_edge(u, v, ep);

        this->vertex(u).connect_to(e);
        this->vertex(v).connect_from(e);
    }

    return e;
}

/**
 * Add an edge that connects the two vertices identified by the given
 * properties.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
typename adjacency_list<T,VP,EP,VS,ES,VES,AEP>::edge_descriptor
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::add_edge(
        vertex_properties const& up,
        vertex_properties const& vp)
{
    return add_edge(up, vp, edge_properties());
}

/**
 * Add an edge that connects the two vertices identified by the given vertex
 * properties, such that it will contain the have edge properties.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
typename adjacency_list<T,VP,EP,VS,ES,VES,AEP>::edge_descriptor
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::add_edge(
        vertex_properties const& up,
        vertex_properties const& vp,
        edge_properties const& ep)
{
    // Start by finding the vertices according to their properties.
    vertex_descriptor u = find_vertex(up);
    vertex_descriptor v = find_vertex(vp);
    BOOST_ASSERT(u.is_valid() && v.is_valid());
    return add_edge(u, v, ep);
}

/**
 * Add an edge that connects the two vertices identified by their keys.
 *
 * @todo Without constrained members, instantiating this function can result in
 * ambigous member functions with the vertex property add_edge() functions if
 * the key type is the same as the vertex properties. We can probably also get
 * rid of the Key template parameter and simply reference the maps key type
 * here.
 *
 * @todo You have to explicitly give the key parameter here because of the
 * other add_vertex() functions.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
template <typename Key>
typename adjacency_list<T,VP,EP,VS,ES,VES,AEP>::edge_descriptor
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::add_edge(
        Key const& up,
        Key const& vp)
{
    return add_edge<Key>(up, vp, edge_properties());
}

template <BOOST_GRAPH_ADJLIST_PARAMS>
template <typename Key>
typename adjacency_list<T,VP,EP,VS,ES,VES,AEP>::edge_descriptor
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::add_edge(
        Key const& up,
        Key const& vp,
        edge_properties const& ep)
{
    // Start by finding the vertices according to their properties.
    vertex_descriptor u = find_vertex(up);
    vertex_descriptor v = find_vertex(vp);
    BOOST_ASSERT(u.is_valid() && v.is_valid());
    return add_edge(u, v, ep);
}


/**
 * Add an edge that connects the given properties.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
std::pair<typename adjacency_list<T,VP,EP,VS,ES,VES,AEP>::edge_descriptor, bool>
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::insert_edge(
        vertex_descriptor u,
        vertex_descriptor v)
{
    return insert_edge(u, v, edge_properties());
}

/**
 * Connect the vertex u to v and set the corresponding edge properties to ep.
 * Return a pair containing an edge descriptor and a boolean value that
 * indicates whether or not the edge was added. Note that if the graph does
 * not allow self loops or loose edges, the edge descriptor will be null.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
std::pair<typename adjacency_list<T,VP,EP,VS,ES,VES,AEP>::edge_descriptor, bool>
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::insert_edge(
        vertex_descriptor u,
        vertex_descriptor v,
        edge_properties const& ep)
{
    typedef adjacency_list<T,VP,EP,VS,ES,VES,AEP> this_type;

    std::pair<edge_descriptor, bool> ret(edge_descriptor(), false);
    if(AEP<this_type>::allow(u, v, ep)) {
        ret = edge_store::insert_edge(u, v, ep);
        if(ret.second) {
            this->vertex(u).connect_to(ret.first);
            this->vertex(v).connect_from(ret.first);
        }
    }
}

/**
 * Disconnect this vertex from all others (remove all incident edges). This
 * is typically a precursor operation for vertex removal to ensure that no
 * dangling edges are left.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
void
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::disconnect_vertex(vertex_descriptor v)
{
    BOOST_ASSERT(v.is_valid());

    // Optimal implementation depends on the vertex structure.
    disconnect(*this, this->vertex(v), typename graph_type::tag());
}

/**
 * Remove the given vertex from the graph. Note that this does not disconnect
 * edges from the vertex before removing it. Removing a vertex connected to
 * the graph can result in dangling edges.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
void
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::remove_vertex(vertex_descriptor v)
{
}

/**
 * Remove the given edge from the graph.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
void
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::remove_edge(edge_descriptor e)
{
    vertex_descriptor u = this->edge(e).source();
    vertex_descriptor v = this->edge(e).target();
    BOOST_ASSERT(u.is_valid() && v.is_valid());

    this->vertex(u).disconnect_to(e);
    this->vertex(v).disconnect_from(e);
    edge_store::remove_edge(e);
}

/**
 * Remove all edges connecting vertices u and v from the graph.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
void
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::remove_edge(
        vertex_descriptor u,
        vertex_descriptor v)
{
    // I suppose it would be a good idea to ensure that the local edges of
    // u and v are actually removed also.
    edge_store::remove_edge(u, v);
}

/**
 * Get a descriptor for the given vertex.
 *
 * This may seem like a strange function, but there's currently no simple way
 * to transform between the two types.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
typename adjacency_list<T,VP,EP,VS,ES,VES,AEP>::vertex_descriptor
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::descriptor(vertex_type const& v) const
{
    typedef typename vertex_store::vertex_type stored_type;
    return vertex_store::descriptor(static_cast<stored_type const&>(v));
}

/**
 * Get vertex properties for the given vertex descriptor.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
typename adjacency_list<T,VP,EP,VS,ES,VES,AEP>::vertex_properties&
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::properties(vertex_descriptor v)
{
    return vertex_store::properties(v);
}

/**
 * Get vertex properties for the given vertex descriptor.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
typename adjacency_list<T,VP,EP,VS,ES,VES,AEP>::vertex_properties const&
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::properties(vertex_descriptor v) const
{
    return vertex_store::properties(v);
}

/**
 * Get edge properties for the given edge descriptor.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
typename adjacency_list<T,VP,EP,VS,ES,VES,AEP>::edge_properties&
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::properties(edge_descriptor e)
{
    return edge_store::properties(e);
}

/**
 * Get edge properties for the given edge descriptor.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
typename adjacency_list<T,VP,EP,VS,ES,VES,AEP>::edge_properties const&
adjacency_list<T,VP,EP,VS,ES,VES,AEP>::properties(edge_descriptor e) const
{
    return edge_store::properties(e);
}

#undef BOOST_GRAPH_ADJLIST_PARAMS

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

// Include a number common graph types.
#include <boost/graphs/adjacency_list/graphs.hpp>

#endif

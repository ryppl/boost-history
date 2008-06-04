
#ifndef BOOST_GRAPH_ADJACENCY_LIST_HPP
#define BOOST_GRAPH_ADJACENCY_LIST_HPP

#include <boost/assert.hpp>

#include <boost/graphs/utility.hpp>
#include <boost/graphs/adjacency_list/types.hpp>
#include <boost/graphs/adjacency_list/storage.hpp>
#include <boost/graphs/adjacency_list/policy.hpp>
#include <boost/graphs/adjacency_list/adjacency_iterator.hpp>

// I think that if we concept-ize the internals of the adjacency list, then
// we can filter the external interface. This would be a great place to use
// constrained members.
//
// requires AssociativeStore<vertex_store> pair<...> insert_vertex();

namespace boost {
namespace graphs {
namespace adj_list {

/**
 */
template <
    typename Type = none,
    typename VertexProps = none,
    typename EdgeProps = none,
    typename VertexStore = none,
    typename EdgeStore = none,
    typename IncidenceStore = none,
    typename AllowableEdgePolicy = none
>
class adjacency_list
{
public:
    typedef VertexProps vertex_properties;
    typedef typename VertexStore::descriptor_type vertex_descriptor;

    typedef EdgeProps edge_properties;
    typedef typename EdgeStore::descriptor_type edge_descriptor;
    
    typedef typename IncidenceStore::template store<edge_descriptor>::type incidence_store;
    
    typedef typename Type::template vertex<vertex_properties, vertex_descriptor, incidence_store>::type vertex_type;
    typedef typename Type::template edge<edge_properties, edge_descriptor, vertex_descriptor>::type edge_type;

    // Note that vertex_type != vertex_store::vertex_type
    typedef typename VertexStore::template store<vertex_type>::type vertex_store;
    typedef typename EdgeStore::template store<edge_type>::type edge_store;


    adjacency_list();

private:
    vertex_store _verts;
    edge_store _edges;
};

// Functions

#define BOOST_GRAPHS_AL_PARAMS \
    typename T, typename VP, typename EP, typename VS, typename ES, typename IS, typename AEP

/**
 * Construct an empty adjacency list.
 */
template <BOOST_GRAPHS_AL_PARAMS>
adjacency_list<T,VP,EP,VS,ES,IS,AEP>::adjacency_list()
    : _verts()
    , _edges()
{ }

#undef BOOST_GRAPHS_ADJLIST_PARAMS

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

// Include a number common graph types.
// #include <boost/graphs/adjacency_list/graphs.hpp>

#endif

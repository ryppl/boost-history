
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
    typename Direction = none,
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
    typedef typename VertexStore::descriptor_type vertex_descriptor;
    typedef typename EdgeStore::descriptor_type edge_descriptor;

    adjacency_list();
};

// Functions

#define BOOST_GRAPH_ADJLIST_PARAMS \
    typename D, typename VP, typename EP, typename VS, typename ES, typename IS, typename AEP

/**
 * Construct an empty adjacency list.
 */
template <BOOST_GRAPH_ADJLIST_PARAMS>
adjacency_list<D,VP,EP,VS,ES,OS,AEP>::adjacency_list()
{ }

#undef BOOST_GRAPH_ADJLIST_PARAMS

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */

// Include a number common graph types.
// #include <boost/graphs/adjacency_list/graphs.hpp>

#endif

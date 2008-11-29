
#ifndef BOOST_GRAPHS_ADJLIST_INCIDENCE_STORE_HPP
#define BOOST_GRAPHS_ADJLIST_INCIDENCE_STORE_HPP

#include <vector>

// Pull all of the incidence list type selectors
#include <boost/graphs/adjacency_list/incs/vector.hpp>
#include <boost/graphs/adjacency_list/incs/list.hpp>
#include <boost/graphs/adjacency_list/incs/set.hpp>

// The incidence store provides a means of selecting different kinds of storage
// mechanisms for the list of incident edges on undirected vertices. An
// incidence store simply contains the edge descriptors that are incident to
// a vertex - nothing more.

// The incidence store should be more appropriately called the adjacency store,
// but we're talking about incident edges, not adjacent vertices.

namespace boost { namespace graphs { namespace adjacency_list {

template <typename Store>
struct incidence_store_traits
{
    typedef typename Store::size_type incident_edges_size_type;
};

namespace incs {

/** Insert an edge descriptor into the incidence store of the vertex. */
template <typename Store, typename Edge>
typename Store::iterator
insert(Store& store, Edge e)
{ return container_insert(store, e); }

/** Return the size of an adjacency list for the given vertex, its degree. */
template <typename Store>
inline typename incidence_store_traits<Store>::incident_edges_size_type
size(Store const& store)
{ return store.size(); }

} /* namespace incs */

} } } /* namespace boost::graphs::adjacency_list */

#endif



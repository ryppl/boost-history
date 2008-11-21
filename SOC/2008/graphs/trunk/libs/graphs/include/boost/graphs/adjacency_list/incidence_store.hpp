
#ifndef BOOST_GRAPHS_ADJLIST_INCIDENCE_STORE_HPP
#define BOOST_GRAPHS_ADJLIST_INCIDENCE_STORE_HPP

#include <vector>

// The incidence store provides a means of selecting different kinds of storage
// mechanisms for the list of incident edges on undirected vertices. An
// incidence store simply contains the edge descriptors that are incident to
// a vertex - nothing more.

namespace boost { namespace graphs { namespace adjacency_list {

namespace ies
{

/** Return the size of an adjacency list for the given vertex. */
template <typename Store>
inline typename Store::size_type size(Store const& store)
{ return store.size(); }

}

} } }

// Pull all of the incidence list type selectors
#include <boost/graphs/adjacency_list/ies/vector.hpp>
#include <boost/graphs/adjacency_list/ies/list.hpp>
#include <boost/graphs/adjacency_list/ies/set.hpp>

#endif



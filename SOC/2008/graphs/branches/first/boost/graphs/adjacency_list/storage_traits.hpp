
#ifndef BOOST_GRAPHS_ADJACENCY_LIST_STORAGE_TRAITS_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_STORAGE_TRAITS_HPP

#include <boost/graphs/adjacency_list/descriptor.hpp>

namespace boost {
namespace graphs {
namespace adj_list {

/**
 * This is primarilly used to decouple graph-related types from the underlying
 * storage implementations. The default option is to simply use a void pointer
 * as an opaque reference to objects in graphs. Clearly, pointers aren't the
 * most useful opaque references for all types since memory can come and go
 * as storage memory is reallocated (e.g., vector).
 *
 * @todo Obviously, this will become a concept in the not-so distant future.
 */
template <typename Store>
struct storage_traits
{
    typedef void* descriptor_type;
};

// Quick note on unordered_* containers...
// Apparently, insertions don't invalidate iterators (i.e., allocated memory)
// so we can use pointers for those stores also. This is probably due to the
// use of lists as the bucket structure of the underlying hash tables.
// I hope...

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */


#endif


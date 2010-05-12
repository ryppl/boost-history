#ifndef BOOST_METAGRAPH_MPL_GRAPH_SEARCH_STATE_AND_OPERATIONS_HPP_INCLUDED
#define BOOST_METAGRAPH_MPL_GRAPH_SEARCH_STATE_AND_OPERATIONS_HPP_INCLUDED

namespace boost {
namespace metagraph {
namespace mpl_graph {


// as with all mpl algorithms, state must be carried along
// because metadata is immutable.  so we do something like the mpl inserter pattern, 
// only there are many operations, which we stick in a blob (? not sure how to avoid this)
template<typename State, typename Operations>
struct search_state_and_operations {
    typedef State state;
    typedef Operations operations;
};


} // namespace mpl_graph
} // namespace metagraph
} // namespace boost


#endif // BOOST_METAGRAPH_MPL_GRAPH_SEARCH_STATE_AND_OPERATIONS_HPP_INCLUDED

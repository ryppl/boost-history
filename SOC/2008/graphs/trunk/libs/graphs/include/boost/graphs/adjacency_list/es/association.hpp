
#ifndef BOOST_GRAPHS_ADJLIST_ES_ASSOCIATION_HPP
#define BOOST_GRAPHS_ADJLIST_ES_ASSOCIATION_HPP

namespace boost { namespace graphs {

// Specializations to support labels for undirected edges for edge vectors,
// and lists.

template <typename VertexDesc, typename EdgeLabel>
struct label_traits<std::pair<std::pair<VertexDesc, VertexDesc> const, EdgeLabel>>
{
    typedef EdgeLabel label_type;
};

template <typename VertexDesc, typename EdgeLabel>
inline EdgeLabel&
label(std::pair<std::pair<VertexDesc, VertexDesc> const, EdgeLabel>& edge)
{ return edge.second; }

template <typename VertexDesc, typename EdgeLabel>
inline EdgeLabel const&
label(std::pair<std::pair<VertexDesc, VertexDesc> const, EdgeLabel> const& edge)
{ return edge.second; }

// Specializations of the edge interface for edge sequences (vectors and lists).

// NOTE: I'm un-consting the key type for an associative container. Will this
// cause problems later? Possibly.
template <typename VertexDesc, typename EdgeLabel>
struct edge_traits<std::pair<std::pair<VertexDesc, VertexDesc> const, EdgeLabel>>
{
    typedef std::pair<VertexDesc, VertexDesc> end_pair;
};

} } /* namespace boost::graphs */

#endif


#ifndef BOOST_GRAPHS_ADJLIST_ES_SEQUENCE_HPP
#define BOOST_GRAPHS_ADJLIST_ES_SEQUENCE_HPP

namespace boost { namespace graphs {

// Specializations to support labels for undirected edges for edge vectors,
// and lists.

// TODO: I'm a little worried about the possibility of type collisions. I may
// be required to introduce a tagging system to label and edge traits to help
// distinguish the intent of the specialization.

template <typename VertexDesc, typename EdgeLabel>
struct label_traits<std::pair<std::pair<VertexDesc, VertexDesc>, EdgeLabel>>
{
    typedef EdgeLabel label_type;
};

template <typename VertexDesc, typename EdgeLabel>
inline EdgeLabel&
label(std::pair<std::pair<VertexDesc, VertexDesc>, EdgeLabel>& edge)
{ return edge.second; }

template <typename VertexDesc, typename EdgeLabel>
inline EdgeLabel const&
label(std::pair<std::pair<VertexDesc, VertexDesc>, EdgeLabel> const& edge)
{ return edge.second; }

// Specializations of the edge interface for edge sequences (vectors and lists).

template <typename VertexDesc, typename EdgeLabel>
struct edge_traits<std::pair<std::pair<VertexDesc, VertexDesc>, EdgeLabel>>
{
    typedef VertexDesc vertex_descriptor;
    typedef std::pair<VertexDesc, VertexDesc> edge_ends;
};

} } /* namespace boost::graphs */

template <typename VertexDesc, typename EdgeLabel>
inline EdgeLabel const&
ends(std::pair<std::pair<VertexDesc, VertexDesc>, EdgeLabel> const& edge)
{ return edge.first; }

template <typename VertexDesc, typename EdgeLabel>
inline VertexDesc
first(std::pair<std::pair<VertexDesc, VertexDesc> const, EdgeLabel> const& edge)
{ return edge.first.first; }

template <typename VertexDesc, typename EdgeLabel>
inline VertexDesc
second(std::pair<std::pair<VertexDesc, VertexDesc> const, EdgeLabel> const& edge)
{ return edge.first.second; }

template <typename VertexDesc, typename EdgeLabel>
inline VertexDesc
oppposite(std::pair<std::pair<VertexDesc, VertexDesc> const, EdgeLabel> const& edge, VertexDesc which)
{ return which == first(edge) ? second(edge) : first(edge); }

#endif

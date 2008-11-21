
#ifndef BOOST_GRAPHS_ADJLIST_ES_SEQUENCE_HPP
#define BOOST_GRAPHS_ADJLIST_ES_SEQUENCE_HPP

namespace boost { namespace graphs {

// Specializations to support labels for undirected edges for edge vectors,
// and lists.

// TODO: I'm a little worried about the possibility of type collisions with
// directed graphs. It would be nice if I could specialize on an extra piece
// of information here. Also, the difference between this and associative
// specializations is the const-ness of the ends.

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
    typedef std::pair<VertexDesc, VertexDesc> end_pair;
};

} } /* namespace boost::graphs */

#endif


#ifndef BOOST_GRAPHS_ADJACENCY_LIST_UNDIRECTED_HPP
#define BOOST_GRAPHS_ADJACENCY_LIST_UNDIRECTED_HPP

namespace boost {
namespace graphs {
namespace adj_list {

// Forward declarations
template <typename VP, typename VD, typename IS> struct undirected_vertex;
template <typename EP, typename ED, typename VD> struct undirected_edge;

struct undirected 
{
    template <typename VertexProps, typename VertexDesc, typename IncidenceStore>
    struct vertex
    {
        typedef undirected_vertex<VertexProps, VertexDesc, IncidenceStore> type;
    };

    template <typename EdgeProps, typename EdgeDesc, typename VertexDesc>
    struct edge
    {
        typedef undirected_edge<EdgeProps, EdgeDesc, VertexDesc> type;
    };
};

template <typename VertexProps, typename VertexDesc, typename IncidenceStore>
struct undirected_vertex
{
    typedef VertexProps vertex_properties;
    typedef VertexDesc vertex_descriptor;
};

template <typename EdgeProps, typename EdgeDesc, typename VertexDesc>
struct undirected_edge
{
    typedef EdgeProps edge_properties;
    typedef EdgeDesc edge_descriptor;
    typedef VertexDesc vertex_descriptor;
};

} /* namespace adj_list */
} /* namespace graphs */
} /* namespace boost */


#endif

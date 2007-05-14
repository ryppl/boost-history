#ifndef BOOST_GRAPH_UNDIRECTED_GRAPH_HPP
#define BOOST_GRAPH_UNDIRECTED_GRAPH_HPP

#include <boost/graph/adjacency_list.hpp>

namespace boost
{
    namespace graph
    {
	template <
	    typename VertexProperty = no_property,
	    typename EdgeProperty = no_property,
	    typename GraphProperty = no_property
	    >
	class undirected_graph
	{
	private:
	    typedef adjacency_list<vecS,
				   vecS,
				   undirectedS,
				   VertexProperty,
				   EdgeProperty,
				   GraphProperty,
				   listS> graph_type;
	    graph_type m_graph;

	public:
	    // types for properties and bundling
	    typedef typename graph_type::graph_property_type graph_property_type;
	    typedef typename graph_type::vertex_property_type vertex_property_type;
	    typedef typename graph_type::edge_property_type edge_property_type;
	    typedef typename graph_type::vertex_bundled vertex_bundled;
	    typedef typename graph_type::edge_bundled edge_bundled;

	    // more commonly used graph types
	    typedef typename graph_type::stored_vertex stored_vertex;
	    typedef typename graph_type::vertices_size_type vertices_size_type;
	    typedef typename graph_type::edges_size_type edges_size_type;
	    typedef typename graph_type::degree_size_type degree_size_type;
	    typedef typename graph_type::vertex_descriptor vertex_descriptor;
	    typedef typename graph_type::edge_descriptor edge_descriptor;

	    // storage selectors
	    typedef typename graph_type::vertex_list_selector vertex_list_selector;
	    typedef typename graph_type::edge_list_selector edge_list_selector;
	    typedef typename graph_type::out_edge_list_selector out_edge_list_selector;
	    typedef typename graph_type::directed_selector directed_selector;

	    // iterator types
	    typedef typename graph_type::vertex_iterator vertex_iterator;
	    typedef typename graph_type::edge_iterator edge_iterator;
	    typedef typename graph_type::out_edge_iterator out_edge_iterator;
	    typedef typename graph_type::in_edge_iterator in_edge_iterator;
	    typedef typename graph_type::adjacency_iterator adjacency_iterator;

	    // miscellaneous types
	    typedef typename graph_type::directed_category directed_category;
	    typedef typename graph_type::edge_parallel_category edge_parallel_category;
	    typedef typename graph_type::traversal_category traversal_category;

	    inline undirected_graph(const GraphProperty& p = GraphProperty())
		: m_graph(p)
	    {}

	    inline undirected_graph(const undirected_graph& x)
		: m_graph(x)
	    {}

	    inline undirected_graph(vertices_size_type n,
				    const GraphProperty& p = GraphProperty())
		: m_graph(n, p)
	    {}

	    inline graph_type& impl()
	    { return m_graph; }

	    inline const graph_type &impl() const
	    { return m_graph; }


	    // VertexListGraph concepts
	    static inline vertex_descriptor null_vertex()
	    { return graph_type::null_vertex(); }

	    inline vertices_size_type num_vertices() const
	    { return boost::num_vertices(m_graph); }

	    inline std::pair<vertex_iterator, vertex_iterator> vertices() const
	    { return boost::vertices(m_graph); }

	    // EdgeListGraph concepts
	    inline edges_size_type num_edges() const
	    { return boost::num_edges(m_graph); }

	    inline std::pair<edge_iterator, edge_iterator> edges() const
	    { return boost::edges(m_graph); }

	    // MutableGraph concepts
	    inline vertex_descriptor add_vertex()
	    { return boost::add_vertex(m_graph); }

	    inline void clear_vertex(vertex_descriptor v)
	    { boost::clear_vertex(v, m_graph); }

	    inline void remove_vertex(vertex_descriptor v)
	    { boost::remove_vertex(v, m_graph); }

	    inline std::pair<edge_descriptor, bool> add_edge(vertex_descriptor u, vertex_descriptor v)
	    { return boost::add_edge(u, v, m_graph); }

	    inline void remove_edge(vertex_descriptor u, vertex_descriptor v)
	    { boost::remove_edge(u, v, m_graph); }

	    inline void remove_edge(edge_descriptor e)
	    { boost::remove_edge(e, m_graph); }

	    inline void remove_edge(edge_iterator i)
	    { boost::remove_edge(i, m_graph); }
	};


	// VertexListGraph concepts
	template <class VP, class EP, class GP>
	inline
	typename undirected_graph<VP,EP,GP>::vertices_size_type
	num_vertices(const undirected_graph<VP,EP,GP>& g)
	{
	    return g.num_vertices();
	}

	template <class VP, class EP, class GP>
	inline
	std::pair<
	    typename undirected_graph<VP,EP,GP>::vertex_iterator,
	    typename undirected_graph<VP,EP,GP>::vertex_iterator>
	vertices(const undirected_graph<VP,EP,GP>& g)
	{
	    return g.vertices();
	}

	// EdgeListGraph concepts
	template <class VP, class EP, class GP>
	inline
	typename undirected_graph<VP,EP,GP>::edges_size_type
	num_edges(const undirected_graph<VP,EP,GP>& g)
	{
	    return g.num_edges();
	}

	template <class VP, class EP, class GP>
	inline
	std::pair<
	    typename undirected_graph<VP,EP,GP>::edge_iterator,
	    typename undirected_graph<VP,EP,GP>::edge_iterator>
	edges(const undirected_graph<VP,EP,GP>& g)
	{
	    return g.edges();
	}


	// MutableGraph concepts
	template <class VP, class EP, class GP>
	inline typename undirected_graph<VP,EP,GP>::vertex_descriptor
	add_vertex(undirected_graph<VP,EP,GP> &g)
	{
	    return g.add_vertex();
	}


	template <class VP, class EP, class GP>
	inline void
	clear_vertex(
	    undirected_graph<VP,EP,GP> &g,
	    typename undirected_graph<VP,EP,GP>::vertex_descriptor u)
	{
	    return g.clear_vertex(u);
	}


	template <class VP, class EP, class GP>
	inline void
	remove_vertex(
	    undirected_graph<VP,EP,GP> &g,
	    typename undirected_graph<VP,EP,GP>::vertex_descriptor u)
	{
	    return g.remove_vertex(u);
	}



	template <class VP, class EP, class GP>
	inline std::pair<typename undirected_graph<VP,EP,GP>::edge_descriptor, bool>
	add_edge(
	    undirected_graph<VP,EP,GP> &g,
	    typename undirected_graph<VP,EP,GP>::vertex_descriptor u,
	    typename undirected_graph<VP,EP,GP>::vertex_descriptor v)
	{
	    return g.add_edge(u, v);
	}


	template <class VP, class EP, class GP>
	inline void
	remove_edge(
	    undirected_graph<VP,EP,GP> &g,
	    typename undirected_graph<VP,EP,GP>::vertex_descriptor u,
	    typename undirected_graph<VP,EP,GP>::vertex_descriptor v)
	{
	    return g.remove_edge(u, v);
	}


	template <class VP, class EP, class GP>
	inline void
	remove_edge(
	    undirected_graph<VP,EP,GP> &g,
	    typename undirected_graph<VP,EP,GP>::edge_descriptor e)
	{
	    return g.remove_edge(e);
	}


	template <class VP, class EP, class GP>
	inline void
	remove_edge(
	    undirected_graph<VP,EP,GP> &g,
	    typename undirected_graph<VP,EP,GP>::edge_iterator i)
	{
	    return g.remove_edge(i);
	}
    }
}

#endif

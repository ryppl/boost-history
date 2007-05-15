#ifndef BOOST_GRAPH_UNDIRECTED_GRAPH_HPP
#define BOOST_GRAPH_UNDIRECTED_GRAPH_HPP

#include <boost/graph/adjacency_list.hpp>

namespace boost
{
    template <
	typename VertexProperty = no_property,
	typename EdgeProperty = no_property,
	typename GraphProperty = no_property
	>
    class undirected_graph
    {
    public:
	typedef adjacency_list<vecS,
			       vecS,
			       undirectedS,
			       VertexProperty,
			       EdgeProperty,
			       GraphProperty,
			       listS> type;
    private:
	type m_graph;

    public:
	typedef typename type::graph_tag graph_tag;

	// types for properties and bundling
	typedef typename type::graph_property_type graph_property_type;
	typedef typename type::vertex_property_type vertex_property_type;
	typedef typename type::edge_property_type edge_property_type;
	typedef typename type::vertex_bundled vertex_bundled;
	typedef typename type::edge_bundled edge_bundled;

	// more commonly used graph types
	typedef typename type::stored_vertex stored_vertex;
	typedef typename type::vertices_size_type vertices_size_type;
	typedef typename type::edges_size_type edges_size_type;
	typedef typename type::degree_size_type degree_size_type;
	typedef typename type::vertex_descriptor vertex_descriptor;
	typedef typename type::edge_descriptor edge_descriptor;

	// storage selectors
	typedef typename type::vertex_list_selector vertex_list_selector;
	typedef typename type::edge_list_selector edge_list_selector;
	typedef typename type::out_edge_list_selector out_edge_list_selector;
	typedef typename type::directed_selector directed_selector;

	// iterator types
	typedef typename type::vertex_iterator vertex_iterator;
	typedef typename type::edge_iterator edge_iterator;
	typedef typename type::out_edge_iterator out_edge_iterator;
	typedef typename type::in_edge_iterator in_edge_iterator;
	typedef typename type::adjacency_iterator adjacency_iterator;

	// miscellaneous types
	typedef typename type::directed_category directed_category;
	typedef typename type::edge_parallel_category edge_parallel_category;
	typedef typename type::traversal_category traversal_category;

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

	// Graph concepts
	static inline vertex_descriptor null_vertex()
	{ return type::null_vertex(); }

	// IncidenceGraph concepts
	vertex_descriptor source(edge_descriptor e) const
	{ return boost::source(e, m_graph); }

	vertex_descriptor target(edge_descriptor e) const
	{ return boost::target(e, m_graph); }

	inline degree_size_type out_degree(vertex_descriptor v) const
	{ return boost::out_degree(v, m_graph); }

	inline std::pair<out_edge_iterator, out_edge_iterator> out_edges(vertex_descriptor v) const
	{ return boost::out_edges(v, m_graph); }

	// BidirectionalGraph concepts
	inline degree_size_type in_degree(vertex_descriptor v) const
	{ return boost::in_degree(v, m_graph); }

	inline std::pair<in_edge_iterator, in_edge_iterator> in_edges(vertex_descriptor v) const
	{ return boost::in_edges(v, m_graph); }

	inline degree_size_type degree(vertex_descriptor v)
	{ return boost::degree(v, m_graph); }

	// VertexListGraph concepts
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

	template <typename Predicate>
	inline void remove_edge_if(Predicate p)
	{ boost::remove_edge_if(p, m_graph); }

	template <typename Predicate>
	inline void remove_out_edge_if(vertex_descriptor v, Predicate p)
	{ boost::remove_out_edge_if(v, p, m_graph); }

	template <typename Predicate>
	inline void remove_in_edge_if(vertex_descriptor v, Predicate p)
	{ boost::remove_in_edge_if(v, p, m_graph); }

	// PropertyGraph concepts
	template <typename Property>
	inline typename property_map<type, Property>::type
	get(Property p)
	{ return boost::get(p, m_graph); }

	template <typename Property>
	inline typename property_map<type, Property>::const_type
	get(Property p) const
	{ return boost::get(p, m_graph); }

	template <typename Property, typename Key>
	inline typename property_traits<typename property_map<type, Property>::const_type>::value_type
	get(Property p, const Key& k)
	{ return boost::get(p, m_graph, k); }

	template <typename Property, typename Key, typename Value>
	inline void
	put(Property p, const Key& k, const Value& v)
	{ boost::put(p, m_graph, k, v); }

	// MutablePropertyGraph concepts
	template <typename Property>
	inline vertex_descriptor add_vertex(const Property& vp)
	{ return boost::add_vertex(vp, m_graph); }

	template <typename Property>
	inline std::pair<edge_descriptor, bool>
	add_edge(vertex_descriptor u, vertex_descriptor v, const Property& ep)
	{ return boost::add_edge(u, v, ep, m_graph); }
    };

    // IncidenceGraph concepts
    template <class VP, class EP, class GP>
    inline typename undirected_graph<VP,EP,GP>::vertex_descriptor
    source(typename undirected_graph<VP,EP,GP>::edge_descriptor e,
	   const undirected_graph<VP,EP,GP> &g)
    {
	return g.source(e);
    }

    template <class VP, class EP, class GP>
    inline typename undirected_graph<VP,EP,GP>::vertex_descriptor
    target(typename undirected_graph<VP,EP,GP>::edge_descriptor e,
	   const undirected_graph<VP,EP,GP> &g)
    {
	return g.target(e);
    }

    template <class VP, class EP, class GP>
    inline typename undirected_graph<VP,EP,GP>::degree_size_type
    out_degree(typename undirected_graph<VP,EP,GP>::vertex_descriptor v,
	       const undirected_graph<VP,EP,GP> &g)
    {
	return g.out_degree(v);
    }

    template <class VP, class EP, class GP>
    inline std::pair<
	typename undirected_graph<VP,EP,GP>::out_edge_iterator,
	typename undirected_graph<VP,EP,GP>::out_edge_iterator
	>
    out_edges(typename undirected_graph<VP,EP,GP>::vertex_descriptor v,
	      const undirected_graph<VP,EP,GP> &g)
    {
	return g.out_edges(v);
    }

    // BidirectionalGraph concepts
    template <class VP, class EP, class GP>
    inline typename undirected_graph<VP,EP,GP>::degree_size_type
    in_degree(const undirected_graph<VP,EP,GP> &g,
	      typename undirected_graph<VP,EP,GP>::vertex_descriptor v)
    {
	return g.in_degree(v);
    }

    template <class VP, class EP, class GP>
    inline typename undirected_graph<VP,EP,GP>::degree_size_type
    degree(typename undirected_graph<VP,EP,GP>::vertex_descriptor v,
	   const undirected_graph<VP,EP,GP> &g)
    {
	return g.degree(v);
    }


    template <class VP, class EP, class GP>
    inline std::pair<
	typename undirected_graph<VP,EP,GP>::in_edge_iterator,
	typename undirected_graph<VP,EP,GP>::in_edge_iterator
	>
    in_edges(typename undirected_graph<VP,EP,GP>::vertex_descriptor v,
	     const undirected_graph<VP,EP,GP> &g)
    {
	return g.in_edges(v);
    }

    // VertexListGraph concepts
    template <class VP, class EP, class GP>
    inline typename undirected_graph<VP,EP,GP>::vertices_size_type
    num_vertices(const undirected_graph<VP,EP,GP>& g)
    {
	return g.num_vertices();
    }

    template <class VP, class EP, class GP>
    inline std::pair<
	typename undirected_graph<VP,EP,GP>::vertex_iterator,
	typename undirected_graph<VP,EP,GP>::vertex_iterator
	>
    vertices(const undirected_graph<VP,EP,GP>& g)
    {
	return g.vertices();
    }

    // EdgeListGraph concepts
    template <class VP, class EP, class GP>
    inline typename undirected_graph<VP,EP,GP>::edges_size_type
    num_edges(const undirected_graph<VP,EP,GP>& g)
    {
	return g.num_edges();
    }

    template <class VP, class EP, class GP>
    inline std::pair<
	typename undirected_graph<VP,EP,GP>::edge_iterator,
	typename undirected_graph<VP,EP,GP>::edge_iterator
	>
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
    clear_vertex(typename undirected_graph<VP,EP,GP>::vertex_descriptor u,
	undirected_graph<VP,EP,GP> &g)
    {
	return g.clear_vertex(u);
    }


    template <class VP, class EP, class GP>
    inline void
    remove_vertex(typename undirected_graph<VP,EP,GP>::vertex_descriptor u,
		  undirected_graph<VP,EP,GP> &g)
    {
	return g.remove_vertex(u);
    }



    template <class VP, class EP, class GP>
    inline std::pair<typename undirected_graph<VP,EP,GP>::edge_descriptor, bool>
    add_edge(typename undirected_graph<VP,EP,GP>::vertex_descriptor u,
	     typename undirected_graph<VP,EP,GP>::vertex_descriptor v,
	     undirected_graph<VP,EP,GP> &g)
    {
	return g.add_edge(u, v);
    }


    template <class VP, class EP, class GP>
    inline void
    remove_edge(typename undirected_graph<VP,EP,GP>::vertex_descriptor u,
		typename undirected_graph<VP,EP,GP>::vertex_descriptor v,
		undirected_graph<VP,EP,GP> &g)
    {
	return g.remove_edge(u, v);
    }


    template <class VP, class EP, class GP>
    inline void
    remove_edge(typename undirected_graph<VP,EP,GP>::edge_descriptor e,
		undirected_graph<VP,EP,GP> &g)
    {
	return g.remove_edge(e);
    }


    template <class VP, class EP, class GP>
    inline void
    remove_edge(typename undirected_graph<VP,EP,GP>::edge_iterator i,
		undirected_graph<VP,EP,GP> &g)
    {
	return g.remove_edge(i);
    }

    template <class VP, class EP, class GP, class Predicate>
    inline void
    remove_edge_if(Predicate pred,
		   undirected_graph<VP,EP,GP> &g)

    {
	return g.remove_edge_if(pred);
    }


    template <class VP, class EP, class GP, class Predicate>
    inline void
    remove_out_edge_if(typename undirected_graph<VP,EP,GP>::vertex_descriptor v,
		       Predicate pred,
		       undirected_graph<VP,EP,GP> &g)
    {
	return g.remove_out_edge_if(v, pred);
    }

    template <class VP, class EP, class GP, class Predicate>
    inline void
    remove_in_edge_if(typename undirected_graph<VP,EP,GP>::vertex_descriptor v,
		      Predicate pred,
		      undirected_graph<VP,EP,GP> &g)
    {
	return g.remove_in_edge_if(v, pred);
    }

    // PropertyGraph concepts
    template <class VP, class EP, class GP, typename Property>
    inline typename property_map<typename undirected_graph<VP,EP,GP>::type, Property>::type
    get(Property p, undirected_graph<VP,EP,GP>& g)
    {
	return g.get(p);
    }

    template <class VP, class EP, class GP, typename Property>
    inline typename property_map<typename undirected_graph<VP,EP,GP>::type, Property>::const_type
    get(Property p, const undirected_graph<VP,EP,GP>& g)
    {
	return g.get(p);
    }

    template <class VP, class EP, class GP, typename Property, typename Key>
    inline typename property_traits<
	typename property_map<typename undirected_graph<VP,EP,GP>::type, Property>::const_type
	>::value_type
    get(Property p, const undirected_graph<VP,EP,GP> &g, const Key& k)
    {
	return g.get(p, k);
    }

    template <class VP, class EP, class GP, typename Property, typename Key, typename Value>
    inline void
    put(Property p, undirected_graph<VP,EP,GP> &g, const Key& k, const Value& v)
    {
	g.put(p, k, v);
    }

    // MutablePropertyGraph concepts
    template <class VP, class EP, class GP, typename Property>
    inline typename undirected_graph<VP,EP,GP>::vertex_descriptor
    add_vertex(const Property& vp, undirected_graph<VP,EP,GP>& g)
    {
	return g.add_vertex(vp);
    }

    template <class VP, class EP, class GP, typename Property>
    inline std::pair<typename undirected_graph<VP,EP,GP>::edge_descriptor, bool>
    add_edge(typename undirected_graph<VP,EP,GP>::vertex_descriptor u,
	     typename undirected_graph<VP,EP,GP>::vertex_descriptor v,
	     const Property& ep,
	     undirected_graph<VP,EP,GP> &g)
    {
	return g.add_edge(u, v, ep);
    }
}

#endif

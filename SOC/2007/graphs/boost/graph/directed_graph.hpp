// (C) Copyright Andrew Sutton 2007
//
// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0 (See accompanying file
// LICENSE_1_0.txt or http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_GRAPH_DIRECTED_GRAPH_HPP
#define BOOST_GRAPH_DIRECTED_GRAPH_HPP

#include <boost/graph/adjacency_list.hpp>

namespace boost
{
    template <
	typename VertexProperty = no_property,
	typename EdgeProperty = no_property,
	typename GraphProperty = no_property
	>
    class directed_graph
    {
    public:
	typedef adjacency_list<listS,
			       listS,
			       bidirectionalS,
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

	inline directed_graph(const GraphProperty& p = GraphProperty())
	    : m_graph(p)
	{}

	inline directed_graph(const directed_graph& x)
	    : m_graph(x)
	{}

	inline directed_graph(vertices_size_type n,
				const GraphProperty& p = GraphProperty())
	    : m_graph(n, p)
	{}

	inline type& impl()
	{ return m_graph; }

	inline const type& impl() const
	{ return m_graph; }


	// bundled property support
#ifndef BOOST_GRAPH_NO_BUNDLED_PROPERTIES
	vertex_bundled& operator [](vertex_descriptor v)
	{ return m_graph[v]; }

	const vertex_bundled& operator [](vertex_descriptor v) const
	{ return m_graph[v]; }

	edge_bundled& operator [](edge_descriptor e)
	{ return m_graph[e]; }

	const edge_bundled& operator [](edge_descriptor e) const
	{ return m_graph[e]; }
#endif

	// Graph concepts
	static inline vertex_descriptor null_vertex()
	{ return type::null_vertex(); }
    };

    // IncidenceGraph concepts
    template <class VP, class EP, class GP>
    inline typename directed_graph<VP,EP,GP>::vertex_descriptor
    source(typename directed_graph<VP,EP,GP>::edge_descriptor e,
	   const directed_graph<VP,EP,GP> &g)
    {
	return source(e, g.impl());
    }

    template <class VP, class EP, class GP>
    inline typename directed_graph<VP,EP,GP>::vertex_descriptor
    target(typename directed_graph<VP,EP,GP>::edge_descriptor e,
	   const directed_graph<VP,EP,GP> &g)
    {
	return target(e, g.impl());
    }

    template <class VP, class EP, class GP>
    inline typename directed_graph<VP,EP,GP>::degree_size_type
    out_degree(typename directed_graph<VP,EP,GP>::vertex_descriptor v,
	       const directed_graph<VP,EP,GP> &g)
    {
	return out_degree(v, g.impl());
    }

    template <class VP, class EP, class GP>
    inline std::pair<
	typename directed_graph<VP,EP,GP>::out_edge_iterator,
	typename directed_graph<VP,EP,GP>::out_edge_iterator
	>
    out_edges(typename directed_graph<VP,EP,GP>::vertex_descriptor v,
	      const directed_graph<VP,EP,GP> &g)
    {
	return out_edges(v, g.impl());
    }

    // BidirectionalGraph concepts
    template <class VP, class EP, class GP>
    inline typename directed_graph<VP,EP,GP>::degree_size_type
    in_degree(const directed_graph<VP,EP,GP> &g,
	      typename directed_graph<VP,EP,GP>::vertex_descriptor v)
    {
	return in_degree(v, g.impl());
    }

    template <class VP, class EP, class GP>
    inline std::pair<
	typename directed_graph<VP,EP,GP>::in_edge_iterator,
	typename directed_graph<VP,EP,GP>::in_edge_iterator
	>
    in_edges(typename directed_graph<VP,EP,GP>::vertex_descriptor v,
	     const directed_graph<VP,EP,GP> &g)
    {
	return in_edges(v, g.impl());
    }

    template <class VP, class EP, class GP>
    inline typename directed_graph<VP,EP,GP>::degree_size_type
    degree(typename directed_graph<VP,EP,GP>::vertex_descriptor v,
	   const directed_graph<VP,EP,GP> &g)
    {
	return degree(v, g.impl());
    }

    // AdjacencyGraph concepts
    template <class VP, class EP, class GP>
    inline std::pair<
        typename directed_graph<VP,EP,GP>::adjacency_iterator,
        typename directed_graph<VP,EP,GP>::adjacency_iterator
        >
    adjacent_vertices(typename directed_graph<VP,EP,GP>::vertex_descriptor v,
                     const directed_graph<VP,EP,GP>& g)
    {
        return adjacent_vertices(v, g.impl());
    }

    // VertexListGraph concepts
    template <class VP, class EP, class GP>
    inline typename directed_graph<VP,EP,GP>::vertices_size_type
    num_vertices(const directed_graph<VP,EP,GP>& g)
    {
	return num_vertices(g.impl());
    }

    template <class VP, class EP, class GP>
    inline std::pair<
	typename directed_graph<VP,EP,GP>::vertex_iterator,
	typename directed_graph<VP,EP,GP>::vertex_iterator
	>
    vertices(const directed_graph<VP,EP,GP>& g)
    {
	return vertices(g.impl());
    }

    // EdgeListGraph concepts
    template <class VP, class EP, class GP>
    inline typename directed_graph<VP,EP,GP>::edges_size_type
    num_edges(const directed_graph<VP,EP,GP>& g)
    {
	return num_edges(g.impl());
    }

    template <class VP, class EP, class GP>
    inline std::pair<
	typename directed_graph<VP,EP,GP>::edge_iterator,
	typename directed_graph<VP,EP,GP>::edge_iterator
	>
    edges(const directed_graph<VP,EP,GP>& g)
    {
	return edges(g.impl());
    }


    // MutableGraph concepts
    template <class VP, class EP, class GP>
    inline typename directed_graph<VP,EP,GP>::vertex_descriptor
    add_vertex(directed_graph<VP,EP,GP> &g)
    {
	return add_vertex(g.impl());
    }


    template <class VP, class EP, class GP>
    inline void
    clear_vertex(typename directed_graph<VP,EP,GP>::vertex_descriptor u,
	directed_graph<VP,EP,GP> &g)
    {
	return clear_vertex(u, g.impl());
    }


    template <class VP, class EP, class GP>
    inline void
    remove_vertex(typename directed_graph<VP,EP,GP>::vertex_descriptor u,
		  directed_graph<VP,EP,GP> &g)
    {
	return emove_vertex(u, g.impl());
    }



    template <class VP, class EP, class GP>
    inline std::pair<typename directed_graph<VP,EP,GP>::edge_descriptor, bool>
    add_edge(typename directed_graph<VP,EP,GP>::vertex_descriptor u,
	     typename directed_graph<VP,EP,GP>::vertex_descriptor v,
	     directed_graph<VP,EP,GP> &g)
    {
	return add_edge(u, v, g.impl());
    }


    template <class VP, class EP, class GP>
    inline void
    remove_edge(typename directed_graph<VP,EP,GP>::vertex_descriptor u,
		typename directed_graph<VP,EP,GP>::vertex_descriptor v,
		directed_graph<VP,EP,GP> &g)
    {
	return remove_edge(u, v, g.impl());
    }


    template <class VP, class EP, class GP>
    inline void
    remove_edge(typename directed_graph<VP,EP,GP>::edge_descriptor e,
		directed_graph<VP,EP,GP> &g)
    {
	return remove_edge(e, g.impl());
    }


    template <class VP, class EP, class GP>
    inline void
    remove_edge(typename directed_graph<VP,EP,GP>::edge_iterator i,
		directed_graph<VP,EP,GP> &g)
    {
	return remove_edge(i, g.impl());
    }

    template <class VP, class EP, class GP, class Predicate>
    inline void
    remove_edge_if(Predicate pred,
		   directed_graph<VP,EP,GP> &g)

    {
	return remove_edge_if(pred, g.impl());
    }


    template <class VP, class EP, class GP, class Predicate>
    inline void
    remove_out_edge_if(typename directed_graph<VP,EP,GP>::vertex_descriptor v,
		       Predicate pred,
		       directed_graph<VP,EP,GP> &g)
    {
	return remove_out_edge_if(v, pred, g.impl());
    }

    template <class VP, class EP, class GP, class Predicate>
    inline void
    remove_in_edge_if(typename directed_graph<VP,EP,GP>::vertex_descriptor v,
		      Predicate pred,
		      directed_graph<VP,EP,GP> &g)
    {
	return remove_in_edge_if(v, pred, g.impl());
    }

    // PropertyGraph concepts
    template <class VP, class EP, class GP, typename Property>
    inline typename property_map<typename directed_graph<VP,EP,GP>::type, Property>::type
    get(Property p, directed_graph<VP,EP,GP>& g)
    {
	return get(p, g.impl());
    }

    template <class VP, class EP, class GP, typename Property>
    inline typename property_map<typename directed_graph<VP,EP,GP>::type, Property>::const_type
    get(Property p, const directed_graph<VP,EP,GP>& g)
    {
	return get(p, g.impl());
    }

    template <class VP, class EP, class GP, typename Property, typename Key>
    inline typename property_traits<
	typename property_map<typename directed_graph<VP,EP,GP>::type, Property>::const_type
	>::value_type
    get(Property p, const directed_graph<VP,EP,GP> &g, const Key& k)
    {
	return g.get(p, g.impl(), k);
    }

    template <class VP, class EP, class GP, typename Property, typename Key, typename Value>
    inline void
    put(Property p, directed_graph<VP,EP,GP> &g, const Key& k, const Value& v)
    {
	g.put(p, g.impl(), k, v);
    }

    // MutablePropertyGraph concepts
    template <class VP, class EP, class GP, typename Property>
    inline typename directed_graph<VP,EP,GP>::vertex_descriptor
    add_vertex(const Property& vp, directed_graph<VP,EP,GP>& g)
    {
	return add_vertex(vp, g.impl());
    }

    template <class VP, class EP, class GP, typename Property>
    inline std::pair<typename directed_graph<VP,EP,GP>::edge_descriptor, bool>
    add_edge(typename directed_graph<VP,EP,GP>::vertex_descriptor u,
	     typename directed_graph<VP,EP,GP>::vertex_descriptor v,
	     const Property& ep,
	     directed_graph<VP,EP,GP> &g)
    {
	return add_edge(u, v, ep, g.impl());
    }
}

#endif

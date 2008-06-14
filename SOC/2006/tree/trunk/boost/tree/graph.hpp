//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

/** 
 * @file graph.hpp
 * Interoperability with the BGL (Boost Graph Library).
 */
 
#ifndef BOOST_TREE_GRAPH_HPP
#define BOOST_TREE_GRAPH_HPP

#include <boost/tree/cursor.hpp>

#include <boost/graph/graph_traits.hpp>
#include <boost/property_map.hpp>
#include <boost/iterator/iterator_adaptor.hpp>

#include <boost/type_traits/integral_constant.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/utility/enable_if.hpp>

namespace boost {

using boost::tree::cursor_size;
using boost::tree::cursor_horizontal_traversal;

namespace tree {

template <class Cursor>
class out_edge_iterator
 : public iterator_facade<out_edge_iterator< std::pair<Cursor, Cursor> >
 						, std::pair<Cursor, Cursor>
 						, typename cursor_horizontal_traversal<Cursor>::type
 						/*, Reference, Difference*/> {

	struct enabler {};
public:
	out_edge_iterator() : m_edge() {}
	
	explicit out_edge_iterator(Cursor u, Cursor v) : m_edge(u, v) {}

    template <class OtherCursor>
    out_edge_iterator(
        out_edge_iterator<OtherCursor> const& other
      , typename boost::enable_if<
            boost::is_convertible<OtherCursor*,Cursor*>
          , enabler
        >::type = enabler()
    )
      : m_edge(other.m_edge) {}

	
private:
	friend class boost::iterator_core_access;

    template <class OtherCursor>
    bool equal(out_edge_iterator<OtherCursor> const& other) const
    {
        return this->m_edge == other.m_edge;
    }

    void increment()
    { ++m_edge.second; }

    void decrement()
    { --m_edge.second; }

    std::pair<Cursor, Cursor>& dereference() const
    { return m_edge; }

	std::pair<Cursor, Cursor> m_edge; 
};

} // namespace tree

template <class Tree>
struct graph_traits<typename Tree::cursor> {
    typedef typename Tree::cursor vertex_descriptor;
    typedef std::pair<vertex_descriptor
    				, vertex_descriptor> edge_descriptor; // Might be tunable...

	typedef boost::tree::out_edge_iterator<vertex_descriptor> out_edge_iterator;

    typedef directed_tag directed_category;
    typedef disallow_parallel_edge_tag edge_parallel_category;
	typedef incidence_graph_tag traversal_category;
    typedef 
    	typename cursor_size<vertex_descriptor>::type 
    	vertices_size_type;
    typedef
    	typename cursor_size<vertex_descriptor>::type
    	edges_size_type;
    typedef
    	typename cursor_size<vertex_descriptor>::type
    	degree_size_type;
};

template <class Tree>
typename graph_traits<typename Tree::cursor>::vertex_descriptor
source(
    typename graph_traits<typename Tree::cursor>::edge_descriptor e,
    const typename Tree::cursor& g)
{
    return e.first;
}

template <class Tree>
typename graph_traits<typename Tree::cursor>::vertex_descriptor
target(
    typename graph_traits<typename Tree::cursor>::edge_descriptor e,
    const typename Tree::cursor& g)
{
    return e.second;
}

template <class Tree>
inline std::pair<
    typename graph_traits<typename Tree::cursor>::out_edge_iterator,
    typename graph_traits<typename Tree::cursor>::out_edge_iterator >  
out_edges(
    typename graph_traits<typename Tree::cursor>::vertex_descriptor u, 
    const typename Tree::cursor& g)
{
    typedef
    	typename graph_traits<typename Tree::cursor>::out_edge_iterator
    	Iter;
    return std::make_pair(Iter(u, u.begin()), Iter(u, u.end()));
}

template <class Cursor> struct deref_property_map;

template <class Cursor>
struct deref_property_map
: public boost::put_get_helper<typename boost::tree::cursor_reference<Cursor>::type
							 , deref_property_map<Cursor> >
{
    typedef Cursor key_type;
    typedef typename boost::tree::cursor_value<Cursor>::type value_type;
    typedef typename boost::tree::cursor_reference<Cursor>::type reference;
    typedef boost::lvalue_property_map_tag category;

    inline reference operator[](const key_type& v) const { return *v; }
};

// TODO
// We still need suitable property_maps, don't we?
// (which have to invoke *c.begin())


} // namespace boost


#endif // BOOST_TREE_GRAPH_HPP

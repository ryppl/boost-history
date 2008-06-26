//  Copyright (c) 2006-2008, Bernhard Reiter
//
//  Distributed under the Boost Software License, Version 1.0.
//  (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <boost/tree/graph.hpp>

#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/visitors.hpp>

#include <boost/test/minimal.hpp>

#include <list>
#include <iterator>

#include "helpers.hpp"
#include "test_tree_traversal_data.hpp"

int test_main(int, char* [])
{
	using namespace boost;
	using boost::tree::binary_tree;
	
	typedef color_traits<default_color_type> Color;
	typedef augmented_type< int, boost::default_color_type > data_type;
	
	typedef binary_tree< data_type > tree_type;
	typedef tree_type::cursor cursor;
	
	tree_type test_tree;
	create_test_data_tree(test_tree);
	
	std::list<int> test_list;
	typedef std::back_insert_iterator< std::list<int> > bi_list_int_type;
	bi_list_int_type bi_list_int = std::back_inserter(test_list);

	// TODO:

	// Wrap a BGL (Incidence)Graph around test_tree
	
	// Traverse the Graph and check if the result is equal
	// to the test data sets, using the depth_first_visit algorithm and 
	// a visitor that is invoked at entering (preorder)
	// the vertices.

	boost::extract_property_map<
		cursor, 
		boost::tree::cursor_value<cursor>::type::extract_data
	> dpm;
	
	boost::extract_property_map<
		cursor, 
		boost::tree::cursor_value<cursor>::type::extract_meta
	> cpm;

	BOOST_CHECK(get(dpm, test_tree.root()) == 8); // Check the entire tree?
	BOOST_CHECK(get(cpm, test_tree.root()) == Color::white());
	
	put(cpm, test_tree.root(), Color::gray());	
	BOOST_CHECK(get(cpm, test_tree.root()) == Color::gray());
	put(cpm, test_tree.root(), Color::white());	
	BOOST_CHECK(get(cpm, test_tree.root()) == Color::white());
	
	boost::dfs_visitor< 
		boost::property_writer<
			boost::extract_property_map<
				cursor, 
				boost::tree::cursor_value<cursor>::type::extract_data
			>, 
			bi_list_int_type, 
			boost::on_discover_vertex> 
		> 
		preorder_writer(write_property(dpm, bi_list_int, 
					    boost::on_discover_vertex()));
		
// 	boost::depth_first_visit(test_tree, test_tree.root(), preorder_writer, cpm, empty_cursor<tree_type>);
	
	graph_traits<tree_type>::vertex_descriptor v = test_tree.root();
	
	graph_traits<tree_type>::out_edge_iterator oei, oei_end;
	tie(oei, oei_end) = out_edges(v, test_tree);
	
	cursor w = target(*oei, test_tree);
	
	w = test_tree.root().begin().end().begin();
	default_color_type color = get(cpm, w);
	BOOST_CHECK(color == Color::white());
	
	put(cpm, w, Color::white());
	BOOST_CHECK(get(cpm, w) == Color::white());
	
	BOOST_CHECK(!empty_cursor(v, test_tree));
	
	BOOST_CHECK(oei != oei_end);
	
	BOOST_CHECK(source(*oei, test_tree) == test_tree.root());
	BOOST_CHECK(source(*oei_end, test_tree) == test_tree.root());
	
	BOOST_CHECK(target(*oei, test_tree) == test_tree.root().begin());
	BOOST_CHECK(target(*oei_end, test_tree) == test_tree.root());
	
	BOOST_CHECK(out_degree(v, test_tree) == 2);
//		
//	BOOST_CHECK(test_list.size() == 2);
//	
//	std::list<int>::const_iterator ci = test_list.begin();
//	
//	BOOST_CHECK(*ci == 8);
//	BOOST_CHECK(*++ci == 10); //FIXME
	
//	test::preorder::traversal(test_list.begin(), test_list.end());
	
	// Output test_tree using write_graphviz. This might require copying
	// the IncidenceGraph to a VertexListGraph (using copy_component) 
	
	return 0;
}

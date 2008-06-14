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
	using boost::tree::binary_tree;
	typedef binary_tree<int>::cursor cursor;
	
	binary_tree<int> test_tree;	
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

	boost::deref_property_map<cursor> dpm;
	BOOST_CHECK(get(dpm, test_tree.root().begin()) == 8); // Check the entire tree?
	
	boost::dfs_visitor< boost::property_writer<
							boost::deref_property_map<cursor>, 
							bi_list_int_type, 
							boost::on_discover_vertex> 
						> 
		preorder_writer(write_property(dpm, bi_list_int, 
					    boost::on_discover_vertex()));
	
	// We need a color map!
 	//boost::depth_first_visit(test_tree, test_tree.root(), preorder_writer);
	
	// Output test_tree using write_graphviz. This might require copying
	// the IncidenceGraph to a VertexListGraph (using copy_component) 
	
	return 0;
}

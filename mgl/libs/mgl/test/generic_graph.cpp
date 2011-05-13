// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#include <typeinfo>

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#undef  BOOST_MPL_LIMIT_METAFUNCTION_ARITY
#define BOOST_MPL_LIMIT_METAFUNCTION_ARITY 6
#include <boost/mpl/apply.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/void.hpp>

#include <boost/typeof/typeof.hpp>

#include <boost/mgl/graph.hpp>
#include <boost/mgl/graph_policies.hpp>
#include <boost/mgl/depth_first_search.hpp>
#include <boost/mgl/breadth_first_search.hpp>
#include <boost/mgl/find.hpp>

#define BOOST_TEST_MODULE "Generic Graph Test"
#include <boost/test/unit_test.hpp>

using namespace boost;
using namespace boost::mgl;

namespace mgl = boost::mgl;

// something that has nothing to do with the test
struct foo {};

// vertices
struct a {};
struct b {};
struct c {};
struct d {};

class simple_directed_graph : public graph<simple_directed_graph, VmapDirectedGraph>
{
public:
    // adjacency list of the graph
    struct adjacency_list : mpl::vector4<
        //    Node   Adjacency
        //    from   nodes
        //  +------+--------------------------------------------+
        row < a    , mpl::vector2<b, c>                         >,
        row < b    , mpl::vector1<d>                            >,
        row < c    , mpl::vector1<b>                            >,
        row < d    , mpl::vector0<>                             >
        //  +------+--------------------------------------------+
    > {};
};

class simple_undirected_graph : public graph<simple_undirected_graph, VmapUndirectedGraph>
{
public:
    // adjacency list of the graph
    struct adjacency_list : mpl::vector4<
        //    Node   Adjacency
        //    from   nodes
        //  +------+--------------------------------------------+
        row < a    , mpl::vector2<b, c>                         >,
        row < b    , mpl::vector1<d>                            >,
        row < c    , mpl::vector1<b>                            >,
        row < d    , mpl::vector0<>                             >
        //  +------+--------------------------------------------+
    > {};
};

BOOST_AUTO_TEST_CASE(test_directed_graph_dfs)
{
	typedef mpl::vector4<
		mpl::pair<b, d>,
		mpl::pair<c, b>,
		mpl::pair<a, b>,
		mpl::pair<a, c>
	> expected_result1;
	typedef BOOST_TYPEOF(expected_result1()) expected_result1_t;

	typedef depth_first_search<simple_directed_graph, ::boost::mgl::edge_trace_visitor, b>::type::visitor_result result1;
	typedef BOOST_TYPEOF(result1()) result1_t;

	BOOST_CHECK_MESSAGE(typeid(result1_t) == typeid(expected_result1_t), "depth_first_search<> should return the expected mpl::vector<>");

	typedef mpl::vector1<
		mpl::pair<b, d>
	> expected_result2;
	typedef BOOST_TYPEOF(expected_result2()) expected_result2_t;

	typedef depth_first_search<simple_directed_graph, ::boost::mgl::edge_trace_visitor, b, EndWhenNoVerticesFound>::type::visitor_result result2;
	typedef BOOST_TYPEOF(result2()) result2_t;

	BOOST_CHECK_MESSAGE(typeid(result2_t) == typeid(expected_result2_t), "depth_first_search<> should return the expected mpl::vector<>");
}

BOOST_AUTO_TEST_CASE(test_directed_graph_dfs_find)
{
	typedef dfs_find<simple_directed_graph, a>::type result1;
	typedef dfs_find<simple_directed_graph, b>::type result2;
	typedef dfs_find<simple_directed_graph, c>::type result3;
	typedef dfs_find<simple_directed_graph, d>::type result4;

	BOOST_CHECK_MESSAGE(mpl::is_not_void_<result1>::value, "dfs_find<> should return a valid iterator");
	BOOST_CHECK_MESSAGE(mpl::is_not_void_<result2>::value, "dfs_find<> should return a valid iterator");
	BOOST_CHECK_MESSAGE(mpl::is_not_void_<result3>::value, "dfs_find<> should return a valid iterator");
	BOOST_CHECK_MESSAGE(mpl::is_not_void_<result4>::value, "dfs_find<> should return a valid iterator");

	typedef dfs_find<simple_directed_graph, foo>::type result5;

	BOOST_CHECK_MESSAGE(mpl::is_void_<result5>::value, "dfs_find<> should return an invalid iterator");
}

BOOST_AUTO_TEST_CASE(test_undirected_graph_dfs)
{
	typedef mpl::vector8<
		mpl::pair<b, d>,
		mpl::pair<b, a>,
		mpl::pair<b, c>,
		mpl::pair<d, b>,
		mpl::pair<a, b>,
		mpl::pair<a, c>,
		mpl::pair<c, b>,
		mpl::pair<c, a>
	> expected_result;
	typedef BOOST_TYPEOF(expected_result()) expected_result_t;

	typedef depth_first_search<simple_undirected_graph, ::boost::mgl::edge_trace_visitor, b>::type::visitor_result result1;
	typedef BOOST_TYPEOF(result1()) result1_t;

	BOOST_CHECK_MESSAGE(typeid(result1_t) == typeid(expected_result_t), "depth_first_search<> should return the expected mpl::vector<>");

	typedef depth_first_search<simple_undirected_graph, ::boost::mgl::edge_trace_visitor, b, EndWhenNoVerticesFound>::type::visitor_result result2;
	typedef BOOST_TYPEOF(result2()) result2_t;

	BOOST_CHECK_MESSAGE(typeid(result2_t) == typeid(expected_result_t), "depth_first_search<> should return the expected mpl::vector<>");
}

BOOST_AUTO_TEST_CASE(test_directed_graph_bfs)
{
	typedef mpl::vector4<
		mpl::pair<b, d>,
		mpl::pair<c, b>,
		mpl::pair<a, b>,
		mpl::pair<a, c>
	> expected_result1;
	typedef BOOST_TYPEOF(expected_result1()) expected_result1_t;

	typedef breadth_first_search<simple_directed_graph, ::boost::mgl::edge_trace_visitor, b>::type::visitor_result result1;
	typedef BOOST_TYPEOF(result1()) result1_t;

	BOOST_CHECK_MESSAGE(typeid(result1_t) == typeid(expected_result1_t), "breadth_first_search<> should return the expected mpl::vector<>");

	typedef mpl::vector1<
		mpl::pair<b, d>
	> expected_result2;
	typedef BOOST_TYPEOF(expected_result2()) expected_result2_t;

	typedef breadth_first_search<simple_directed_graph, ::boost::mgl::edge_trace_visitor, b, EndWhenNoVerticesFound>::type::visitor_result result2;
	typedef BOOST_TYPEOF(result2()) result2_t;

	BOOST_CHECK_MESSAGE(typeid(result2_t) == typeid(expected_result2_t), "breadth_first_search<> should return the expected mpl::vector<>");
}

BOOST_AUTO_TEST_CASE(test_directed_graph_bfs_find)
{
	typedef bfs_find<simple_directed_graph, a>::type result1;
	typedef bfs_find<simple_directed_graph, b>::type result2;
	typedef bfs_find<simple_directed_graph, c>::type result3;
	typedef bfs_find<simple_directed_graph, d>::type result4;

	BOOST_CHECK_MESSAGE(mpl::is_not_void_<result1>::value, "bfs_find<> should return a valid iterator");
	BOOST_CHECK_MESSAGE(mpl::is_not_void_<result2>::value, "bfs_find<> should return a valid iterator");
	BOOST_CHECK_MESSAGE(mpl::is_not_void_<result3>::value, "bfs_find<> should return a valid iterator");
	BOOST_CHECK_MESSAGE(mpl::is_not_void_<result4>::value, "bfs_find<> should return a valid iterator");

	typedef bfs_find<simple_directed_graph, foo>::type result5;

	BOOST_CHECK_MESSAGE(mpl::is_void_<result5>::value, "bfs_find<> should return an invalid iterator");
}

BOOST_AUTO_TEST_CASE(test_undirected_graph_bfs)
{
	typedef mpl::vector8<
		mpl::pair<b, d>,
		mpl::pair<b, a>,
		mpl::pair<b, c>,
		mpl::pair<d, b>,
		mpl::pair<a, b>,
		mpl::pair<a, c>,
		mpl::pair<c, b>,
		mpl::pair<c, a>
	> expected_result;
	typedef BOOST_TYPEOF(expected_result()) expected_result_t;

	typedef breadth_first_search<simple_undirected_graph, ::boost::mgl::edge_trace_visitor, b>::type::visitor_result result1;
	typedef BOOST_TYPEOF(result1()) result1_t;

	BOOST_CHECK_MESSAGE(typeid(result1_t) == typeid(expected_result_t), "breadth_first_search<> should return the expected mpl::vector<>");

	typedef breadth_first_search<simple_undirected_graph, ::boost::mgl::edge_trace_visitor, b, EndWhenNoVerticesFound>::type::visitor_result result2;
	typedef BOOST_TYPEOF(result2()) result2_t;

	BOOST_CHECK_MESSAGE(typeid(result2_t) == typeid(expected_result_t), "breadth_first_search<> should return the expected mpl::vector<>");
}

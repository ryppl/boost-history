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

#include <boost/typeof/typeof.hpp>

#include <boost/mpl/bool.hpp>
#include <boost/mpl/void.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>

#include <boost/type_traits/is_integral.hpp>

#include <boost/mgl/directed_graph.hpp>
#include <boost/mgl/depth_first_search.hpp>

#define BOOST_TEST_MODULE "Visitor Test"
#include <boost/test/unit_test.hpp>

using namespace boost;
using namespace boost::mgl;

namespace mgl = boost::mgl;

// vertices
struct v0 {};
struct v1 {};
struct v2 {};
struct v3 {};
struct v4 {};

class some_graph : public directed_graph<some_graph>
{
public:
    // adjacency list of the graph
    struct adjacency_list : mpl::vector5<
        //    Node   Adjacency
        //    from   nodes
        //  +------+--------------------------------------------+
        row < v0   , mpl::vector1<v4>                           >,
        row < v1   , mpl::vector3<v2, v1, v0>                   >,
        row < v2   , mpl::vector2<v1, v0>                       >,
        row < v3   , mpl::vector1<v0>                           >,
        row < v4   , mpl::vector1<v4>                           >
        //  +------+--------------------------------------------+
    > {};
};

BOOST_AUTO_TEST_CASE(test_null_visitor)
{
	typedef depth_first_search<some_graph, ::boost::mgl::null_visitor>::type::visitor_result result;
	typedef BOOST_TYPEOF(result()) result_t;

	typedef BOOST_TYPEOF(mpl::void_()) void_t;

	BOOST_CHECK_MESSAGE(typeid(result_t) == typeid(void_t), "depth_first_search<> should return ::mpl::void_");
}

BOOST_AUTO_TEST_CASE(test_vertex_counter_visitor)
{
	typedef depth_first_search<some_graph, ::boost::mgl::vertex_counter_visitor>::type::visitor_result result;
	typedef BOOST_TYPEOF(result()) result_t;

	BOOST_CHECK_MESSAGE(result::value == 5, "depth_first_search<> should return 5");
}

BOOST_AUTO_TEST_CASE(test_vertex_trace_visitor)
{
	typedef mpl::vector5<v0, v4, v3, v2, v1> expected_result;
	typedef BOOST_TYPEOF(expected_result()) expected_result_t;

	typedef depth_first_search<some_graph, ::boost::mgl::vertex_trace_visitor>::type::visitor_result result;
	typedef BOOST_TYPEOF(result()) result_t;

	BOOST_CHECK_MESSAGE(typeid(result_t) == typeid(expected_result_t), "depth_first_search<> should return expected ::mpl::vector<>");
}

BOOST_AUTO_TEST_CASE(test_edge_counter_visitor)
{
	typedef depth_first_search<some_graph, ::boost::mgl::edge_counter_visitor>::type::visitor_result result;
	typedef BOOST_TYPEOF(result()) result_t;

	BOOST_CHECK_MESSAGE(result::value == 8, "depth_first_search<> should return 8");
}

BOOST_AUTO_TEST_CASE(test_edge_trace_visitor)
{
	typedef mpl::vector8<
		mpl::pair<v0, v4>,
		mpl::pair<v4, v4>,
		mpl::pair<v3, v0>,
		mpl::pair<v2, v1>,
		mpl::pair<v2, v0>,
		mpl::pair<v1, v2>,
		mpl::pair<v1, v1>,
		mpl::pair<v1, v0>
	> expected_result;
	typedef BOOST_TYPEOF(expected_result()) expected_result_t;

	typedef depth_first_search<some_graph, ::boost::mgl::edge_trace_visitor>::type::visitor_result result;
	typedef BOOST_TYPEOF(result()) result_t;

	BOOST_CHECK_MESSAGE(typeid(result_t) == typeid(expected_result_t), "depth_first_search<> should return expected ::mpl::vector<>");
}

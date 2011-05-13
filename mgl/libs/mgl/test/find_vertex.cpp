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

#include <boost/mpl/vector.hpp>
#include <boost/mpl/void.hpp>

#include <boost/typeof/typeof.hpp>

#include <boost/mgl/directed_graph.hpp>
#include <boost/mgl/find.hpp>

#define BOOST_TEST_MODULE "Find Vertex Test"
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
struct v5 {};
struct v6 {};
struct v7 {};
struct v8 {};
struct v9 {};

class some_graph : public directed_graph<some_graph>
{
public:
    // adjacency list of the graph
    struct adjacency_list : mpl::vector10<
        //    Node   Adjacency
        //    from   nodes
        //  +------+--------------------------------------------+
        row < v0   , mpl::vector1<v6>                           >,
        row < v1   , mpl::vector1<v8>                           >,
        row < v2   , mpl::vector5<v5, v8, v1, v9, v6>           >,
        row < v3   , mpl::vector1<v7>                           >,
        row < v4   , mpl::vector2<v1, v5>                       >,
        row < v5   , mpl::vector0<>                             >,
        row < v6   , mpl::vector0<>                             >,
        row < v7   , mpl::vector1<v1>                           >,
        row < v8   , mpl::vector1<v3>                           >,
        row < v9   , mpl::vector0<>                             >
        //  +------+--------------------------------------------+
    > {};
};

BOOST_AUTO_TEST_CASE(dfs_find_all_vertices)
{
	typedef BOOST_TYPEOF(mpl::void_()) void_t;

	typedef dfs_find<some_graph, v0>::type iter_v0;
	typedef deref<iter_v0>::type iter_v0_deref;
	typedef BOOST_TYPEOF(iter_v0_deref()) iter_v0_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v0_deref_t) != typeid(void_t), "dfs_find<> should return a valid iterator type");

	typedef dfs_find<some_graph, v1>::type iter_v1;
	typedef deref<iter_v1>::type iter_v1_deref;
	typedef BOOST_TYPEOF(iter_v1_deref()) iter_v1_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v1_deref_t) != typeid(void_t), "dfs_find<> should return a valid iterator type");

	typedef dfs_find<some_graph, v2>::type iter_v2;
	typedef deref<iter_v2>::type iter_v2_deref;
	typedef BOOST_TYPEOF(iter_v2_deref()) iter_v2_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v2_deref_t) != typeid(void_t), "dfs_find<> should return a valid iterator type");

	typedef dfs_find<some_graph, v3>::type iter_v3;
	typedef deref<iter_v3>::type iter_v3_deref;
	typedef BOOST_TYPEOF(iter_v3_deref()) iter_v3_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v3_deref_t) != typeid(void_t), "dfs_find<> should return a valid iterator type");

	typedef dfs_find<some_graph, v4>::type iter_v4;
	typedef deref<iter_v4>::type iter_v4_deref;
	typedef BOOST_TYPEOF(iter_v4_deref()) iter_v4_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v4_deref_t) != typeid(void_t), "dfs_find<> should return a valid iterator type");

	typedef dfs_find<some_graph, v5>::type iter_v5;
	typedef deref<iter_v5>::type iter_v5_deref;
	typedef BOOST_TYPEOF(iter_v5_deref()) iter_v5_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v5_deref_t) != typeid(void_t), "dfs_find<> should return a valid iterator type");

	typedef dfs_find<some_graph, v6>::type iter_v6;
	typedef deref<iter_v6>::type iter_v6_deref;
	typedef BOOST_TYPEOF(iter_v6_deref()) iter_v6_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v6_deref_t) != typeid(void_t), "dfs_find<> should return a valid iterator type");

	typedef dfs_find<some_graph, v7>::type iter_v7;
	typedef deref<iter_v7>::type iter_v7_deref;
	typedef BOOST_TYPEOF(iter_v7_deref()) iter_v7_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v7_deref_t) != typeid(void_t), "dfs_find<> should return a valid iterator type");

	typedef dfs_find<some_graph, v8>::type iter_v8;
	typedef deref<iter_v8>::type iter_v8_deref;
	typedef BOOST_TYPEOF(iter_v8_deref()) iter_v8_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v8_deref_t) != typeid(void_t), "dfs_find<> should return a valid iterator type");

	typedef dfs_find<some_graph, v9>::type iter_v9;
	typedef deref<iter_v9>::type iter_v9_deref;
	typedef BOOST_TYPEOF(iter_v9_deref()) iter_v9_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v9_deref_t) != typeid(void_t), "dfs_find<> should return a valid iterator type");
}

BOOST_AUTO_TEST_CASE(invalid_dfs_find_vertex)
{
	typedef BOOST_TYPEOF(mpl::void_()) void_t;

	typedef dfs_find<int, v0>::type iter1;
	typedef deref<iter1>::type iter1_deref;
	typedef BOOST_TYPEOF(iter1_deref()) iter1_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter1_deref_t) == typeid(void_t), "dfs_find<> should return mpl::void_ at invalid graph type");

	typedef dfs_find<some_graph, int>::type iter2;
	typedef deref<iter2>::type iter2_deref;
	typedef BOOST_TYPEOF(iter2_deref()) iter2_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter2_deref_t) == typeid(void_t), "dfs_find<> should return mpl::void_ at invalid vertex type");
}

BOOST_AUTO_TEST_CASE(bfs_find_all_vertices)
{
	typedef BOOST_TYPEOF(mpl::void_()) void_t;

	typedef bfs_find<some_graph, v0>::type iter_v0;
	typedef deref<iter_v0>::type iter_v0_deref;
	typedef BOOST_TYPEOF(iter_v0_deref()) iter_v0_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v0_deref_t) != typeid(void_t), "bfs_find<> should return a valid iterator type");

	typedef bfs_find<some_graph, v1>::type iter_v1;
	typedef deref<iter_v1>::type iter_v1_deref;
	typedef BOOST_TYPEOF(iter_v1_deref()) iter_v1_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v1_deref_t) != typeid(void_t), "bfs_find<> should return a valid iterator type");

	typedef bfs_find<some_graph, v2>::type iter_v2;
	typedef deref<iter_v2>::type iter_v2_deref;
	typedef BOOST_TYPEOF(iter_v2_deref()) iter_v2_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v2_deref_t) != typeid(void_t), "bfs_find<> should return a valid iterator type");

	typedef bfs_find<some_graph, v3>::type iter_v3;
	typedef deref<iter_v3>::type iter_v3_deref;
	typedef BOOST_TYPEOF(iter_v3_deref()) iter_v3_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v3_deref_t) != typeid(void_t), "bfs_find<> should return a valid iterator type");

	typedef bfs_find<some_graph, v4>::type iter_v4;
	typedef deref<iter_v4>::type iter_v4_deref;
	typedef BOOST_TYPEOF(iter_v4_deref()) iter_v4_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v4_deref_t) != typeid(void_t), "bfs_find<> should return a valid iterator type");

	typedef bfs_find<some_graph, v5>::type iter_v5;
	typedef deref<iter_v5>::type iter_v5_deref;
	typedef BOOST_TYPEOF(iter_v5_deref()) iter_v5_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v5_deref_t) != typeid(void_t), "bfs_find<> should return a valid iterator type");

	typedef bfs_find<some_graph, v6>::type iter_v6;
	typedef deref<iter_v6>::type iter_v6_deref;
	typedef BOOST_TYPEOF(iter_v6_deref()) iter_v6_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v6_deref_t) != typeid(void_t), "bfs_find<> should return a valid iterator type");

	typedef bfs_find<some_graph, v7>::type iter_v7;
	typedef deref<iter_v7>::type iter_v7_deref;
	typedef BOOST_TYPEOF(iter_v7_deref()) iter_v7_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v7_deref_t) != typeid(void_t), "bfs_find<> should return a valid iterator type");

	typedef bfs_find<some_graph, v8>::type iter_v8;
	typedef deref<iter_v8>::type iter_v8_deref;
	typedef BOOST_TYPEOF(iter_v8_deref()) iter_v8_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v8_deref_t) != typeid(void_t), "bfs_find<> should return a valid iterator type");

	typedef bfs_find<some_graph, v9>::type iter_v9;
	typedef deref<iter_v9>::type iter_v9_deref;
	typedef BOOST_TYPEOF(iter_v9_deref()) iter_v9_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_v9_deref_t) != typeid(void_t), "bfs_find<> should return a valid iterator type");
}

BOOST_AUTO_TEST_CASE(invalid_bfs_find_vertex)
{
	typedef BOOST_TYPEOF(mpl::void_()) void_t;

	typedef bfs_find<int, v0>::type iter1;
	typedef deref<iter1>::type iter1_deref;
	typedef BOOST_TYPEOF(iter1_deref()) iter1_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter1_deref_t) == typeid(void_t), "bfs_find<> should return mpl::void_ at invalid graph type");

	typedef bfs_find<some_graph, int>::type iter2;
	typedef deref<iter2>::type iter2_deref;
	typedef BOOST_TYPEOF(iter2_deref()) iter2_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter2_deref_t) == typeid(void_t), "bfs_find<> should return mpl::void_ at invalid vertex type");
}

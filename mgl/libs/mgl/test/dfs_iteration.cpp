// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#include <typeinfo>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/void.hpp>

#include <boost/typeof/typeof.hpp>

#include <boost/mgl/directed_graph.hpp>
#include <boost/mgl/begin_end.hpp>
#include <boost/mgl/next_prior.hpp>
#include <boost/mgl/iterator_policies.hpp>

#define BOOST_TEST_MODULE "DFS Iteration Test"
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

struct foo {};

BOOST_AUTO_TEST_CASE(invalid_dfs_begin)
{
	typedef BOOST_TYPEOF(mpl::void_()) void_t;

	typedef dfs_begin<foo>::type iter;
	typedef BOOST_TYPEOF(iter()) iter_t;

	BOOST_CHECK_MESSAGE(typeid(iter_t) == typeid(void_t), "dfs_begin<> should return mpl::void_ at invalid graph type");
}

BOOST_AUTO_TEST_CASE(dfs_iteration_short)
{
	typedef BOOST_TYPEOF(mpl::void_()) void_t;
	typedef BOOST_TYPEOF(some_graph::end::type()) end_t;

	typedef dfs_begin<some_graph, ::boost::mgl::EndWhenNoVerticesFound>::type iter;
	typedef deref<iter>::type iter_deref;
	typedef BOOST_TYPEOF(iter_deref()) iter_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_deref_t) != typeid(void_t), "dfs_begin<> should return a valid iterator type");

	typedef ::boost::mgl::next<iter>::type iter_next1;
	typedef deref<iter_next1>::type iter_next1_deref;
	typedef BOOST_TYPEOF(iter_next1_deref()) iter_next1_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_next1_deref_t) != typeid(end_t), "next<> should return a valid iterator type");

	typedef ::boost::mgl::next<iter_next1>::type iter_next2;
	typedef deref<iter_next2>::type iter_next2_deref;
	typedef BOOST_TYPEOF(iter_next2_deref()) iter_next2_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_next2_deref_t) == typeid(end_t), "next<> should return a valid graph::end type");
}

BOOST_AUTO_TEST_CASE(dfs_iteration_complete)
{
	typedef BOOST_TYPEOF(mpl::void_()) void_t;
	typedef BOOST_TYPEOF(some_graph::end::type()) end_t;

	typedef dfs_begin<some_graph>::type iter;
	typedef deref<iter>::type iter_deref;
	typedef BOOST_TYPEOF(iter_deref()) iter_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_deref_t) != typeid(void_t), "dfs_begin<> should return a valid iterator type");

	typedef ::boost::mgl::next<iter>::type iter_next1;
	typedef deref<iter_next1>::type iter_next1_deref;
	typedef BOOST_TYPEOF(iter_next1_deref()) iter_next1_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_next1_deref_t) != typeid(end_t), "next<> should return a valid iterator type");

	typedef ::boost::mgl::next<iter_next1>::type iter_next2;
	typedef deref<iter_next2>::type iter_next2_deref;
	typedef BOOST_TYPEOF(iter_next2_deref()) iter_next2_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_next2_deref_t) != typeid(end_t), "next<> should return a valid iterator type");

	typedef ::boost::mgl::next<iter_next2>::type iter_next3;
	typedef deref<iter_next3>::type iter_next3_deref;
	typedef BOOST_TYPEOF(iter_next3_deref()) iter_next3_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_next3_deref_t) != typeid(end_t), "next<> should return a valid iterator type");

	typedef ::boost::mgl::next<iter_next3>::type iter_next4;
	typedef deref<iter_next4>::type iter_next4_deref;
	typedef BOOST_TYPEOF(iter_next4_deref()) iter_next4_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_next4_deref_t) != typeid(end_t), "next<> should return a valid iterator type");

	typedef ::boost::mgl::next<iter_next4>::type iter_next5;
	typedef deref<iter_next5>::type iter_next5_deref;
	typedef BOOST_TYPEOF(iter_next5_deref()) iter_next5_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_next5_deref_t) != typeid(end_t), "next<> should return a valid iterator type");

	typedef ::boost::mgl::next<iter_next5>::type iter_next6;
	typedef deref<iter_next6>::type iter_next6_deref;
	typedef BOOST_TYPEOF(iter_next6_deref()) iter_next6_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_next6_deref_t) != typeid(end_t), "next<> should return a valid iterator type");

	typedef ::boost::mgl::next<iter_next6>::type iter_next7;
	typedef deref<iter_next7>::type iter_next7_deref;
	typedef BOOST_TYPEOF(iter_next7_deref()) iter_next7_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_next7_deref_t) != typeid(end_t), "next<> should return a valid iterator type");

	typedef ::boost::mgl::next<iter_next7>::type iter_next8;
	typedef deref<iter_next8>::type iter_next8_deref;
	typedef BOOST_TYPEOF(iter_next8_deref()) iter_next8_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_next8_deref_t) != typeid(end_t), "next<> should return a valid iterator type");

	typedef ::boost::mgl::next<iter_next8>::type iter_next9;
	typedef deref<iter_next9>::type iter_next9_deref;
	typedef BOOST_TYPEOF(iter_next9_deref()) iter_next9_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_next9_deref_t) != typeid(end_t), "next<> should return a valid iterator type");

	typedef ::boost::mgl::next<iter_next9>::type iter_next10;
	typedef deref<iter_next10>::type iter_next10_deref;
	typedef BOOST_TYPEOF(iter_next10_deref()) iter_next10_deref_t;

	BOOST_CHECK_MESSAGE(typeid(iter_next10_deref_t) == typeid(end_t), "next<> should return a valid graph::end type");
}

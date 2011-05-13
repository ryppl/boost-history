// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <typeinfo>

#include <boost/mgl/directed_graph.hpp>
#include <boost/mgl/iterator.hpp>
#include <boost/mgl/begin_end.hpp>
#include <boost/mgl/next_prior.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/set.hpp>

#include <boost/mpl/assert.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/has_key.hpp>
#include <boost/type_traits/is_same.hpp>

#include <boost/typeof/typeof.hpp>

using namespace boost;
using namespace boost::mgl;

namespace mgl = boost::mgl;

class simple_directed : public directed_graph<simple_directed>
{
public:
	// vertices
	struct a {};
	struct b {};
	struct c {};
	struct d {};
	struct e {};
	struct f {};
	struct g {};
	struct h {};

	// vertex properties
	struct prop_v1 {};
	struct prop_v2 {};
	struct prop_v3 {};

	// edge properties
	struct prop_e1 {};
	struct prop_e2 {};

	//
	//  a-------------b       h
	//  |\ \       /  |
	//  | \  \   /    |
	//  |  \   c      d
	//  |   \  |      |
	//  |    \ |      |
	//  e------f------g
	//
	// adjacency list of the graph
	struct adjacency_list : mpl::vector8<
		//    Node   Adjacency
		//    from   nodes
		//  +------+--------------------------------------------+
		row < a    , mpl::vector3<c, e, f>                      >,
		row < b    , mpl::vector1<a>                            >,
		row < c    , mpl::vector2<b, f>                         >,
		row < d    , mpl::vector2<b, g>                         >,
		row < e    , mpl::vector2<a, f>                         >,
		row < f    , mpl::vector0<>                             >,
		row < g    , mpl::vector2<d, f>                         >,
		row < h    , mpl::vector0<>                             >
		//  +------+--------------------------------------------+
	> {};

	// vertex properties
	struct vertex_property_list : mpl::vector8<
		//       Node   Properties
		//     +------+-----------------------------------------+
		v_prop < a    , mpl::vector1<prop_v1>                   >,
		v_prop < b    , mpl::vector1<prop_v2>                   >,
		v_prop < c    , mpl::vector0<>                          >,
		v_prop < d    , mpl::vector2<prop_v1, prop_v2>          >,
		v_prop < e    , mpl::vector2<prop_v1, prop_v3>          >,
		v_prop < f    , mpl::vector2<prop_v2, prop_v3>          >,
		v_prop < g    , mpl::vector0<>                          >,
		v_prop < h    , mpl::vector0<>                          >
		//     +------+-----------------------------------------+
	> {};

	// edge properties and weights
	struct edge_list : mpl::vector12<
		//     Node   Node   Weight   Properties
		//     from   to
		//   +------+------+--------+---------------------------+
		edge < a    , b    , 3      , mpl::vector1<prop_e1>     >,
		edge < a    , c    , 2      , mpl::vector1<prop_e1>     >,
		//   +------+------+--------+---------------------------+
		edge < b    , a    , 3      , mpl::vector1<prop_e2>     >,
		edge < b    , c    , 3      , mpl::vector0<>            >,
		edge < b    , e    , 1      , mpl::vector0<>            >,
		//   +------+------+--------+---------------------------+
		edge < c    , a    , 2      , mpl::vector0<>            >,
		edge < c    , e    , 8      , mpl::vector1<prop_e1>     >,
		edge < c    , d    , 2      , mpl::vector1<prop_e2>     >,
		//   +------+------+--------+---------------------------+
		edge < e    , b    , 1      , mpl::vector0<>            >,
		edge < e    , c    , 8      , mpl::vector0<>            >,
		edge < e    , d    , 11     , mpl::vector0<>            >,
		//   +------+------+--------+---------------------------+
		edge < f    , g    , 1      , mpl::vector0<>            >
		//   +------+------+--------+---------------------------+
	> {};
};

template<class TracePolicy>
void perform_dfs_iteration_to_end_of_graph()
{
	// get dfs-iterator to begin of graph
	typedef typename dfs_begin<simple_directed, EndAtEndOfGraph, TracePolicy>::type dfs_begin;

	typedef BOOST_TYPEOF(dfs_begin()) dfs_begin_t;

	//std::cout << "type of 'dfs_begin' = " << typeid(dfs_begin_t).name() << std::endl;

	// dereference iterator to begin to get first vertex of graph
	typedef typename deref<dfs_begin>::type dfs_begin_deref;

	typedef BOOST_TYPEOF(dfs_begin_deref()) dfs_begin_deref_t;

	std::cout << "type of 'dfs_begin_deref' = " << typeid(dfs_begin_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_begin>::type dfs_next1;

	typedef BOOST_TYPEOF(dfs_next1()) dfs_next1_t;

	//std::cout << "type of 'dfs_next1' = " << typeid(dfs_next1_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next1>::type dfs_next1_deref;

	typedef BOOST_TYPEOF(dfs_next1_deref()) dfs_next1_deref_t;

	std::cout << "type of 'dfs_next1_deref' = " << typeid(dfs_next1_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next1>::type dfs_next2;

	typedef BOOST_TYPEOF(dfs_next2()) dfs_next2_t;

	//std::cout << "type of 'dfs_next2' = " << typeid(dfs_next2_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next2>::type dfs_next2_deref;

	typedef BOOST_TYPEOF(dfs_next2_deref()) dfs_next2_deref_t;

	std::cout << "type of 'dfs_next2_deref' = " << typeid(dfs_next2_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next2>::type dfs_next3;

	typedef BOOST_TYPEOF(dfs_next3()) dfs_next3_t;

	//std::cout << "type of 'dfs_next3' = " << typeid(dfs_next3_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next3>::type dfs_next3_deref;

	typedef BOOST_TYPEOF(dfs_next3_deref()) dfs_next3_deref_t;

	std::cout << "type of 'dfs_next3_deref' = " << typeid(dfs_next3_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next3>::type dfs_next4;

	typedef BOOST_TYPEOF(dfs_next4()) dfs_next4_t;

	//std::cout << "type of 'dfs_next4' = " << typeid(dfs_next4_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next4>::type dfs_next4_deref;

	typedef BOOST_TYPEOF(dfs_next4_deref()) dfs_next4_deref_t;

	std::cout << "type of 'dfs_next4_deref' = " << typeid(dfs_next4_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next4>::type dfs_next5;

	typedef BOOST_TYPEOF(dfs_next5()) dfs_next5_t;

	//std::cout << "type of 'dfs_next5' = " << typeid(dfs_next5_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next5>::type dfs_next5_deref;

	typedef BOOST_TYPEOF(dfs_next5_deref()) dfs_next5_deref_t;

	std::cout << "type of 'dfs_next5_deref' = " << typeid(dfs_next5_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next5>::type dfs_next6;

	typedef BOOST_TYPEOF(dfs_next6()) dfs_next6_t;

	//std::cout << "type of 'dfs_next6' = " << typeid(dfs_next6_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next6>::type dfs_next6_deref;

	typedef BOOST_TYPEOF(dfs_next6_deref()) dfs_next6_deref_t;

	std::cout << "type of 'dfs_next6_deref' = " << typeid(dfs_next6_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next6>::type dfs_next7;

	typedef BOOST_TYPEOF(dfs_next7()) dfs_next7_t;

	//std::cout << "type of 'dfs_next7' = " << typeid(dfs_next7_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next7>::type dfs_next7_deref;

	typedef BOOST_TYPEOF(dfs_next7_deref()) dfs_next7_deref_t;

	std::cout << "type of 'dfs_next7_deref' = " << typeid(dfs_next7_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next7>::type dfs_next8;

	typedef BOOST_TYPEOF(dfs_next8()) dfs_next8_t;

	//std::cout << "type of 'dfs_next8' = " << typeid(dfs_next8_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next8>::type dfs_next8_deref;

	typedef BOOST_TYPEOF(dfs_next8_deref()) dfs_next8_deref_t;

	std::cout << "type of 'dfs_next8_deref' = " << typeid(dfs_next8_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next8>::type dfs_next9;

	typedef BOOST_TYPEOF(dfs_next9()) dfs_next9_t;

	//std::cout << "type of 'dfs_next9' = " << typeid(dfs_next9_t).name() << std::endl;

	typedef BOOST_TYPEOF((typename dfs_next9::vertex_trace())) dfs_next9_vertices_t;

	//std::cout << "type of 'dfs_next9::vertex_trace' = " << typeid(dfs_next9_vertices_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next9>::type dfs_next9_deref;

	typedef BOOST_TYPEOF(dfs_next9_deref()) dfs_next9_deref_t;

	std::cout << "type of 'dfs_next9_deref' = " << typeid(dfs_next9_deref_t).name() << std::endl;

	// print trace
	std::cout << "trace = " << typeid(dfs_next9_vertices_t).name() << std::endl;
}

void perform_dfs_iteration_to_end_of_graph_with_trace()
{
	std::cout << "perform DFS iteration to end of graph with trace of vertices" << std::endl;

	perform_dfs_iteration_to_end_of_graph< ::boost::mgl::RecordTrace< ::boost::mpl::vector0<> > >();
}

void perform_dfs_iteration_to_end_of_graph_without_trace()
{
	std::cout << "perform DFS iteration to end of graph without any trace" << std::endl;

	perform_dfs_iteration_to_end_of_graph< ::boost::mgl::NoTrace>();
}

template<class TracePolicy>
void perform_dfs_iteration_to_end_of_path()
{
	// get dfs-iterator to begin of graph
	typedef typename dfs_begin<simple_directed, EndWhenNoVerticesFound, TracePolicy>::type dfs_begin;

	typedef BOOST_TYPEOF(dfs_begin()) dfs_begin_t;

	//std::cout << "type of 'dfs_begin' = " << typeid(dfs_begin_t).name() << std::endl;

	// dereference iterator to begin to get first vertex of graph
	typedef typename deref<dfs_begin>::type dfs_begin_deref;

	typedef BOOST_TYPEOF(dfs_begin_deref()) dfs_begin_deref_t;

	std::cout << "type of 'dfs_begin_deref' = " << typeid(dfs_begin_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_begin>::type dfs_next1;

	typedef BOOST_TYPEOF(dfs_next1()) dfs_next1_t;

	//std::cout << "type of 'dfs_next1' = " << typeid(dfs_next1_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next1>::type dfs_next1_deref;

	typedef BOOST_TYPEOF(dfs_next1_deref()) dfs_next1_deref_t;

	std::cout << "type of 'dfs_next1_deref' = " << typeid(dfs_next1_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next1>::type dfs_next2;

	typedef BOOST_TYPEOF(dfs_next2()) dfs_next2_t;

	//std::cout << "type of 'dfs_next2' = " << typeid(dfs_next2_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next2>::type dfs_next2_deref;

	typedef BOOST_TYPEOF(dfs_next2_deref()) dfs_next2_deref_t;

	std::cout << "type of 'dfs_next2_deref' = " << typeid(dfs_next2_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next2>::type dfs_next3;

	typedef BOOST_TYPEOF(dfs_next3()) dfs_next3_t;

	//std::cout << "type of 'dfs_next3' = " << typeid(dfs_next3_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next3>::type dfs_next3_deref;

	typedef BOOST_TYPEOF(dfs_next3_deref()) dfs_next3_deref_t;

	std::cout << "type of 'dfs_next3_deref' = " << typeid(dfs_next3_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next3>::type dfs_next4;

	typedef BOOST_TYPEOF((typename dfs_next4::traversal_stack())) dfs_next4_t;

	//std::cout << "type of 'dfs_next4' = " << typeid(dfs_next4_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next4>::type dfs_next4_deref;

	typedef BOOST_TYPEOF(dfs_next4_deref()) dfs_next4_deref_t;

	std::cout << "type of 'dfs_next4_deref' = " << typeid(dfs_next4_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next4>::type dfs_next5;

	typedef BOOST_TYPEOF((typename dfs_next5::traversal_stack())) dfs_next5_t;

	//std::cout << "type of 'dfs_next5' = " << typeid(dfs_next5_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next5>::type dfs_next5_deref;

	typedef BOOST_TYPEOF(dfs_next5_deref()) dfs_next5_deref_t;

	std::cout << "type of 'dfs_next5_deref' = " << typeid(dfs_next5_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next5>::type dfs_next6;

	typedef BOOST_TYPEOF((typename dfs_next6::traversal_stack())) dfs_next6_t;

	//std::cout << "type of 'dfs_next6' = " << typeid(dfs_next6_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next6>::type dfs_next6_deref;

	typedef BOOST_TYPEOF(dfs_next6_deref()) dfs_next6_deref_t;

	std::cout << "type of 'dfs_next6_deref' = " << typeid(dfs_next6_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next6>::type dfs_next7;

	typedef BOOST_TYPEOF(dfs_next7()) dfs_next7_t;

	//std::cout << "type of 'dfs_next7' = " << typeid(dfs_next7_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next7>::type dfs_next7_deref;

	typedef BOOST_TYPEOF(dfs_next7_deref()) dfs_next7_deref_t;

	std::cout << "type of 'dfs_next7_deref' = " << typeid(dfs_next7_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next7>::type dfs_next8;

	typedef BOOST_TYPEOF(dfs_next8()) dfs_next8_t;

	//std::cout << "type of 'dfs_next8' = " << typeid(dfs_next8_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next8>::type dfs_next8_deref;

	typedef BOOST_TYPEOF(dfs_next8_deref()) dfs_next8_deref_t;

	std::cout << "type of 'dfs_next8_deref' = " << typeid(dfs_next8_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<dfs_next8>::type dfs_next9;

	typedef BOOST_TYPEOF(dfs_next9()) dfs_next9_t;

	//std::cout << "type of 'dfs_next9' = " << typeid(dfs_next9_t).name() << std::endl;

	typedef BOOST_TYPEOF((typename dfs_next9::vertex_trace())) dfs_next9_vertices_t;

	//std::cout << "type of 'dfs_next9::vertex_trace' = " << typeid(dfs_next9_vertices_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<dfs_next9>::type dfs_next9_deref;

	typedef BOOST_TYPEOF(dfs_next9_deref()) dfs_next9_deref_t;

	std::cout << "type of 'dfs_next9_deref' = " << typeid(dfs_next9_deref_t).name() << std::endl;

	// print trace
	std::cout << "trace = " << typeid(dfs_next9_vertices_t).name() << std::endl;
}

void perform_dfs_iteration_to_end_of_path_with_trace()
{
	std::cout << "perform DFS iteration to end of path with trace of vertices" << std::endl;

	perform_dfs_iteration_to_end_of_path< ::boost::mgl::RecordTrace< ::boost::mpl::vector0<> > >();
}

void perform_dfs_iteration_to_end_of_path_without_trace()
{
	std::cout << "perform DFS iteration to end of path without any trace" << std::endl;

	perform_dfs_iteration_to_end_of_path< ::boost::mgl::NoTrace>();
}

template<class TracePolicy>
void perform_bfs_iteration()
{
	// get bfs-iterator to begin of graph
	typedef typename bfs_begin<simple_directed, TracePolicy>::type bfs_begin;

	typedef BOOST_TYPEOF(bfs_begin()) bfs_begin_t;

	//std::cout << "type of 'bfs_begin' = " << typeid(bfs_begin_t).name() << std::endl;

	typedef BOOST_TYPEOF((typename bfs_begin::vertex_trace())) bfs_begin_vertices_t;

	//std::cout << "type of 'bfs_begin::vertex_trace' = " << typeid(bfs_begin_vertices_t).name() << std::endl;

	// dereference iterator to begin to get first vertex of graph
	typedef typename deref<bfs_begin>::type bfs_begin_deref;

	typedef BOOST_TYPEOF(bfs_begin_deref()) bfs_begin_deref_t;

	std::cout << "type of 'bfs_begin_deref' = " << typeid(bfs_begin_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<bfs_begin>::type bfs_next1;

	typedef BOOST_TYPEOF(bfs_next1()) bfs_next1_t;

	//std::cout << "type of 'dfs_next1' = " << typeid(bfs_next1_t).name() << std::endl;

	typedef BOOST_TYPEOF((typename bfs_next1::vertex_trace())) bfs_next1_vertices_t;

	//std::cout << "type of 'dfs_next1::vertex_trace' = " << typeid(bfs_next1_vertices_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<bfs_next1>::type bfs_next1_deref;

	typedef BOOST_TYPEOF(bfs_next1_deref()) bfs_next1_deref_t;

	std::cout << "type of 'bfs_next1_deref' = " << typeid(bfs_next1_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<bfs_next1>::type bfs_next2;

	typedef BOOST_TYPEOF(bfs_next2()) bfs_next2_t;

	//std::cout << "type of 'dfs_next2' = " << typeid(bfs_next2_t).name() << std::endl;

	typedef BOOST_TYPEOF((typename bfs_next2::vertex_trace())) bfs_next2_vertices_t;

	//std::cout << "type of 'dfs_next2::vertex_trace' = " << typeid(bfs_next2_vertices_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<bfs_next2>::type bfs_next2_deref;

	typedef BOOST_TYPEOF(bfs_next2_deref()) bfs_next2_deref_t;

	std::cout << "type of 'bfs_next2_deref' = " << typeid(bfs_next2_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<bfs_next2>::type bfs_next3;

	typedef BOOST_TYPEOF(bfs_next3()) bfs_next3_t;

	//std::cout << "type of 'dfs_next3' = " << typeid(bfs_next3_t).name() << std::endl;

	typedef BOOST_TYPEOF((typename bfs_next3::vertex_trace())) bfs_next3_vertices_t;

	//std::cout << "type of 'dfs_next3::vertex_trace' = " << typeid(bfs_next3_vertices_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<bfs_next3>::type bfs_next3_deref;

	typedef BOOST_TYPEOF(bfs_next3_deref()) bfs_next3_deref_t;

	std::cout << "type of 'bfs_next3_deref' = " << typeid(bfs_next3_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<bfs_next3>::type bfs_next4;

	typedef BOOST_TYPEOF(bfs_next4()) bfs_next4_t;

	//std::cout << "type of 'dfs_next4' = " << typeid(bfs_next4_t).name() << std::endl;

	typedef BOOST_TYPEOF((typename bfs_next4::vertex_trace())) bfs_next4_vertices_t;

	//std::cout << "type of 'dfs_next4::vertex_trace' = " << typeid(bfs_next4_vertices_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<bfs_next4>::type bfs_next4_deref;

	typedef BOOST_TYPEOF(bfs_next4_deref()) bfs_next4_deref_t;

	std::cout << "type of 'bfs_next4_deref' = " << typeid(bfs_next4_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<bfs_next4>::type bfs_next5;

	typedef BOOST_TYPEOF(bfs_next5()) bfs_next5_t;

	//std::cout << "type of 'dfs_next5' = " << typeid(bfs_next5_t).name() << std::endl;

	typedef BOOST_TYPEOF((typename bfs_next5::vertex_trace())) bfs_next5_vertices_t;

	//std::cout << "type of 'dfs_next5::vertex_trace' = " << typeid(bfs_next5_vertices_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<bfs_next5>::type bfs_next5_deref;

	typedef BOOST_TYPEOF(bfs_next5_deref()) bfs_next5_deref_t;

	std::cout << "type of 'bfs_next5_deref' = " << typeid(bfs_next5_deref_t).name() << std::endl;

	// move iterator to next element
	typedef typename next<bfs_next5>::type bfs_next6;

	typedef BOOST_TYPEOF(bfs_next6()) bfs_next6_t;

	//std::cout << "type of 'dfs_next6' = " << typeid(bfs_next6_t).name() << std::endl;

	typedef BOOST_TYPEOF((typename bfs_next6::vertex_trace())) bfs_next6_vertices_t;

	//std::cout << "type of 'dfs_next6::vertex_trace' = " << typeid(bfs_next6_vertices_t).name() << std::endl;

	// dereference iterator to next element
	typedef typename deref<bfs_next6>::type bfs_next6_deref;

	typedef BOOST_TYPEOF(bfs_next6_deref()) bfs_next6_deref_t;

	std::cout << "type of 'bfs_next6_deref' = " << typeid(bfs_next6_deref_t).name() << std::endl;

	// print trace
	std::cout << "trace = " << typeid(bfs_next6_vertices_t).name() << std::endl;
}

void perform_bfs_iteration_with_trace()
{
	std::cout << "perform BFS iteration with trace of vertices" << std::endl;

	perform_bfs_iteration< ::boost::mgl::RecordTrace< ::boost::mpl::vector0<> > >();
}

void perform_bfs_iteration_without_trace()
{
	std::cout << "perform BFS iteration without any trace" << std::endl;

	perform_bfs_iteration< ::boost::mgl::NoTrace>();
}

int main(int /*argc*/, char * /*argv*/[])
{
	// graph has vertices 'a', 'b', 'c', 'd', 'e', 'f', 'g' and 'h'
	BOOST_MPL_ASSERT((mpl::equal<
		simple_directed::get_vertices::type,
		mpl::set<simple_directed::a, simple_directed::b, simple_directed::c,
			     simple_directed::d, simple_directed::e, simple_directed::f,
				 simple_directed::g, simple_directed::h> >
	));

	// vertex 'a' has property 'prop_v1'
	BOOST_MPL_ASSERT((mpl::equal<
		simple_directed::get_vertex_properties<simple_directed::a>::type,
		mpl::vector1<simple_directed::prop_v1> >
	));

	// vertex 'c' has no properties
	BOOST_MPL_ASSERT((mpl::equal<
		simple_directed::get_vertex_properties<simple_directed::c>::type,
		mpl::vector0<> >
	));

	// edge from 'b' to 'e' has no properties
	BOOST_MPL_ASSERT((mpl::equal<
		simple_directed::get_edge_properties<simple_directed::b, simple_directed::e>::type,
		mpl::vector0<> >
	));

	// edge from 'c' to 'd' has property 'prop_e2'
	BOOST_MPL_ASSERT((mpl::equal<
		simple_directed::get_edge_properties<simple_directed::c, simple_directed::d>::type,
		mpl::vector1<simple_directed::prop_e2> >
	));

	//// edge from 'a' to 'e' has no properties
	//BOOST_MPL_ASSERT((is_same<
	//	simple_directed::get_edge_properties<simple_directed::a, simple_directed::e>::type,
	//	mpl::void_>
	//));

	//typedef simple_directed::get_edge_properties<simple_directed::a, simple_directed::e>::type edge_ae;

	//typedef BOOST_TYPEOF(edge_ae()) edge_ae_t;

	//std::cout << "type of 'edge_ae' = " << typeid(edge_ae_t).name() << std::endl;

	// weight of edge from 'a' to 'b' is 3
	BOOST_MPL_ASSERT_RELATION(
		(simple_directed::get_edge_weight<simple_directed::a, simple_directed::b>::value), ==, 3);

	// weight of edge from 'b' to 'c' is 3
	BOOST_MPL_ASSERT_RELATION(
		(simple_directed::get_edge_weight<simple_directed::b, simple_directed::c>::value), ==, 3);

	//// find vertex 'd' beginning from vertex 'a'
	//BOOST_MPL_ASSERT((mpl::equal<
	//	simple_directed::find_vertex<simple_directed::a, simple_directed::d>::type,
	//	mpl::vector5<simple_directed::a, simple_directed::b, simple_directed::c, simple_directed::e, simple_directed::d> >
	//));

	// find vertex 'c' beginning from vertex 'a'
	BOOST_MPL_ASSERT((mpl::equal<
		simple_directed::find_vertex<simple_directed::a, simple_directed::c>::type,
		mpl::vector2<simple_directed::a, simple_directed::c> >
	));

	// test the DFS iteration to the end of the graph (all vertices has to be
	// visited) with trace of vertices
	perform_dfs_iteration_to_end_of_graph_with_trace();

	// test the DFS iteration to the end of the graph (all vertices has to be
	// visited) without trace of vertices
	perform_dfs_iteration_to_end_of_graph_without_trace();

	// test the DFS iteration until no vertex is reachable (there could be some
	// unvisited vertices at the end) with trace of vertices
	perform_dfs_iteration_to_end_of_path_with_trace();

	// test the DFS iteration until no vertex is reachable (there could be some
	// unvisited vertices at the end) without trace of vertices
	perform_dfs_iteration_to_end_of_path_without_trace();

	// test the BFS iteration with trace of vertices
	perform_bfs_iteration_with_trace();

	// test the BFS iteration without trace of vertices
	perform_bfs_iteration_without_trace();

	return 0;
}

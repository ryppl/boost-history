// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#undef  BOOST_MPL_LIMIT_METAFUNCTION_ARITY
#define BOOST_MPL_LIMIT_METAFUNCTION_ARITY 6
#include <boost/mpl/apply.hpp>

#include <boost/typeof/typeof.hpp>

#include <boost/type_traits/is_same.hpp>

#include <boost/mpl/vector.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/fold.hpp>
#include <boost/mpl/plus.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/placeholders.hpp>
#include <boost/mpl/greater.hpp>
#include <boost/mpl/comparison.hpp>
#include <boost/mpl/push_back.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/assert.hpp>

#include <boost/mgl/directed_graph.hpp>
#include <boost/mgl/depth_first_search.hpp>

using namespace boost;
using namespace boost::mgl;

namespace mgl = boost::mgl;

//! @todo Add description of the example.

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
struct v10 {};
struct v11 {};
struct v12 {};
struct v13 {};
struct v14 {};

class some_graph : public directed_graph<some_graph>
{
public:
    // adjacency list of the graph
    struct adjacency_list : mpl::vector15<
        //    Node   Adjacency
        //    from   nodes
        //  +------+--------------------------------------------+
        row < v0   , mpl::vector2<v13, v14>                     >,
        row < v1   , mpl::vector2<v13, v12>                     >,
        row < v2   , mpl::vector0<>                             >,
        row < v3   , mpl::vector2<v2, v12>                      >,
        row < v4   , mpl::vector3<v13, v8, v9>                  >,
        row < v5   , mpl::vector2<v13, v7>                      >,
        row < v6   , mpl::vector1<v2>                           >,
        row < v7   , mpl::vector3<v6, v8, v11>                  >,
        row < v8   , mpl::vector3<v12, v12, v2>                 >,
        row < v9   , mpl::vector1<v9>                           >,
        row < v10  , mpl::vector2<v5, v3>                       >,
        row < v11  , mpl::vector1<v11>                          >,
        row < v12  , mpl::vector2<v4, v7>                       >,
        row < v13  , mpl::vector3<v12, v9, v5>                  >,
        row < v14  , mpl::vector3<v0, v12, v14>                 >
        //  +------+--------------------------------------------+
    > {};
};

typedef mpl::vector5<
	v0,
	v9,
	v10,
	v12,
	v2
> source_vertices;

typedef v2 target_vertex;

typedef mpl::vector4<
	v0,
	v10,
	v12,
	v2
> reachable_vertices;

template<class V>
struct count_vertex_visitor
{
	typedef mpl::true_ discover_vertex;

	struct on_init
	{
		typedef mpl::int_<0> type;
	};

	template<typename Vertex, typename T, typename TraversalStack, typename ColorMap>
	struct on_discover_vertex
	{
		typedef typename mpl::if_<
			typename is_same<Vertex, V>::type,
			typename mpl::plus<T, mpl::int_<1> >::type,
			T
		>::type type;
	};
};

template<class Source, class Target>
struct is_found
{
	typedef typename mpl::greater<
		typename depth_first_search<
			some_graph,
			//count_vertex_visitor<Target>,
			::boost::mgl::null_visitor,
			//Source
			::boost::mpl::void_
		>::type::visitor_result,
		mpl::int_<0>
	>::type type;
};

typedef mpl::fold<
	source_vertices,
	mpl::vector0<>,
	mpl::if_<
		is_found<mpl::placeholders::_2, target_vertex>,
		mpl::push_back<mpl::placeholders::_1, mpl::placeholders::_2>,
		mpl::placeholders::_1
	>
>::type reached_vertices;

BOOST_MPL_ASSERT(( mpl::equal<reached_vertices, reachable_vertices>::type ));

int main(int argc, char * argv[])
{
	return 0;
}

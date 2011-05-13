// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

//! @todo Add your include statements here.

#include <iostream>
#include <typeinfo>

#include <boost/typeof/typeof.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/equal.hpp>
#include <boost/mpl/assert.hpp>

#include <boost/mgl/directed_graph.hpp>
#include <boost/mgl/depth_first_search.hpp>

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

class random_graph : public directed_graph<random_graph>
{
public:
    // vertex properties
    //! @todo Add your vertex properties here.

    // edge properties
    //! @todo Add your edge properties here.

    // adjacency list of the graph
    struct adjacency_list : mpl::vector10<
        //    Node   Adjacency
        //    from   nodes
        //  +------+--------------------------------------------+
        row < v0   , mpl::vector0<>                            >,
        row < v1   , mpl::vector2<v6, v6>                      >,
        row < v2   , mpl::vector1<v8>                          >,
        row < v3   , mpl::vector0<>                            >,
        row < v4   , mpl::vector0<>                            >,
        row < v5   , mpl::vector0<>                            >,
        row < v6   , mpl::vector3<v7, v5, v3>                  >,
        row < v7   , mpl::vector1<v3>                          >,
        row < v8   , mpl::vector2<v0, v2>                      >,
        row < v9   , mpl::vector1<v5>                          >
        //  +------+--------------------------------------------+
    > {};

    // vertex properties
    //! @todo Fill the structure with your properties from above.
    struct vertex_property_list : mpl::vector10<
        //       Node   Properties
        //     +------+-----------------------------------------+
        v_prop < v0   , mpl::vector0<>                          >,
        v_prop < v1   , mpl::vector0<>                          >,
        v_prop < v2   , mpl::vector0<>                          >,
        v_prop < v3   , mpl::vector0<>                          >,
        v_prop < v4   , mpl::vector0<>                          >,
        v_prop < v5   , mpl::vector0<>                          >,
        v_prop < v6   , mpl::vector0<>                          >,
        v_prop < v7   , mpl::vector0<>                          >,
        v_prop < v8   , mpl::vector0<>                          >,
        v_prop < v9   , mpl::vector0<>                          >
        //     +------+-----------------------------------------+
    > {};

    // edge properties and weights
    //! @todo Fill the structure with your properties from above and your own weights.
    struct edge_list : mpl::vector10<
        //     Node   Node   Weight   Properties
        //     from   to
        //   +------+------+--------+---------------------------+
        edge < v2   , v8   , 3      , mpl::vector0<>            >,
        //   +------+------+--------+---------------------------+
        edge < v1   , v6   , 3      , mpl::vector0<>            >,
        //   +------+------+--------+---------------------------+
        edge < v6   , v7   , 4      , mpl::vector0<>            >,
        //   +------+------+--------+---------------------------+
        edge < v9   , v5   , 8      , mpl::vector0<>            >,
        //   +------+------+--------+---------------------------+
        edge < v8   , v0   , 3      , mpl::vector0<>            >,
        //   +------+------+--------+---------------------------+
        edge < v6   , v5   , 2      , mpl::vector0<>            >,
        //   +------+------+--------+---------------------------+
        edge < v1   , v6   , 9      , mpl::vector0<>            >,
        //   +------+------+--------+---------------------------+
        edge < v8   , v2   , 10     , mpl::vector0<>            >,
        //   +------+------+--------+---------------------------+
        edge < v6   , v3   , 1      , mpl::vector0<>            >,
        //   +------+------+--------+---------------------------+
        edge < v7   , v3   , 5      , mpl::vector0<>            >
        //   +------+------+--------+---------------------------+
    > {};
};

// test vertices (in/out) and edges (in/out) of 'v0'
typedef random_graph::in_vertices<v0>::type in_vertices_v0;
typedef random_graph::in_edges<v0>::type in_edges_v0;
typedef random_graph::out_vertices<v0>::type out_vertices_v0;
typedef random_graph::out_edges<v0>::type out_edges_v0;
BOOST_MPL_ASSERT((mpl::equal<in_vertices_v0, mpl::vector1<v8> >::type));
BOOST_MPL_ASSERT((mpl::equal<in_edges_v0, mpl::vector1<edge<v8, v0, 3, mpl::vector0<> > > >::type));
BOOST_MPL_ASSERT((mpl::equal<out_vertices_v0, mpl::vector0<> >::type));
BOOST_MPL_ASSERT((mpl::equal<out_edges_v0, mpl::vector0<> >::type));

// test vertices (in/out) and edges (in/out) of 'v1'
typedef random_graph::in_vertices<v1>::type in_vertices_v1;
typedef random_graph::in_edges<v1>::type in_edges_v1;
typedef random_graph::out_vertices<v1>::type out_vertices_v1;
typedef random_graph::out_edges<v1>::type out_edges_v1;
BOOST_MPL_ASSERT((mpl::equal<in_vertices_v1, mpl::vector0<> >::type));
BOOST_MPL_ASSERT((mpl::equal<in_edges_v1, mpl::vector0<> >::type));
BOOST_MPL_ASSERT((mpl::equal<out_vertices_v1, mpl::vector2<v6, v6> >::type));
BOOST_MPL_ASSERT((mpl::equal<out_edges_v1, mpl::vector2<edge<v1, v6, 3, mpl::vector0<> >, edge<v1, v6, 9, mpl::vector0<> > > >::type));

// test vertices (in/out) and edges (in/out) of 'v2'
typedef random_graph::in_vertices<v2>::type in_vertices_v2;
typedef random_graph::in_edges<v2>::type in_edges_v2;
typedef random_graph::out_vertices<v2>::type out_vertices_v2;
typedef random_graph::out_edges<v2>::type out_edges_v2;
BOOST_MPL_ASSERT((mpl::equal<in_vertices_v2, mpl::vector1<v8> >::type));
BOOST_MPL_ASSERT((mpl::equal<in_edges_v2, mpl::vector1<edge<v8, v2, 10, mpl::vector0<> > > >::type));
BOOST_MPL_ASSERT((mpl::equal<out_vertices_v2, mpl::vector1<v8> >::type));
BOOST_MPL_ASSERT((mpl::equal<out_edges_v2, mpl::vector1<edge<v2, v8, 3, mpl::vector0<> > > >::type));

// test vertices (in/out) and edges (in/out) of 'v3'
typedef random_graph::in_vertices<v3>::type in_vertices_v3;
typedef random_graph::in_edges<v3>::type in_edges_v3;
typedef random_graph::out_vertices<v3>::type out_vertices_v3;
typedef random_graph::out_edges<v3>::type out_edges_v3;
BOOST_MPL_ASSERT((mpl::equal<in_vertices_v3, mpl::vector2<v6, v7> >::type));
BOOST_MPL_ASSERT((mpl::equal<in_edges_v3, mpl::vector2<edge<v6, v3, 1, mpl::vector0<> >, edge<v7, v3, 5, mpl::vector0<> > > >::type));
BOOST_MPL_ASSERT((mpl::equal<out_vertices_v3, mpl::vector0<> >::type));
BOOST_MPL_ASSERT((mpl::equal<out_edges_v3, mpl::vector0<> >::type));

// test vertices (in/out) and edges (in/out) of 'v4'
typedef random_graph::in_vertices<v4>::type in_vertices_v4;
typedef random_graph::in_edges<v4>::type in_edges_v4;
typedef random_graph::out_vertices<v4>::type out_vertices_v4;
typedef random_graph::out_edges<v4>::type out_edges_v4;
BOOST_MPL_ASSERT((mpl::equal<in_vertices_v4, mpl::vector0<> >::type));
BOOST_MPL_ASSERT((mpl::equal<in_edges_v4, mpl::vector0<> >::type));
BOOST_MPL_ASSERT((mpl::equal<out_vertices_v4, mpl::vector0<> >::type));
BOOST_MPL_ASSERT((mpl::equal<out_edges_v4, mpl::vector0<> >::type));

// test vertices (in/out) and edges (in/out) of 'v5'
typedef random_graph::in_vertices<v5>::type in_vertices_v5;
typedef random_graph::in_edges<v5>::type in_edges_v5;
typedef random_graph::out_vertices<v5>::type out_vertices_v5;
typedef random_graph::out_edges<v5>::type out_edges_v5;
BOOST_MPL_ASSERT((mpl::equal<in_vertices_v5, mpl::vector2<v9, v6> >::type));
BOOST_MPL_ASSERT((mpl::equal<in_edges_v5, mpl::vector2<edge<v9, v5, 8, mpl::vector0<> >, edge<v6, v5, 2, mpl::vector0<> > > >::type));
BOOST_MPL_ASSERT((mpl::equal<out_vertices_v5, mpl::vector0<> >::type));
BOOST_MPL_ASSERT((mpl::equal<out_edges_v5, mpl::vector0<> >::type));

// test vertices (in/out) and edges (in/out) of 'v6'
typedef random_graph::in_vertices<v6>::type in_vertices_v6;
typedef random_graph::in_edges<v6>::type in_edges_v6;
typedef random_graph::out_vertices<v6>::type out_vertices_v6;
typedef random_graph::out_edges<v6>::type out_edges_v6;
BOOST_MPL_ASSERT((mpl::equal<in_vertices_v6, mpl::vector2<v1, v1> >::type));
BOOST_MPL_ASSERT((mpl::equal<in_edges_v6, mpl::vector2<edge<v1, v6, 3, mpl::vector0<> >, edge<v1, v6, 9, mpl::vector0<> > > >::type));
BOOST_MPL_ASSERT((mpl::equal<out_vertices_v6, mpl::vector3<v7, v5, v3> >::type));
BOOST_MPL_ASSERT((mpl::equal<out_edges_v6, mpl::vector3<edge<v6, v7, 4, mpl::vector0<> >, edge<v6, v5, 2, mpl::vector0<> >, edge<v6, v3, 1, mpl::vector0<> > > >::type));

// test vertices (in/out) and edges (in/out) of 'v7'
typedef random_graph::in_vertices<v7>::type in_vertices_v7;
typedef random_graph::in_edges<v7>::type in_edges_v7;
typedef random_graph::out_vertices<v7>::type out_vertices_v7;
typedef random_graph::out_edges<v7>::type out_edges_v7;
BOOST_MPL_ASSERT((mpl::equal<in_vertices_v7, mpl::vector1<v6> >::type));
BOOST_MPL_ASSERT((mpl::equal<in_edges_v7, mpl::vector1<edge<v6, v7, 4, mpl::vector0<> > > >::type));
BOOST_MPL_ASSERT((mpl::equal<out_vertices_v7, mpl::vector1<v3> >::type));
BOOST_MPL_ASSERT((mpl::equal<out_edges_v7, mpl::vector1<edge<v7, v3, 5, mpl::vector0<> > > >::type));

// test vertices (in/out) and edges (in/out) of 'v8'
typedef random_graph::in_vertices<v8>::type in_vertices_v8;
typedef random_graph::in_edges<v8>::type in_edges_v8;
typedef random_graph::out_vertices<v8>::type out_vertices_v8;
typedef random_graph::out_edges<v8>::type out_edges_v8;
BOOST_MPL_ASSERT((mpl::equal<in_vertices_v8, mpl::vector1<v2> >::type));
BOOST_MPL_ASSERT((mpl::equal<in_edges_v8, mpl::vector1<edge<v2, v8, 3, mpl::vector0<> > > >::type));
BOOST_MPL_ASSERT((mpl::equal<out_vertices_v8, mpl::vector2<v0, v2> >::type));
BOOST_MPL_ASSERT((mpl::equal<out_edges_v8, mpl::vector2<edge<v8, v0, 3, mpl::vector0<> >, edge<v8, v2, 10, mpl::vector0<> > > >::type));

// test vertices (in/out) and edges (in/out) of 'v9'
typedef random_graph::in_vertices<v9>::type in_vertices_v9;
typedef random_graph::in_edges<v9>::type in_edges_v9;
typedef random_graph::out_vertices<v9>::type out_vertices_v9;
typedef random_graph::out_edges<v9>::type out_edges_v9;
BOOST_MPL_ASSERT((mpl::equal<in_vertices_v9, mpl::vector0<> >::type));
BOOST_MPL_ASSERT((mpl::equal<in_edges_v9, mpl::vector0<> >::type));
BOOST_MPL_ASSERT((mpl::equal<out_vertices_v9, mpl::vector1<v5> >::type));
BOOST_MPL_ASSERT((mpl::equal<out_edges_v9, mpl::vector1<edge<v9, v5, 8, mpl::vector0<> > > >::type));

int main(int argc, char * argv[])
{
    // perform a DFS search
	typedef depth_first_search<random_graph>::type::vertex_trace dfs;

    // get type of the DFS result
    typedef BOOST_TYPEOF(dfs()) dfs_t;

    // print vertices with DFS result to stdout
    std::cout << typeid(dfs_t).name() << std::endl;

    return 0;
}

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

#include <boost/mpl/bool.hpp>

#include <boost/mgl/directed_graph.hpp>
#include <boost/mgl/contains_cycle.hpp>

#include <boost/typeof/typeof.hpp>

#define BOOST_TEST_MODULE "Contains Cycle Test"
#include <boost/test/unit_test.hpp>

using namespace boost;
using namespace boost::mgl;

namespace mgl = boost::mgl;

// vertices
struct a {};
struct b {};
struct c {};
struct d {};

class cycle_graph : public directed_graph<cycle_graph>
{
public:
    // adjacency list of the graph
    struct adjacency_list : mpl::vector4<
        //    Node   Adjacency
        //    from   nodes
        //  +------+--------------------------------------------+
        row < a    , mpl::vector1<d>                            >,
        row < b    , mpl::vector0<>                             >,
        row < c    , mpl::vector2<a, b>                         >,
        row < d    , mpl::vector2<c, b>                         >
        //  +------+--------------------------------------------+
    > {};
};

class non_cycle_graph : public directed_graph<non_cycle_graph>
{
public:
    // adjacency list of the graph
    struct adjacency_list : mpl::vector4<
        //    Node   Adjacency
        //    from   nodes
        //  +------+--------------------------------------------+
        row < a    , mpl::vector1<d>                            >,
        row < b    , mpl::vector0<>                             >,
        row < c    , mpl::vector1<b>                            >,
        row < d    , mpl::vector2<c, b>                         >
        //  +------+--------------------------------------------+
    > {};
};

BOOST_AUTO_TEST_CASE(test_cycle_graph)
{
	typedef BOOST_TYPEOF(mpl::true_()) true_t;

	typedef contains_cycle<cycle_graph>::type result;
	typedef BOOST_TYPEOF(result()) result_t;

	BOOST_CHECK_MESSAGE(typeid(result_t) == typeid(true_t), "contains_cycle<> should return a proper ::mpl::true_");
}

BOOST_AUTO_TEST_CASE(test_non_cycle_graph)
{
	typedef BOOST_TYPEOF(mpl::false_()) false_t;

	typedef contains_cycle<non_cycle_graph>::type result;
	typedef BOOST_TYPEOF(result()) result_t;

	BOOST_CHECK_MESSAGE(typeid(result_t) == typeid(false_t), "contains_cycle<> should return a proper ::mpl::false_");
}

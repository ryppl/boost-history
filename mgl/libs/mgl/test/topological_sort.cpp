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

#include <boost/mpl/equal.hpp>

#include <boost/mgl/directed_graph.hpp>
#include <boost/mgl/topological_sort.hpp>

#define BOOST_TEST_MODULE "Topological Sort Test"
#include <boost/test/unit_test.hpp>

using namespace boost;
using namespace boost::mgl;

namespace mgl = boost::mgl;

// vertices
struct a {};
struct b {};
struct c {};
struct d {};
struct e {};
struct f {};

class some_graph : public directed_graph<some_graph>
{
public:
    // adjacency list of the graph
    struct adjacency_list : mpl::vector6<
        //    Node   Adjacency
        //    from   nodes
        //  +------+--------------------------------------------+
        row < a    , mpl::vector1<b>                            >,
		row < b    , mpl::vector1<c>                            >,
		row < c    , mpl::vector1<d>                            >,
		row < d    , mpl::vector0<>                             >,
		row < e    , mpl::vector2<b, d>                         >,
		row < f    , mpl::vector1<c>                            >
        //  +------+--------------------------------------------+
    > {};
};

BOOST_AUTO_TEST_CASE(check_topological_sort)
{
	typedef ::boost::mpl::vector6<a, f, e, b, c, d> expected_result;
	typedef BOOST_TYPEOF(expected_result()) expected_result_t;

	typedef topological_sort<some_graph>::type result;
	typedef BOOST_TYPEOF(result()) result_t;

	BOOST_CHECK_MESSAGE(typeid(result_t) == typeid(expected_result_t), "topological_sort<> should return a proper ::mpl::vector<>");
}

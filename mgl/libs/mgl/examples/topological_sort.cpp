// Copyright Franz Alt 2009-2011
//
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or copy at 
// http://www.boost.org/LICENSE_1_0.txt)

#include <iostream>
#include <typeinfo>

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#undef  BOOST_MPL_LIMIT_METAFUNCTION_ARITY
#define BOOST_MPL_LIMIT_METAFUNCTION_ARITY 6
#include <boost/mpl/apply.hpp>

#include <boost/typeof/typeof.hpp>

#include <boost/mgl/directed_graph.hpp>
#include <boost/mgl/topological_sort.hpp>

using namespace std;

using namespace boost;
using namespace boost::mgl;

namespace mgl = boost::mgl;

// vertices
struct pants {};
struct sox {};
struct pullover {};
struct trousers {};
struct cloak {};
struct shoes {};
struct shirt {};

class adduction_graph : public directed_graph<adduction_graph>
{
public:
    // adjacency list of the graph
    struct adjacency_list : mpl::vector7<
        //    Node       Adjacency
        //    from       nodes
        //  +----------+----------------------------------------+
		row < pants    , mpl::vector1<trousers>                 >,
		row < sox      , mpl::vector1<shoes>                    >,
		row < pullover , mpl::vector1<cloak>                    >,
		row < trousers , mpl::vector2<cloak, shoes>             >,
		row < cloak    , mpl::vector0<>                         >,
		row < shoes    , mpl::vector0<>                         >,
		row < shirt    , mpl::vector1<pullover>                 >
        //  +----------+----------------------------------------+
    > {};
};

int main(int /*argc*/, char * /*argv*/[])
{
	typedef topological_sort<adduction_graph>::type graph;

	typedef BOOST_TYPEOF(graph()) graph_t;

	cout << typeid(graph_t).name() << endl << endl;

	return 0;
}

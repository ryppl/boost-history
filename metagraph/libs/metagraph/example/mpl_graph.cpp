// mplgraph.cpp : Defines the entry point for the console application.
//

#include <boost/metagraph/mpl_graph.h>
#include <boost/mpl/print.hpp>

namespace mpl_graph = boost::metagraph::mpl_graph;
namespace mpl = boost::mpl;
/* can't at the moment think of a cute graph example so the following abstract
    and poorly drawn
    A -> B -> C -\--> D
           \     |--> E
            \    \--> F
             \-----/
*/           

// vertices
struct A{}; struct B{}; struct C{}; struct D{}; struct E{}; struct F{};

// edges
struct A_B{}; struct B_C{}; struct C_D{}; struct C_E{}; struct C_F{}; struct B_F{};

typedef mpl::vector<mpl::vector<A_B,A,B>,
               mpl::vector<B_C,B,C>,
               mpl::vector<C_D,C,D>,
               mpl::vector<C_E,C,E>,
               mpl::vector<C_F,C,F>,
               mpl::vector<B_F,B,F> >
    some_edge_sequence;
typedef mpl_graph::edgeseq_graph<some_edge_sequence> some_graph;

BOOST_MPL_ASSERT(( boost::is_same<mpl_graph::source<B_C,some_graph>::type, B> ));
BOOST_MPL_ASSERT(( boost::is_same<mpl_graph::source<C_D,some_graph>::type, C> ));

BOOST_MPL_ASSERT(( boost::is_same<mpl_graph::target<C_D,some_graph>::type, D> ));
BOOST_MPL_ASSERT(( boost::is_same<mpl_graph::target<B_F,some_graph>::type, F> ));

// shouldn't assume the order but this seems to work
BOOST_MPL_ASSERT(( mpl::equal<mpl_graph::out_edges<C,some_graph>::type, mpl::vector<C_D,C_E,C_F> > ));
BOOST_MPL_ASSERT(( mpl::equal<mpl_graph::out_edges<B,some_graph>::type, mpl::vector<B_C,B_F> > ));

BOOST_MPL_ASSERT(( mpl::equal<mpl_graph::in_edges<C,some_graph>::type, mpl::vector<B_C> > ));
BOOST_MPL_ASSERT(( mpl::equal<mpl_graph::in_edges<F,some_graph>::type, mpl::vector<C_F,B_F> > ));

BOOST_MPL_ASSERT_RELATION( (mpl_graph::out_degree<B,some_graph>::value), ==, 2 );
BOOST_MPL_ASSERT_RELATION( (mpl_graph::out_degree<C,some_graph>::value), ==, 3 );

BOOST_MPL_ASSERT_RELATION( (mpl_graph::in_degree<A,some_graph>::value), ==, 0 );
BOOST_MPL_ASSERT_RELATION( (mpl_graph::in_degree<F,some_graph>::value), ==, 2 );

BOOST_MPL_ASSERT_RELATION( (mpl_graph::degree<A,some_graph>::value), ==, 1 );
BOOST_MPL_ASSERT_RELATION( (mpl_graph::degree<C,some_graph>::value), ==, 4 );

BOOST_MPL_ASSERT(( mpl::equal<mpl_graph::adjacent_vertices<A,some_graph>::type, mpl::vector<B> > ));
BOOST_MPL_ASSERT(( mpl::equal<mpl_graph::adjacent_vertices<C,some_graph>::type, mpl::vector<D,E,F> > ));

// this doesn't work because equal is ordered
//BOOST_MPL_ASSERT(( equal<mpl_graph::vertices<some_graph>::type, set<A,B,C,D,E,F> > ));

BOOST_MPL_ASSERT_RELATION( mpl_graph::num_vertices<some_graph>::value, ==, 6 );

BOOST_MPL_ASSERT_RELATION( mpl_graph::num_edges<some_graph>::value, ==, 6 );


int main(int argc, char* argv[])
{
    return 0;
}


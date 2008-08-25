#include <boost/metagraph/mpl_graph/dfs.hpp>
#include <boost/mpl/print.hpp>

#include <iostream>

namespace mpl_graph = boost::metagraph::mpl_graph;
namespace mpl = boost::mpl;

/* 
    test graph:
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

struct preordering : mpl_graph::dfs_default_visitor_operations {    
    template<typename Node, typename Graph, typename State>
    struct discover_vertex :
        mpl::push_back<State, Node>
    {};
};

struct postordering : mpl_graph::dfs_default_visitor_operations {    
    template<typename Node, typename Graph, typename State>
    struct finish_vertex :
        mpl::push_back<State, Node>
    {};
};

template<typename T> struct incomplete;

int main() {
    typedef mpl::first<mpl_graph::dfs_visit<some_graph,A,mpl_graph::state_and_operations<mpl::vector<>, preordering > >::type>::type preorder;
    BOOST_MPL_ASSERT(( mpl::equal<preorder::type, mpl::vector<A,B,C,D,E,F> > ));
    
    typedef mpl::first<mpl_graph::dfs_visit<some_graph,A,mpl_graph::state_and_operations<mpl::vector<>, postordering > >::type>::type postorder;
    BOOST_MPL_ASSERT(( mpl::equal<postorder::type, mpl::vector<D,E,F,C,B,A> > ));
    return 0;
}
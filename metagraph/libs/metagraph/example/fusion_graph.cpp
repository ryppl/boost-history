#include <boost/metagraph/fusion_graph/fusion_graph.hpp>
#include <boost/mpl/print.hpp>

namespace fusion_graph = boost::metagraph::fusion_graph;
namespace fusion = boost::fusion;
namespace mpl_graph = boost::metagraph::mpl_graph;
namespace mpl = boost::mpl;

struct G : fusion_graph::mapper_vertex<int> {};
struct N : fusion_graph::mapper_vertex<int> {};
struct E : fusion_graph::mapper_vertex<int> {};

struct G_N : fusion_graph::ptr_list_edge<int> {};
struct N_E : fusion_graph::ptr_list_edge<int> {};
struct E_N : fusion_graph::ptr_edge<int> {};
struct E_T : E_N {};
struct E_S : E_N {};

struct graph_desc : 
    mpl::vector<
        mpl::vector<G_N,G,N>,
        mpl::vector<N_E,N,E>,
        mpl::vector<E_T,E,N>,
        mpl::vector<E_S,E,N> 
        >
{};
struct graph_desc_graph :
    mpl_graph::edgeseq_graph<graph_desc>
{};

typedef fusion_graph::make_fusion_graph<graph_desc_graph>::type graphy;

typedef graphy::vertex_impl<G>::type Graph;
typedef graphy::vertex_impl<N>::type Node;
typedef graphy::vertex_impl<E>::type Edge;

mpl::print<Graph>::type foo;

int main(int narg, char *args[]) {
    Graph *g = new Graph;
    Node *n1 = new Node, *n2 = new Node;
    Edge *e1 = new Edge;
    
    fusion::at_key<G_N>(g->edges).link.push_back(n1);
    fusion::at_key<G_N>(g->edges).link.push_back(n2);
    fusion::at_key<N_E>(n1->edges).link.push_back(e1);
    fusion::at_key<E_S>(e1->edges).link = n1;
    fusion::at_key<E_T>(e1->edges).link = n2;
    
    // NO (generates horribly incomprehensible messages
    //fusion::at_key<N_E>(g->edges).link.push_back(n1);

}

#include <iostream>
#include <string>

#include <boost/graphs/adjacency_list/vertex_store.hpp>
#include <boost/graphs/adjacency_list/edge_store.hpp>

#include "typestr.hpp"

using namespace std;
using namespace boost;
using namespace boost::graphs;
using namespace boost::graphs::adjacency_list;

template <typename Store>
void test()
{
    typedef typename edge_store_traits<Store>::edge_type Edge;
    typedef typename edge_traits<Edge>::edge_ends Ends;
    typedef typename descriptor_traits<Store>::descriptor_type EdgeDesc;

    Store store;

    EdgeDesc e1 = es::insert(store, Ends(0, 1), 0.5);
    EdgeDesc e2 = es::insert(store, Ends(0, 2), 1.5);
    BOOST_ASSERT(es::size(store) == 2u);
    cout << es::label(store, e1) << endl;
    cout << es::label(store, e2) << endl;
}

int main()
{
    typedef vertex_vector<>::vertex_descriptor VertexDesc;
    typedef std::pair<VertexDesc, VertexDesc> Ends;

    typedef edge_vector<>::edge_store<Ends, double>::type EV;
    typedef edge_set<>::edge_store<Ends, double>::type EM;

    test<EV>();
    test<EM>();
}

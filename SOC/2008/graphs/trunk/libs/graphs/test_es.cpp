
#include <iostream>

#include <boost/next_prior.hpp>
#include <boost/graphs/edge_vector.hpp>

#include "typestr.hpp"

using namespace std;
using namespace boost;

// These are things that we can't do much about...
typedef int VertexProps;
typedef int EdgeProps;
typedef index_descriptor<size_t> VertexDesc;
typedef index_descriptor<size_t> IncDesc;

template <typename EdgeStore>
void
undirected()
{
    cout << "--- " << typestr<EdgeStore>() << " ---" << endl;

    // Instantiate data structures related to the storage of edges and their
    // properties.
    typedef typename EdgeStore::template property_store<EdgeProps, IncDesc>::type PropStore;
    typedef typename EdgeStore::template incidence_store<EdgeProps, IncDesc>::type IncStore;
    cout << "  * " << typestr<PropStore>() << endl;
    cout << "  * " << typestr<IncStore>() << endl;
}

int main()
{
    undirected<edge_vector<>>();

    return 0;
}
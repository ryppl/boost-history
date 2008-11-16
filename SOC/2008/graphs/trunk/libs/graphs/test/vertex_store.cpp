
#include <iostream>

#include <boost/graphs/adjacency_list/vertex_store.hpp>
#include <boost/graphs/adjacency_list/vertex_vector.hpp>
#include <boost/graphs/adjacency_list/vertex_list.hpp>

using namespace std;
using namespace boost;
using namespace boost::graphs::adjacency_list;

struct my_vertex
{ };

template <typename Store>
void test()
{
    typedef typename Store::value_type Vertex;
    typedef typename descriptor_traits<Store>::descriptor_type VertexDesc;

    Store store;

    VertexDesc u = add_vertex_to_store(store, Vertex());
    VertexDesc v = add_vertex_to_store(store, Vertex());
    cout << u << " " << v << endl;
}

int main()
{
    typedef vertex_vector<>::vertex_store<my_vertex>::type VV;
    typedef vertex_list<>::vertex_store<my_vertex>::type VL;

    test<VV>();
    test<VL>();
}


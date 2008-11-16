
#include <iostream>
#include <string>

#include <boost/graphs/adjacency_list/vertex_store.hpp>
#include <boost/graphs/adjacency_list/vertex_vector.hpp>
#include <boost/graphs/adjacency_list/vertex_list.hpp>
#include <boost/graphs/adjacency_list/vertex_set.hpp>
#include <boost/graphs/adjacency_list/vertex_map.hpp>

using namespace std;
using namespace boost;
using namespace boost::graphs::adjacency_list;

struct my_vertex
{
    typedef string label_type;  // Adaptor for vertex_traits

    my_vertex(string const& x)
        : _label(x)
    { }

    string& label()                 { return _label; }
    string const& label() const     { return _label; }

    string _label;
};

template <typename Store>
void test()
{
    typedef typename Store::value_type Vertex;
    typedef typename descriptor_traits<Store>::descriptor_type VertexDesc;
    typedef typename Store::iterator StoreIterator;

    Store store;

    // Add some vertices
    vs_add_vertex(store, Vertex("b"));
    vs_add_vertex(store, Vertex("a"));

    // Find a verts
    StoreIterator i = vs_find_vertex(store, string("b"));
    cout << "test: " << distance(store.begin(), i) << endl;
}

int main()
{
    typedef vertex_vector<>::vertex_store<my_vertex>::type VV;
    typedef vertex_list<>::vertex_store<my_vertex>::type VL;
    typedef vertex_set<>::vertex_store<my_vertex>::type VS;
    // typedef vertex_map<>::vertex_map<string, my_vertex>::type VM;

    test<VV>();
    test<VL>();
    test<VS>();
}


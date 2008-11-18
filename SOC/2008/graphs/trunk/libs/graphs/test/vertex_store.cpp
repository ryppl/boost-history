
#include <iostream>
#include <string>

#include <boost/graphs/adjacency_list/vertex_store.hpp>
#include <boost/graphs/adjacency_list/vertex_vector.hpp>
#include <boost/graphs/adjacency_list/vertex_list.hpp>
#include <boost/graphs/adjacency_list/vertex_set.hpp>
#include <boost/graphs/adjacency_list/vertex_map.hpp>

#include "typestr.hpp"


using namespace std;
using namespace boost;
using namespace boost::graphs::adjacency_list;

struct my_vertex
{
    typedef string label_type;  // Adaptor for vertex_traits

    my_vertex()
        : _label()
    { }

    my_vertex(string const& x)
        : _label(x)
    { }

    string& label()                 { return _label; }
    string const& label() const     { return _label; }

    string _label;
};

struct simple_container_tag : virtual sequence_tag, virtual simple_associative_container_tag { };
struct pair_container_tag : pair_associative_container_tag { };

simple_container_tag container_arity(vector_tag) { return simple_container_tag(); }
simple_container_tag container_arity(list_tag) { return simple_container_tag(); }
simple_container_tag container_arity(set_tag) { return simple_container_tag(); }
pair_container_tag container_arity(map_tag) { return pair_container_tag(); }

template <typename Store>
void add_verts(Store& store, simple_container_tag)
{
    typedef typename Store::iterator Iterator;
    typedef typename Store::value_type Vertex;
    vs_add_vertex(store, Vertex("b"));
    Iterator iter = vs_add_vertex(store, Vertex("a"));

    vs_remove_vertex(store, iter);

    // Try to re-add a vertex.
    pair<Iterator, bool> i = vs_try_add_vertex(store, Vertex("b"));
    cout << "re-add: " << i.second << "\n";
}

template <typename Store>
void add_verts(Store& store, pair_container_tag)
{
    typedef typename Store::iterator Iterator;
    typedef typename Store::mapped_type Vertex;
    vs_add_vertex(store, "a", Vertex());
    vs_add_vertex(store, "b", Vertex());

    // Try to re-add a vertex.
    pair<Iterator, bool> i = vs_try_add_vertex(store, string("b"), Vertex());
    cout << "re-add: " << i.second << "\n";
}

template <typename Store>
void test()
{
    typedef typename Store::value_type Vertex;
    typedef typename descriptor_traits<Store>::descriptor_type VertexDesc;
    typedef typename Store::iterator StoreIterator;

    Store store;

    add_verts(store, container_arity(container_category(store)));

    // Find a verts
    StoreIterator i = vs_find_vertex(store, string("b"));
    cout << "test: " << distance(store.begin(), i) << endl;
}

int main()
{
    typedef vertex_vector<>::vertex_store<my_vertex>::type VV;
    typedef vertex_list<>::vertex_store<my_vertex>::type VL;
    typedef vertex_set<>::vertex_store<my_vertex>::type VS;
    typedef vertex_map<string>::vertex_store<my_vertex>::type VM;

    // test<VV>();
    test<VL>();
    test<VS>();
    test<VM>();
}


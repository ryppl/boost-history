
#include <iostream>
#include <string>

#include <boost/graphs/adjacency_list/vertex_store.hpp>

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
    typedef typename descriptor_traits<Store>::descriptor_type Descriptor;
    typedef typename vertex_store_traits<Store>::vertex_type Vertex;
    vs::insert(store, Vertex("b"));
    vs::insert(store, Vertex("a"));

    // Try to re-add a vertex.
    pair<Descriptor, bool> i = vs::try_insert(store, Vertex("b"));
    BOOST_ASSERT(!i.second);
}

template <typename Store>
void add_verts(Store& store, pair_container_tag)
{
    typedef typename descriptor_traits<Store>::descriptor_type Descriptor;
    typedef typename vertex_store_traits<Store>::vertex_type Vertex;
    vs::insert(store, "a", Vertex());
    vs::insert(store, "b", Vertex());

    // Try to re-add a vertex.
    pair<Descriptor, bool> i = vs::try_insert(store, string("b"), Vertex());
    BOOST_ASSERT(!i.second);
}

template <typename Store, typename Tag>
void remove_vert(Store& store, Tag t)
{
    typedef typename descriptor_traits<Store>::descriptor_type Descriptor;
    Descriptor d = vs::find(store, string("b"));
    BOOST_ASSERT(d);
    size_t n = vs::size(store);
    vs::remove(store, d);
    BOOST_ASSERT(vs::size(store) == n - 1);
}

// No-op for vectors... Can't remove vertices.
template <typename Store>
void remove_vert(Store& store, vector_tag)
{ }

template <typename Store>
void test()
{
    typedef typename Store::value_type Vertex;
    typedef typename descriptor_traits<Store>::descriptor_type Descriptor;

    Store store;

    add_verts(store, container_arity(container_category(store)));
    BOOST_ASSERT(!vs::empty(store));
    BOOST_ASSERT(vs::size(store));

    // Find a verts
    Descriptor d = vs::find(store, string("b"));
    BOOST_ASSERT(d);

    // Check the removal of vertices
    remove_vert(store, container_category(store));
}

int main()
{
    typedef vertex_vector<>::vertex_store<my_vertex>::type VV;
    typedef vertex_list<>::vertex_store<my_vertex>::type VL;
    typedef vertex_set<>::vertex_store<my_vertex>::type VS;
    typedef vertex_map<string>::vertex_store<my_vertex>::type VM;

    test<VV>();
    test<VL>();
    test<VS>();
    test<VM>();
}


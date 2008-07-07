
#include <iostream>

#include <boost/graphs/vertex_vector.hpp>
#include <boost/graphs/vertex_list.hpp>
#include <boost/graphs/vertex_set.hpp>
#include <boost/graphs/vertex_map.hpp>

#include "demangle.hpp"

using namespace std;
using namespace boost;

struct no_remove_tag { };
struct has_remove_tag { };

// A fake vertex type.
struct Vertex
{
    typedef int vertex_properties;

    Vertex(int x)
        : value(x)
    { }

    int& properties()
    { return value; }

    int const& properties() const
    { return value; }

    int value;
};

template <typename Store>
void populate(Store& verts, sequence_tag)
{
    for(int i = 0; i < 5; ++i) {
        verts.add(i);
    }
}

template <typename Store>
void populate(Store& verts, simple_associative_container_tag)
{
    for(int i = 0; i < 5; ++i) {
        verts.add(i);
    }
}

template <typename Store>
void populate(Store& verts, pair_associative_container_tag)
{
    for(int i = 0; i < 5; ++i) {
        verts.add(i, 2 * i);
    }
}

template <typename Store>
void test_remove(Store& verts, stable_descriptor_tag)
{
    verts.remove(3);
    cout << "num verts after remove: " << verts.size() << endl;
}

template <typename VertexSet>
void test_remove(VertexSet& vs, unstable_remove_tag)
{ /* Noop for unstable removes */ }

template <typename VertexSet>
void test()
{
    typedef typename VertexSet::template store<Vertex>::type Store;
    typedef typename VertexSet::descriptor_type Descriptor;
    typedef typename Store::store_type StoreImpl;

    cout << "--- " << demangle<VertexSet>() << " ---" << endl;

    Store verts;

    populate(verts, typename container_traits<StoreImpl>::category());
    cout << "num verts after building: " << verts.size() << endl;

    Descriptor d = verts.find(3);
    cout << "value of vertex properties: " << verts.properties(d) << endl;

    test_remove(verts, typename descriptor_traits<StoreImpl>::descriptor_stability());
}

int main()
{
    test<vertex_vector<>>();
    test<vertex_list<>>();
    test<vertex_set<>>();
    test<vertex_map<int>>();

    return 0;
}
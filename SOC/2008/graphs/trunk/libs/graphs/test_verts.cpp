
#include <iostream>

#include <boost/graphs/vertex_vector.hpp>
#include <boost/graphs/vertex_list.hpp>
#include <boost/graphs/vertex_set.hpp>
#include <boost/graphs/vertex_map.hpp>

#include "typestr.hpp"
#include "vertices_traits.hpp"

using namespace std;
using namespace boost;

struct unlabled_vertex_tag { };
struct labeled_vertex_tag { };

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

template <typename Verts>
void populate(Verts& verts, simple_vertex_store_tag)
{
    for(int i = 0; i < 5; ++i) {
        verts.add(i);
    }
}
template <typename Verts>
void populate(Verts& verts, mapped_vertex_store_tag)
{
    for(int i = 0; i < 5; ++i) {
        verts.add(i, 2 * i);
    }
}

template <typename Verts>
void test_remove(Verts& verts, stable_mutators_tag)
{
    verts.remove(3);
    cout << "num verts after remove: " << verts.size() << endl;
}

template <typename Verts>
void test_remove(Verts& vs, unstable_remove_tag)
{ /* Noop for unstable removes */ }

template <typename Verts>
void test()
{
    typedef typename Verts::template store<Vertex>::type Store;
    typedef typename Verts::descriptor_type Descriptor;

    Store verts;
    cout << "--- " << typestr(verts) << " ---" << endl;

    populate(verts, vertices_category(verts));
    cout << "num verts after building: " << verts.size() << endl;

    Descriptor d = verts.find(3);
    cout << "value of vertex properties: " << verts.properties(d) << endl;

    test_remove(verts, vertices_category(verts));
}

int main()
{
    test<vertex_vector<>>();
    test<vertex_list<>>();
    test<vertex_set<>>();
    test<vertex_map<int>>();

    return 0;
}
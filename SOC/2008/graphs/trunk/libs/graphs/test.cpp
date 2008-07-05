
#include <iostream>
#include <string>
#include <list>

#include <boost/none.hpp>
#include <boost/graphs/vertex_vector.hpp>
#include <boost/graphs/vertex_list.hpp>

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
void test_remove(Store& verts, stable_descriptor_tag)
{
    verts.remove(3);
    cout << "size after remove: " << verts.size() << endl;
}

template <typename VertexSet>
void test_remove(VertexSet& vs, unstable_remove_tag)
{ /* Noop for unstable removes */ }

template <typename VertexSet>
void test()
{
    typedef typename VertexSet::template store<Vertex>::type Store;
    typedef typename VertexSet::descriptor_type Descriptor;

    cout << "--- " << demangle<VertexSet>() << " ---" << endl;

    Store verts;
    Descriptor d1 = verts.add(3);

    cout << "value of added props: " << verts.vertex(d1).value << endl;
    cout << "value of found props: " << verts.vertex(verts.find(3)).value << endl;

    test_remove(verts, typename descriptor_traits<typename Store::store_type>::descriptor_stability());
}

int main()
{
    test<vertex_vector<>>();
    test<vertex_list<>>();

    return 0;
}
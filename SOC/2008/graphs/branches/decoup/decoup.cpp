
#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <tr1/unordered_set>
#include <tr1/unordered_map>

#include <boost/graphs/placeholder.hpp>
#include <boost/graphs/descriptor.hpp>

#include "demangle.hpp"

using namespace std;
using namespace std::tr1;

template <typename Properties, typename IncidenceStore>
struct vertex
{
    vertex(Properties const& p)
        : props(p), edges()
    { }

    Properties      props;
    IncidenceStore  edges;
};

typedef double T;

// Generate the underlying storage types for a vertex (its slot). This will
// be used as the basic block of memory passed to the vertex store.
typedef vector<int> dummy_store;
typedef vertex<T, dummy_store> dummy_vertex;
typedef placeholder<sizeof(dummy_vertex)> vertex_slot;

// Create the vertex store and use that to generate descriptors.
typedef vector<vertex_slot> vertex_store;
typedef descriptor<vertex_store> vertex_descriptor;

// We can now use the vertex descriptor to generate the incidence store
typedef vector<vertex_descriptor> incidence_store;

// Now, we can generate the full vertex type.
typedef vertex<T, incidence_store> vertex_type;

int main()
{
    // Make sure that these are the correct sizes.
    BOOST_ASSERT(sizeof(vertex_type) == sizeof(vertex_slot));

    vertex_store verts;
    verts.push_back(vertex_type(3.14));

    vertex_type& v = verts.back().get<vertex_type>();
    cout << v.props << endl;

    return 0;
}

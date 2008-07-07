
#include <iostream>

#include <boost/graphs/incidence_vector.hpp>
#include <boost/graphs/incidence_list.hpp>
#include <boost/graphs/incidence_set.hpp>

#include "typestr.hpp"
#include "incidence_traits.hpp"

using namespace std;
using namespace boost;

typedef index_descriptor<size_t> VertexDesc;
typedef index_descriptor<size_t> PropDesc;
typedef std::pair<VertexDesc, PropDesc> Edge;
typedef allocator<Edge> Alloc;
typedef std::less<VertexDesc> Compare;

template <typename IncStore, typename Vertex, typename Property>
void add(IncStore& incs, Vertex v, Property p, associative_container_tag)
{
    typename IncStore::incidence_descriptor i = incs.add(v);
    incs.bind(i, p);
}

template <typename IncStore, typename Vertex, typename Property>
void add(IncStore& incs, Vertex v, Property p, sequence_tag)
{
    incs.add(v, p);
}


template <typename IncStore>
void test_remove(IncStore& incs, stable_mutators_tag)
{
    // Kind of strange, but we can actually construct some types of descriptors
    // withou an iterator.
    size_t n = incs.size();
    incs.remove(incs.find(VertexDesc(0)));
    BOOST_ASSERT(incs.size() == n - 1);
    cout << "  * num incs after removing " << incs.size() << endl;
}

template <typename IncStore>
void test_remove(IncStore& incs, unstable_remove_tag)
{ /* Can't remove elements. */ }

template <typename IncStore>
void test()
{
    IncStore incs;
    cout << "--- " << typestr(incidence_category(incs)) << " ---" << endl;

    // Add some edges
    incs.add(VertexDesc(0), PropDesc(0));
    incs.add(VertexDesc(1), PropDesc(1));
    cout << "  * num incs after building: " << incs.size() << endl;

    // Try to remove something
    test_remove(incs, incidence_category(incs));
}

int main()
{
    test<incidence_vector<Edge, Alloc>>();
    test<incidence_list<Edge, Alloc>>();
    test<incidence_set<Edge, Compare, Alloc>>();

    return 0;
}
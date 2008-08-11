
#include <iostream>

#include <boost/graphs/adjacency_list/in_vector.hpp>
#include <boost/graphs/adjacency_list/in_list.hpp>
#include <boost/graphs/adjacency_list/in_set.hpp>

#include "typestr.hpp"
#include "in_edge_traits.hpp"

using namespace std;
using namespace boost;

typedef index_descriptor<size_t> VertexDesc;
typedef index_descriptor<size_t> OutDesc;
typedef pair<VertexDesc, OutDesc> InEdge;
typedef allocator<InEdge> Alloc;
typedef less<VertexDesc> Compare;

template <typename Ins>
void test_remove(Ins& ins, stable_mutators_tag)
{
    size_t n = ins.size();
    ins.remove(ins.find(3));
    BOOST_ASSERT(ins.size() == n - 1);
    cout << "  * size after remove: " << ins.size() << endl;
}

template <typename Ins>
void test_remove(Ins&, unstable_remove_tag)
{ /* Don't do anything. */ }

template <typename Ins>
void test()
{
    Ins ins;
    cout << "--- " << typestr(in_edge_category(ins)) << " ---" <<  endl;

    // Add some vertices.
    BOOST_ASSERT(ins.empty());
    for(int i = 0; i < 5; ++i) {
        ins.add(VertexDesc(i), OutDesc(i + 1));
    }
    BOOST_ASSERT(ins.size() == 5);
    cout << "  * size after building: " << ins.size() << endl;

    test_remove(ins, in_edge_category(ins));
}

int main()
{
    test<in_vector<InEdge, Alloc>>();
    test<in_list<InEdge, Alloc>>();
    test<in_set<InEdge, Compare, Alloc>>();

    return 0;
}

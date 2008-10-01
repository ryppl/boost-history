
#include <iostream>

#include <boost/graphs/adjacency_list/out_vector.hpp>
#include <boost/graphs/adjacency_list/out_list.hpp>
#include <boost/graphs/adjacency_list/out_set.hpp>

#include "typestr.hpp"
#include "out_edge_traits.hpp"

using namespace std;
using namespace boost;


typedef index_descriptor<size_t, basic_descriptor_kind> VertexDesc;
typedef index_descriptor<size_t, basic_descriptor_kind> InDesc;
typedef int EdgeProps;
typedef pair<VertexDesc, pair<EdgeProps, InDesc>> OutEdge;
typedef allocator<OutEdge> Alloc;
typedef less<VertexDesc> Compare;

template <typename Outs>
void test_remove(Outs& outs, stable_mutators_tag)
{
    size_t n = outs.size();
    outs.remove(outs.find(3));
    BOOST_ASSERT(outs.size() == n - 1);
    cout << "  * size after remove: " << outs.size() << endl;
}

template <typename Outs>
void test_remove(Outs& outs, unstable_remove_tag)
{ /* Don't do anything. */ }

template <typename Outs>
void test()
{
    Outs outs;
    cout << "--- " << typestr(out_edge_category(outs)) << " ---" <<  endl;

    // Add some vertices.
    BOOST_ASSERT(outs.empty());
    for(int i = 0; i < 5; ++i) {
        outs.add(VertexDesc(i), i * i);
    }
    BOOST_ASSERT(outs.size() == 5);
    cout << "  * size after building: " << outs.size() << endl;

    test_remove(outs, out_edge_category(outs));
}

int main()
{
    test<out_vector<OutEdge, Alloc>>();
    test<out_list<OutEdge, Alloc>>();
    test<out_set<OutEdge, Compare, Alloc>>();

    return 0;
}

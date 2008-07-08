
#include <iostream>

#include <boost/graphs/out_vector.hpp>

#include "typestr.hpp"

using namespace std;
using namespace boost;

struct out_vector_tag : sequence_tag, unstable_remove_tag { };
struct out_list_tag : sequence_tag, stable_mutators_tag { };
struct out_set_tag : associative_container_tag, stable_mutators_tag { };

template <typename Outs>
struct out_edge_traits
{ typedef typename Outs::category cateogry; };

template <typename Outs>
typename out_edge_traits<Outs>::category
out_edge_category(Outs const&)
{ return typename out_edge_traits<Outs>::category(); }

template <typename Edge, typename Alloc>
struct out_edge_traits<out_vector<Edge, Alloc>>
{ typedef out_vector_tag category; };


typedef index_descriptor<size_t> VertexDesc;
typedef index_descriptor<size_t> InDesc;
typedef int EdgeProps;
typedef triple<VertexDesc, EdgeProps, InDesc> OutEdge;
typedef allocator<OutEdge> Alloc;

template <typename Outs>
void test()
{
    Outs outs;
    cout << "--- " << typestr(out_edge_category(outs)) << " ---" <<  endl;

    BOOST_ASSERT(outs.empty());
    for(int i = 0; i < 5; ++i) {
        outs.add(VertexDesc(i), i * i);
    }
    BOOST_ASSERT(outs.size() == 5);
}

int main()
{
    test<out_vector<OutEdge, Alloc>>();
    return 0;
}

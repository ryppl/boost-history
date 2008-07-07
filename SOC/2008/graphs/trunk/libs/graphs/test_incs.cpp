
#include <iostream>

#include <boost/graphs/incidence_vector.hpp>
#include <boost/graphs/incidence_list.hpp>
#include <boost/graphs/incidence_set.hpp>

#include "typestr.hpp"

using namespace std;
using namespace boost;

struct incidence_vector_tag : unstable_remove_tag { };
struct incidence_list_tag : stable_descriptor_tag { };
struct incidence_set_tag : stable_descriptor_tag { };

template <typename IncStore>
struct incidence_traits
{ typedef typename IncStore::category category; };

template <typename IncStore>
typename incidence_traits<IncStore>::category
incidence_category(IncStore const&)
{ return typename incidence_traits<IncStore>::category(); }

template <typename Edge, typename Alloc>
struct incidence_traits<incidence_vector<Edge, Alloc>>
{ typedef incidence_vector_tag category; };

template <typename Edge, typename Alloc>
struct incidence_traits<incidence_list<Edge, Alloc>>
{ typedef incidence_list_tag category; };

template <typename Edge, typename Comp, typename Alloc>
struct incidence_traits<incidence_set<Edge, Comp, Alloc>>
{ typedef incidence_set_tag category; };



typedef index_descriptor<size_t> VertexDesc;
typedef index_descriptor<size_t> PropDesc;
typedef std::pair<VertexDesc, PropDesc> Edge;
typedef allocator<Edge> Alloc;
typedef std::less<VertexDesc> Compare;



template <typename IncStore>
void test_remove(IncStore& incs, stable_descriptor_tag)
{
    // Kind of strange, but we can actually construct some types of descriptors
    // withou an iterator.
    incs.remove(incs.find(VertexDesc(0)));
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
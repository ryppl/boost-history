
#include <iostream>

#include <boost/graphs/property_vector.hpp>
#include <boost/graphs/property_list.hpp>

#include "typestr.hpp"
#include "properties_traits.hpp"

using namespace std;
using namespace boost;


typedef int EdgeProperties;
typedef descriptor_traits<std::list<int>>::descriptor_type IncDesc;

template <typename PropSet>
void test_remove(PropSet& props, stable_mutators_tag)
{
    size_t n = props.size();
    props.remove(props.find(5));
    BOOST_ASSERT(props.size() == n - 1);
    std::cout << "num props after remove: " << props.size() << endl;
}

template <typename PropSet>
void test_remove(PropSet& props, unstable_remove_tag)
{ /* Can't remove! */ }

template <typename PropSet>
void test()
{
    typedef typename PropSet::property_descriptor PropDesc;

    cout << "--- " << typestr<PropSet>() << " ---" << endl;

    // Add some properties
    size_t const N = 10;
    PropSet props;
    for(size_t i = 0; i < N; ++i) {
        props.add(i);
    }
    BOOST_ASSERT(props.size() == N);
    cout << "num props after build: " << props.size() << endl;

    test_remove(props, properties_category(props));
}

int main()
{
    typedef pair<IncDesc, IncDesc> EdgePair;
    typedef pair<EdgeProperties, EdgePair> StoredEdge;
    typedef allocator<StoredEdge> Allocator;

    test<property_vector<StoredEdge, Allocator>>();
    test<property_list<StoredEdge, Allocator>>();

    return 0;
}
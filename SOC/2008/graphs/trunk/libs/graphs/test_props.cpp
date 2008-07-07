
#include <iostream>

#include <boost/graphs/property_vector.hpp>
#include <boost/graphs/property_list.hpp>

#include "typestr.hpp"

using namespace std;
using namespace boost;

typedef int EdgeProperties;
typedef descriptor_traits<std::list<int>>::descriptor_type IncDesc;

template <typename PropSet>
void test_remove(PropSet& props, stable_descriptor_tag)
{
    props.remove(props.find(5));
    std::cout << "num props after remove: " << props.size() << endl;
}

template <typename PropSet>
void test_remove(PropSet& props, unstable_remove_tag)
{ /* Can't remove! */ }

template <typename PropSet>
void test()
{
    typedef typename PropSet::property_descriptor PropDesc;
    typedef typename descriptor_traits<typename PropSet::store_type>::descriptor_stability Stability;

    cout << "--- " << typestr<PropSet>() << " ---" << endl;

    // Add some properties
    PropSet props;
    for(int i = 0; i < 10; ++i) {
        props.add(i);
    }
    cout << "num props after build: " << props.size() << endl;

    test_remove(props, Stability());
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
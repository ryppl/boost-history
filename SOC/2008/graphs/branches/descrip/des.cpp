
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>

#include "descriptor.hpp"
#include "demangle.hpp"

using namespace std;

namespace dispatch
{
    // A little help for mapped values.
    template <typename Container, typename Value>
    void insert(Container& c, Value const& x, pair_associative_container_tag)
    { c.insert(make_pair(x, x)); }
}

template <typename T, typename U>
ostream& operator<<(ostream& os, pair<T, U> const& x)
{ return os << "(" << x.first << "," << x.second << ")"; }

template <typename Container>
void test()
{
    typedef typename container_traits<Container>::category Category;
    typedef typename container_traits<Container>::iterator_stability IteratorStability;
    typedef typename extended_container_traits<Container>::descriptor_type Descriptor;
    typedef typename extended_container_traits<Container>::descriptor_stability DescriptorStability;

    cout << "--- " << demangle<Container>() << " ---" << endl;
    cout << "category: " << demangle<Category>() << endl;
    cout << "iterator stability: " << demangle<IteratorStability>() << endl;
    cout << "descriptor stabiltiy: " << demangle<DescriptorStability>() << endl;

    cout << "allows insert: " << has_insert_mutator<Container>::value << endl;
    cout << "allows remove: " << has_remove_mutator<Container>::value << endl;
    cout << "unique elements: " << contains_unique_elements<Container>::value << endl;
    cout << "mapped elements: " << contains_mapped_elements<Container>::value << endl;

    Container c;
    insert(c, 0.0);
    insert(c, 6.28);
    insert(c, 3.14);

    Descriptor d1 = make_descriptor(c, c.begin());
    Descriptor d2 = make_descriptor(c, next(c.begin(), 1));
    Descriptor d3 = make_descriptor(c, next(c.begin(), 2));

    cout << *make_iterator(c, d1) << endl;
    cout << *make_iterator(c, d2) << endl;
    cout << *make_iterator(c, d3) << endl;
}

int
main()
{
    test<vector<double>>();
    test<list<double>>();
    test<set<double>>();
    test<map<double, double>>();
    test<multiset<double>>();
    test<multimap<double, double>>();

    return 0;
}


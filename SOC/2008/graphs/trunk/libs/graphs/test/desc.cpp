
#include <iostream>

#include <boost/graphs/descriptor.hpp>

#include "demangle.hpp"

using namespace std;

template <typename C>
void test(C& c)
{
    typedef descriptor<C> D;
    D a(c, c.begin());
    D b(c, ++c.begin());
    
    cout << "--- " << demangle<C>() << " ---" << endl;
    cout << "id a: " << hash_value(a) << endl;
    cout << "id b: " << hash_value(b) << endl;
    cout << (a != a) << std::endl;
    cout << (a < b) << std::endl;
}

int main()
{
    std::vector<int> v;
    v.push_back(10);
    v.push_back(11);
    test(v);
    
    std::list<int> l;
    l.push_back(10);
    l.push_back(11);
    test(l);
    
    std::set<int> s;
    s.insert(10);
    s.insert(11);
    test(s);
    
    std::map<int, int> m;
    m[10] = 20;
    m[11] = 21;
    test(m);
    
    return 0;
}

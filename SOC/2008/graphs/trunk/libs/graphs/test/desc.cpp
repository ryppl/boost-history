
#include <iostream>
#include <vector>
#include <list>
#include <set>
#include <map>

#include <boost/descriptors.hpp>

#include "typestr.hpp"

using namespace std;

struct my_kind { };

struct foo
{
    typedef int* foo::*p;

    void test()
    {
        cout << typestr(&foo::x) << endl;
    }

    int x;
};

template <typename C>
void test(C& c)
{
    typedef descriptor_traits<C, my_kind> Traits;
    typedef typename Traits::descriptor_type D;

    typedef typename descriptor_traits<vector<int>>::descriptor_type X;

    cout << typestr(c) << endl;
    // cout << typestr<typename D::descriptor_kind>() << " " << typestr<typename X::descriptor_kind>() << endl;

    D a(c, c.begin());
    D b(c, ++c.begin());
    X x;
    // cout << typestr(a) << " " << typestr(x) << endl;
    // cout << "comp: " << (a == a) << endl;
    // cout << "should not compile: " << (x == a) << endl;

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

    foo f;
    f.test();

    return 0;
}


#include <iostream>

#include <boost/assert.hpp>
#include <boost/optional.hpp>
#include <boost/optional_value.hpp>

#include "typestr.hpp"

using namespace std;
using namespace boost;

void test_opt()
{
    optional<int> x, y = 5;
    BOOST_ASSERT(!x.valid());
    BOOST_ASSERT(y.valid());
    x = y;
    BOOST_ASSERT(*x == 5);
    BOOST_ASSERT(*y == 5);

    int i = 20;
    optional<int*> p, q = &i;
    BOOST_ASSERT(!p.valid());
    BOOST_ASSERT(q.valid() && *q == 20);
}

void test_opt_val()
{
    optional_value<unsigned> x, y = 5;
    BOOST_ASSERT(!x.valid());
    BOOST_ASSERT(y.valid());
    x = y;
    BOOST_ASSERT(x && *x == 5);

    cout << typestr(optional_value<int*>::absent()) << " -> " << optional_value<int*>::absent() << endl;
    cout << typestr(optional_value<int>::absent()) << " -> " << optional_value<int>::absent() << endl;
    cout << typestr(optional_value<unsigned>::absent()) << " -> " << optional_value<unsigned>::absent() << endl;
    cout << typestr(optional_value<double>::absent()) << " -> " << optional_value<double>::absent() << endl;
    cout << typestr(optional_value<double, absent_neg_inf<double>>::absent())
         << " -> " << optional_value<double, absent_neg_inf<double>>::absent() << endl;
}

int main()
{
    test_opt();
    test_opt_val();
}
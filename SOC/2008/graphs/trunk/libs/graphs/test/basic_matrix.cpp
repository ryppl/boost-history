
#include <iostream>

#include <boost/graphs/adjacency_matrix/basic_matrix.hpp>

#include "typestr.hpp"

using namespace std;
using namespace boost;
using namespace boost::graphs::adjacency_matrix;

int main()
{
    basic_matrix<none> a(10);
    a(1, 1) = true;
    cout << "(1, 1) -> " << (a.test(1, 1) ? "on" : "off") << endl;
    cout << "(2, 2) -> " << (a.test(2, 2) ? "on" : "off") << endl;
    cout << endl;

    basic_matrix<double> b(10);
    b(1, 1) = 3.14;
    cout << "(1, 1) -> " << (b.test(1, 1) ? "on" : "off") << endl;
    cout << "(2, 2) -> " << (b.test(2, 2) ? "on" : "off") << endl;
    cout << endl;

    typedef basic_matrix<optional_value<int>> C;
    C c(10);
    c(1, 1) = 12;
    cout << "(1, 1) -> " << (c.test(1, 1) ? "on" : "off") << endl;
    cout << "(2, 2) -> " << (c.test(2, 2) ? "on" : "off") << endl;
}


#include <iostream>

#include <boost/graphs/utility.hpp>

using namespace std;
using namespace boost::graphs;

int
main(int argc, char *argv[])
{
    ordered_pair<int> a(0, 3);
    ordered_pair<int> b = make_ordered_pair(1, 4);

    cout << a.first() << " " << a.second() << endl;
    cout << b.first() << " " << b.second() << endl;
}

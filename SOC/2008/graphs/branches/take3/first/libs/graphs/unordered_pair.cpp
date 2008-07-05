
#include <iostream>

#include <boost/graphs/utility.hpp>

using namespace std;
using namespace boost::graphs;

int
main(int argc, char *argv[])
{
    unordered_pair<int> a(3, 0);
    unordered_pair<int> b = make_unordered_pair(4, 1);

    cout << a.first() << " " << a.second() << endl;
    cout << b.first() << " " << b.second() << endl;
}

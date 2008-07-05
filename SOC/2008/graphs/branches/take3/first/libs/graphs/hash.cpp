
#include <iostream>
#include <vector>
#include <tr1/unordered_map>
#include <cstdlib>
#include <ctime>

#include <boost/date_time.hpp>

using namespace std;
using namespace std::tr1;
using namespace boost::posix_time;

int
main(int argc, char *argv[])
{
    srand(time(0));

    // Create a vector of 1000 elements.
    vector<int> v(1000);
    for(size_t i = 0; i < 1000; ++i) {
        v[i] = rand() % 100;
    }

    // Create a map of 1000 elements
    unordered_map<int*, int> m;
    for(size_t i = 0; i < 1000; ++i) {
        m[&v[i]] = rand() % 100;
    }

    ptime start, stop;

    // Time accesses
    start = microsec_clock::local_time();
    for(size_t i = 0; i < 1000; ++i) {
        int x = v[rand() % 1000];
    }
    stop = microsec_clock::local_time();
    cout << "Vector: " << stop - start << endl;

    // Time accesses
    start = microsec_clock::local_time();
    for(size_t i = 0; i < 1000; ++i) {
        cout << m[&v[rand() % 1000]] << " ";
    }
    stop = microsec_clock::local_time();
    cout << "Hash: " << stop - start << endl;

    return 0;
}

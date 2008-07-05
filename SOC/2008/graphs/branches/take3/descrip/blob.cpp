
#include <iostream>

#include "blob.hpp"

using namespace std;

int
main()
{
    blob<sizeof(int)> x, y;
    x.put(1);
    y.put(2);

    cout << "x < y == " << (x < y) << endl;
    cout << "x > y == " << (x > y) << endl;
    cout << "x <= y == " << (x <= y) << endl;
    cout << "x >= y == " << (x >= y) << endl;

    return 0;
}


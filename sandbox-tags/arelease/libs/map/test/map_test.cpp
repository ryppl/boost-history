//---------------------------------------------------------------------------
#pragma hdrstop
#include "map.hpp"
#include <functional>
#include <iostream>
//---------------------------------------------------------------------------
struct D
{
    int     mID;
    double  mVal;
};

struct KeyOfValue : std::unary_function<D, int>
{
    int     operator()(D const& d) const { return d.mID; }
};

int main(int argc, char* argv[])
{
    typedef boost::map<int, D, boost::UniqueKey, boost::StdKeyOfValue<int, D>, boost::IndexedNode> myMap;
    myMap map;
    D d = {2, 5.5};

    std::cout << (map.valid() ? "Valid map" : "Invalid map!!!") << std::endl;
    map[2] = d; // 5.5;
    myMap::iterator i = map.find(2);
    if (i == map.end()) {
        std::cout << "Item not found!" << std::endl;
    } else {
        std::cout << "mVal: " << i->second.mVal << std::endl;
    }

    i = map.at(1);
    if (i == map.end()) {
        std::cout << "Item not found!" << std::endl;
    } else {
        std::cout << "mVal: " << i->second.mVal << std::endl;
    }

    try {
        int n = map.pos(5);
        std::cout << "Item at: " << n << std::endl;
    } catch (std::exception& e) {
        std::cout << "Item not found!" << std::endl;
    }

    std::cin >> d.mID;

    return 0;
}
//---------------------------------------------------------------------------


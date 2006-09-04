#include <boost/property_tree/ptree.hpp>
#include <iostream>

int main()
{
    using namespace boost::property_tree;

    path p1, p2;
    "a.b.c" / p2;
    p1 / "a.b.c";
    
}

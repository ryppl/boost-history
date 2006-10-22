#include <boost/property_tree/info_parser.hpp>
#include <iostream>
#include <sstream>

using namespace boost::property_tree;

int main()
{
    
    ptree pt;

    unsigned char a = -7;
    signed char b = -8;
    char c = 9;
    bool d = true;

    pt.put("a", a);
    pt.put("b", b);
    pt.put("c", c);
    pt.put("d", d);
    write_info(std::cout, pt);

    b = pt.get<signed char>("b");
    a = pt.get<unsigned char>("a");
    c = pt.get<char>("c");
    d = pt.get<bool>("d");

    std::stringstream stream;
    write_info(stream, pt);
    ptree pt2;
    read_info(stream, pt2);

}

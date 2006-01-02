#include <boost/property_tree/ptree.hpp>
#include <iostream>

int main()
{
    try
    {
        boost::property_tree::ptree pt;
        pt.put("a.b.c", "a");
        pt.get<int>("a.b.c");
    }
    catch (std::runtime_error &e)
    {
        std::cerr << e.what() << "\n";
    }
}
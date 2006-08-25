#include <boost/property_tree/ptree.hpp>
#include <iostream>

int main()
{
    using namespace boost::property_tree;
    
    ptree pt;
    
    pt.put_value(3);
    int a = pt.get_value<int>();

    ptree &child1 = pt.push_back(ptree::value_type("key1", empty_ptree<ptree>()))->second;
    ptree &child2 = child1.push_back(ptree::value_type("key2", empty_ptree<ptree>()))->second;

    ptree &child = pt.get_child("key1.key2");

    path p("key1/key2", '/');
    ptree &child3 = pt.get_child(p);


}

#include <boost/optional.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

int main()
{
    boost::property_tree::ptree pt;
    pt.put("path", 7);
    boost::optional<int> q = pt.get_o<int>("path");
    boost::optional<int> w = pt.get_o<int>("path2");
}
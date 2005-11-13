#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

int main()
{
    using namespace boost::property_tree;
    ptree pt;
    pt.put("kaalus", 7);
    write_info("out.info", pt);
}
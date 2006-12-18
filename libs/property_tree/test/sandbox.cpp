#define _CRT_SECURE_NO_DEPRECATE
#define BOOST_PROPERTY_TREE_XML_PARSER_PUGXML
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/info_parser.hpp>

int main()
{
    using namespace boost::property_tree;
    ptree pt;
    read_xml("pugxml.xml", pt);
    write_info("gogo.txt", pt);
}

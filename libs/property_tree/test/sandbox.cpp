#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

int main()
{
    using namespace boost::property_tree;
    ptree pt;
    char *c = setlocale(LC_ALL, "polish");
    pt.put("number", 1.234);
    pt.put("number2", 1.234, std::locale("french"));
    c = setlocale(LC_ALL, "english");
    std::string number = pt.get<std::string>("number");
    std::string number2 = pt.get<std::string>("number2");
    c = setlocale(LC_ALL, "");
}
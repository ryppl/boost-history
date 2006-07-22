#include <boost/property_tree/ini_parser.hpp>
#include <iostream>

void f()
{
	boost::property_tree::ptree pt;
	read_ini("test.ini", pt);
	boost::property_tree::ptree::const_iterator ci = pt.find("User Data.value2");
	if (ci != pt.end()) 
	{
		std::cout << ci->second.data() << std::endl;
	}
}

int main()
{
	f();
}

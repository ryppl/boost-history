

//#include "virtual_constructors.hpp"

#include <string>
#include <iostream>
#include <map>
using namespace std;

#include "weapon.hpp"

#include <boost/any.hpp>

#include <boost/plugin/plugin_factory.hpp>
#include <boost/plugin/dll.hpp>


using namespace boost::plugin;

//typedef std::map<std::string, boost::any> exported_plugins_t;
//typedef exported_plugins_t& (*get_factory_t)();
//typedef exported_plugins_t& (get_factory_np)();


int main()
{
    boost::plugin::dll d("library.so");

    boost::plugin::plugin_factory<Weapon> pf(d);
    cout << "*** Creating an instance of plugin class\n";
    Weapon* w = pf.create("Missile", "biz", 13);
    cout << "*** Calling method of the created instance\n";
    w->fire();
    delete w;
    for(;;) {}

}


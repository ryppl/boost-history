

#include <string>
#include <iostream>
#include <map>

#include <boost/plugin/plugin_factory.hpp>
#include <boost/plugin/dll.hpp>

#include "weapon.hpp"

using namespace std;

int main()
{
    boost::plugin::dll d ("library.dll");
    boost::plugin::plugin_factory<Weapon> pf(d);
    
    cout << "*** Creating an instance of plugin class\n";
    Weapon* w1 = pf.create("Missile", "biz", 13);

    cout << "*** Creating an instance of plugin class\n";
    Weapon* w2 = pf.create("Missile", "wush");

    cout << "*** Creating an instance of plugin class\n";
    Weapon* w3 = pf.create("Missile", "wish", 10, 20);

    cout << "*** Calling method of the created instance\n";
    w1->fire();
    w2->fire();
    w3->fire();

    delete w1;
    delete w2;
    delete w3;
}


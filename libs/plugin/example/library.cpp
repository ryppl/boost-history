
#include <boost/plugin/concrete_factory.hpp>
using namespace boost::plugin;

#include <boost/any.hpp>
#include <map>
#include <utility>

#include "weapon.hpp"

#include <boost/plugin/export_plugin.hpp>

class Missile : public Weapon {
public:
    Missile(const std::string& s, int i = 10) {
        std::cout << "Created missile '" << s << "'\n";
        std::cout << "Fuel load is " << i << "\n";
        
    }
    virtual void fire() { std::cout << "Fire!\n"; }
};

BOOST_EXPORT_PLUGIN(Weapon, Missile, "Missile");
BOOST_EXPORT_PLUGIN_LIST();


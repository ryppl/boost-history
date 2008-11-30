/* example to demonstrate compile-in-place for Boost.Program_options

Compile with
  $CXX -I path/to/boost_X_YY_Z test-program-options.cpp

Notes:

$Id$
*/

#define BOOST_PROGRAM_OPTIONS_COMPILE_IN_PLACE
#include <iostream>
#include <ostream>
#include <boost/program_options.hpp>
#include <boost/program_options/compile_in_place.cpp>

namespace po = boost::program_options;


int main(int argc, char** argv)
{
    // Declare the supported options.
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "show usage info")
        ("name", po::value<std::string>(), "set the one to greet")
    ;

    po::variables_map vars;
    po::store(po::parse_command_line(argc, argv, desc), vars);
    po::notify(vars);

    if (vars.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    std::string name = "world";
    if(vars.count("name"))
        name = vars["name"].as<std::string>();

    std::cout << "hello " << name << "!" << std::endl;

    return 0;
}

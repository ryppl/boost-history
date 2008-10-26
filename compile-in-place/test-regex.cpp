/* example to demonstrate compile-in-place for Boost.Regex

Compile with
  $CXX -I path/to/boost_X_YY_Z test-regex.cpp

Notes:

$Id$
*/
#include <boost/regex.hpp>
#include <boost/regex/compile_in_place.cpp>
#include <iostream>
#include <ostream>
#ifndef BOOST_CPP_REGEX_TRAITS_HPP_INCLUDED
# error
#endif
int main( int argc, char** argv)
{
    if(argc!=3)
    {
        std::cerr << argv[0] << " <regex> <string>" << std::endl;
        return 1;
    }

    boost::regex e( argv[1]);

    std::cout << "regex_match(" << argv[2] << ", " << e << ") = " << regex_match( argv[2], e) << std::endl;

    return 0;
}

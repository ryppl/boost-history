/* example to demonstrate compile-in-place for Boost.System

Compile with
  $CXX -I path/to/boost_X_YY_Z test-system.cpp

Notes:

$Id$
*/

#include <iostream>
#include <ostream>
#include <boost/system/system_error.hpp>

int main()
{
    try
    {
        std::cout << "throwing ENOENT" << std::endl;
        throw boost::system::system_error(ENOENT, boost::system::get_posix_category());
    }
    catch(std::exception const& e)
    {
        std::cout << "caught '" << e.what() << '\'' << std::endl;
    }
}

#include <boost/system/compile_in_place.cpp>

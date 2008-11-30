/* example to demonstrate compile-in-place for the filesytem lib

Compile with
  $CXX -I path/to/boost_X_YY_Z test-filesystem.cpp

Notes:

- This needs to be changed to support 1.34, where the filesystem lib didn't
depend on a separate system lib.

$Id$
*/

#define BOOST_FILESYSTEM_COMPILE_IN_PLACE
#include <boost/filesystem.hpp>
#include <iostream>
#include <ostream>
#include <boost/filesystem/compile_in_place.cpp>
// Note: Boost.Filesystem since 1.35 depends on Boost.System
#include <boost/system/compile_in_place.cpp>

using namespace boost::filesystem;

int
main( int argc, char* argv[])
{
    if(argc!=2)
    {
        std::cerr << "need exactly one argument for the dir to list" << std::endl;
        return EXIT_FAILURE;
    }
    path dir_path = argv[1];
    if ( !exists( dir_path ) )
        return EXIT_FAILURE;

    for ( directory_iterator it( dir_path ), end;
        it != end;
        ++it )
    {
        if ( is_directory(it->status()) )
        {
            std::cout << "dir: " << *it << std::endl;
        }
        else
        {
            std::cout << "file: " << *it << std::endl;
        }
    }
}

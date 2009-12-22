//  filesystem tut4.cpp  ---------------------------------------------------------------//

//  Copyright Beman Dawes 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

#include <iostream>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: tut4 path\n";
    return 1;
  }

  std::cout << argv[1] << ": ";
  if ( fs::exists(argv[1]) )
  {
    if ( fs::is_regular_file(argv[1]) )
      std::cout << fs::file_size(argv[1]) << '\n';
    else if ( fs::is_directory(argv[1]) )
    {
      std::cout << "is a directory containing:\n";
      for ( fs::directory_iterator it (argv[1]); it != fs::directory_iterator(); ++it )
      {
        std::cout << "   " << it->path() << '\n';
      }
    }
    else
      std::cout << "exists, but is neither a regular file nor a directory\n";
  }
  else
    std::cout << "does not exist\n";

  return 0;
}

//  filesystem tut3.cpp  ---------------------------------------------------------------//

//  Copyright Beman Dawes 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

#include <iostream>
#include <iterator>
#include <algorithm>
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cout << "Usage: tut3 path\n";
    return 1;
  }

  path p (argv[1]);   // p reads clearer than argv[1] in the following code

  cout << p << ": ";  // utilize the path narrow stream inserter

  if (exists(p))    // does p actually exist?
  {
    if (is_regular_file(p))        // is p a regular file?
      cout << file_size(p) << '\n';

    else if (is_directory(p))      // is p a directory?
    {
      cout << "is a directory containing:\n";

      copy(directory_iterator(p), directory_iterator(),  // directory_iterator::value_type
        ostream_iterator<directory_entry>(cout, "\n"));  // is directory_entry, which is
                                                         // converted to a path by the
                                                         // path stream inserter
    }
    else
      cout << "exists, but is neither a regular file nor a directory\n";
  }
  else
    cout << "does not exist\n";

  return 0;
}

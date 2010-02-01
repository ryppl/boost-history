//  filesystem tut4.cpp  ---------------------------------------------------------------//

//  Copyright Beman Dawes 2009

//  Distributed under the Boost Software License, Version 1.0.
//  See http://www.boost.org/LICENSE_1_0.txt

//  Library home page: http://www.boost.org/libs/filesystem

#include <iostream>
#include <vector>
#include <algorithm>
#include <boost/filesystem.hpp>
using namespace std;
using namespace boost::filesystem;

int main(int argc, char* argv[])
{
  if (argc < 2)
  {
    cout << "Usage: tut4 path\n";
    return 1;
  }

  path p (argv[1]);   // p reads clearer than argv[1] in the following code

  cout << p << ": ";  // utilize the path narrow stream inserter

  if ( exists(p) )    // does p actually exist?
  {
    if ( is_regular_file(p) )        // is p a regular file?
      cout << file_size(p) << '\n';

    else if ( is_directory(p) )      // is p a directory?
    {
      cout << "is a directory containing:\n";

      typedef vector<path> vec;             // store paths,
      vec v;                                // so we can sort them later

      for ( directory_iterator it (p);      // initialize it to the first element
            it != directory_iterator();     // test for the past-the-end element
            ++it )                          // increment
      {
        path fn = it->path().filename();    // extract the filename from the path
        v.push_back(fn);                    // push into vector for later sorting
      }

      sort(v.begin(), v.end());             // sort, since directory iteration
                                            // is not ordered on some file systems

      for (vec::const_iterator it (v.begin()); it != v.end(); ++it)
      {
        cout << "   " << *it << '\n';
      }
    }
    else
      cout << "exists, but is neither a regular file nor a directory\n";
  }
  else
    cout << "does not exist\n";

  return 0;
}

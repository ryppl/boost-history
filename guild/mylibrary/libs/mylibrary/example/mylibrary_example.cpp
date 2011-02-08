/*! \file
\brief
 An example to run the simplest "Hello world" program,
 and to use code in an included header file.
 \version 1

 \details This example is used to illustrate Quickbook markup,
Doxygen C++ code indexing, and use of AutoIndex to provide index links
to items in this file.

Demonstrate indexing of this example by including the path
to this file in the scan-path in script file mylibrary.idx thus:

# All example source files, assuming no sub-folders.
!scan-path "libs/mylibrary/example" ".*\.cpp"

Some terms in this file should be indexed by the chosen index terms,
but others should be excluded,
for example degrees "kelvin" and
even the name of the great man himself "Lord Kelvin".

 \warning This example is entirely contrived to show off features, and does nothing useful.

 \author Paul A. Bristow
 \version 3
 \date Feb 2011
*/

// Copyright Paul A. Bristow 2011

// Use, modification and distribution are subject to the
// Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt
// or copy at http://www.boost.org/LICENSE_1_0.txt)

// This file is written to be included from a Quickbook .qbk document.
// It can be compiled by the C++ compiler, and run. Any output can
// also be added here as comment or included or pasted in elsewhere.
// Caution: this file contains Quickbook markup as well as code
// and comments: don't change any of the special comment markups!

// This file also includes Doxygen-style documentation about the function of the code.
// See http://www.doxygen.org for details.

//[mylibrary_example_1
// This is a snippet of code that can be included into a Quickbook program.

// Include Standard Library input output, for example.
#include <iostream>
using std::cout;
using std::endl;

// Include mylibrary header(s).
#include <boost/mylibrary/mylibrary.hpp>

// Probably need to add #include directories to the project as well.

/*!  A very simple example program, doing very little except some trivial output.

     \detail  273 o kelvin should NOT be indexed.
     \pre No preconditions (apart from assuming that this is the main function)
     \post No side effects, just a zero return.

     \returns Zero always, even if an error is detected.
     \remark This is a Doxygen remark about an obscure detail that should get indexed under "obscure".

*/

int main()
{
  using boost::mylibrary::myclass;

  cout << "Hello World!" << endl;
  return 0;

} // int main()

//] [mylibrary_example_1]


/*

//[mylibrary_example_output

Output from running my_library_example.cpp is:

------ Rebuild All started: Project: mylibrary_example, Configuration: Debug Win32 ------
  mylibrary_example.cpp
  mylibrary_example.vcxproj -> J:\Cpp\mylibrary_example\Debug\mylibrary_example.exe
  Hello World!
========== Rebuild All: 1 succeeded, 0 failed, 0 skipped ==========


//] [mylibrary_example_output]

*/


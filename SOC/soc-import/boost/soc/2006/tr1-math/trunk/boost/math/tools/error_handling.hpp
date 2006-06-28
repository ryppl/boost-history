//  (C) Copyright Xiaogang Zhang 2006.
//  Use, modification and distribution are subject to the
//  Boost Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_MATH_TOOLS_ERROR_HANDLING_HPP
#define BOOST_MATH_TOOLS_ERROR_HANDLING_HPP

#include <string>

namespace boost { namespace math { namespace tools {

using namespace std;

int error_exit(string function, string file, string message)
{
    cout << "Error at " << function << " in " << file
         << ": " << message << endl << "Exiting..." << endl;
    exit(-1);
}

}}} // namespaces

#endif // BOOST_MATH_TOOLS_ERROR_HANDLING_HPP

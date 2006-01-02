//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_PRINTCONTAINER_HPP
#define BOOST_PRINTCONTAINER_HPP

#include <functional>
#include <iostream>
#include <algorithm>

struct PrintValues : public std::unary_function<int, void>
{
   void operator() (int value) const
   {
      std::cout << value << " ";
   }
};

template<class Container>
void PrintContents(const Container &cont, const char *contName)
{
   std::cout<< "Printing contents of " << contName << std::endl;
   std::for_each(cont.begin(), cont.end(), PrintValues());
   std::cout<< std::endl << std::endl;
}

#endif //#ifndef BOOST_PRINTCONTAINER_HPP

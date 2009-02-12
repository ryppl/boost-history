//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef __BOOST_SYNCHRO_DETAIL_NON_HEAP_ALLOCATED__HPP
#define __BOOST_SYNCHRO_DETAIL_NON_HEAP_ALLOCATED__HPP
/**
* avoids allocation on the heap by using a technique:
* disable operator new and operator delete.
*/
#include <memory>
#define BOOST_SYNCHRO_NON_HEAP_ALLOCATED(T)         \
private:                                    \
  void* operator new(unsigned);             \
  void* operator new[](unsigned);           \
  void operator delete(void*, unsigned);    \
  void operator delete[](void*, unsigned);  \

#endif

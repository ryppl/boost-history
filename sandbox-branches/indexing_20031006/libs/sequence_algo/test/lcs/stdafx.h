// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#include <memory.h>
#include <iostream>
#include <fstream>
#include <time.h>
#include <list>
#include <vector>
#include <deque>
#include <string>                // typeid() requires string
#include <typeinfo>              // typeid
#include <sstream>               // std::stringstream
#include <iomanip>               // std::setw

#include "boost/config.hpp"

// optional header file for debug library
#if 1 && defined(BOOST_MSVC)
#   include <windows.h>
#   pragma message("Including debug library")
#   include "DbgLib.h"
#   undef BOOST_ASSERT
#   define BOOST_ASSERT ASSERT
#else
#   define BOOST_DEBUG 1
#   include "boost/assert.hpp"      // BOOST_ASSERT
#endif

#undef max
#undef min

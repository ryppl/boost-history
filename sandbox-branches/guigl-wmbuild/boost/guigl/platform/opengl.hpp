/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#ifdef __APPLE__

#include <OpenGL/gl.h>

#else

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include "windows.h"
#endif

#include <GL/gl.h>

#endif


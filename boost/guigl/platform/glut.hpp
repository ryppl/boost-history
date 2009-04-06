/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#if defined(BOOST_GUIGL_GLUT_H)

#include BOOST_GUIGL_GLUT_H

#else

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#include "windows.h"
#endif

#include <GL/glut.h>

#endif

/*=================================---------------------------------------------
    Copyright 2008 Stjepan Rajko
  
    Distributed under the Boost Software License, Version 1.0.
    (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
-----------------------------------------------===============================*/

#if defined(BOOST_GUIGL_USE_FLTK_GLUT)

#include <Fl/glut.H>

#elif defined(__APPLE__)

#include <GLUT/glut.h>

#else

#include <GL/glut.h>

#endif

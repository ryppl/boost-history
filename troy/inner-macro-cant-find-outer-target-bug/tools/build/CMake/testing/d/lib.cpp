#include <iostream>
#include <string>
#include <boost/preprocessor/stringize.hpp>

#include "b.hpp"
#include "c.hpp"

// we DO see these
#ifndef B_STICKY_COMPILE_FLAGS
#error  B_STICKY_COMPILE_FLAGS
#endif
#ifndef C_STICKY_COMPILE_FLAGS
#error  C_STICKY_COMPILE_FLAGS
#endif
#ifndef D_COMPILE_FLAGS
#error D_COMPILE_FLAGS
#endif
#ifndef D_STICKY_COMPILE_FLAGS
#error D_STICKY_COMPILE_FLAGS
#endif

// we DONT see these
#ifdef A_COMPILE_FLAGS
#error A_COMPILE_FLAGS
#endif
#ifdef A_STICKY_COMPILE_FLAGS
#error A_STICKY_COMPILE_FLAGS
#endif
#ifdef B_COMPILE_FLAGS
#error B_COMPILE_FLAGS
#endif
#ifdef C_COMPILE_FLAGS
#error C_COMPILE_FLAGS
#endif


std::string d_linktype() { b_linktype(); c_linktype(); return BOOST_PP_STRINGIZE(LINKTYPE); }


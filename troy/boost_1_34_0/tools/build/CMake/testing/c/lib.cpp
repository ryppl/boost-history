#include <iostream>
#include <string>
#include <boost/preprocessor/stringize.hpp>

// we should see these
#ifndef C_COMPILE_FLAGS
#error C_COMPILE_FLAGS
#endif

#ifndef A_COMPILE_REQUIREMENTS
#error A_COMPILE_REQUIREMENTS
#endif

/// should NOT see these
#ifdef A_COMPILE_FLAGS
#error A_COMPILE_FLAGS
#endif

#ifdef B_COMPILE_FLAGS
#error B_COMPILE_FLAGS
#endif
#ifdef B_STICKY_COMPILE_FLAGS
#error B_STICKY_COMPILE_FLAGS
#endif

#include "a.hpp"

std::string c_linktype() { a_linktype(); return BOOST_PP_STRINGIZE(LINKTYPE); }


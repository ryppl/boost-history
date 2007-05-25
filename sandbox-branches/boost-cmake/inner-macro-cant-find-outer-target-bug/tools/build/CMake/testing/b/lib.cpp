#include <iostream>
#include <string>
#include <boost/preprocessor/stringize.hpp>

// we should see these
#ifndef B_COMPILE_FLAGS
#error B_COMPILE_FLAGS
#endif
#ifndef B_STICKY_COMPILE_FLAGS
#error B_STICKY_COMPILE_FLAGS
#endif

#ifndef A_STICKY_COMPILE_FLAGS
#error A_STICKY_COMPILE_FLAGS
#endif

/// should NOT see these
#ifdef A_COMPILE_FLAGS
#error A_COMPILE_FLAGS
#endif

#ifdef C_COMPILE_FLAGS
#error C_COMPILE_FLAGS
#endif
#ifdef C_STICKY_COMPILE_FLAGS
#error C_STICKY_COMPILE_FLAGS
#endif

#include "a.hpp"

std::string b_linktype() { a_linktype(); return BOOST_PP_STRINGIZE(LINKTYPE); }


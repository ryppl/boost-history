#include <iostream>
#include <string>
#include <boost/preprocessor/stringize.hpp>

#ifndef B_COMPILE_FLAGS
#error B_COMPILE_FLAGS
#endif
#ifndef B_STICKY_COMPILE_FLAGS
#error B_STICKY_COMPILE_FLAGS
#endif

std::string b_linktype() { return BOOST_PP_STRINGIZE(LINKTYPE); }


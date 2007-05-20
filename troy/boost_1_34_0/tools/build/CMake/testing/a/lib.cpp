#include <iostream>
#include <string>
#include <boost/preprocessor/stringize.hpp>
#ifndef A_COMPILE_FLAGS
#error A_COMPILE_FLAGS
#endif

// dont check (sticky)static/shared here, this gets compiled twice.
// tests check that things worked right via LINKTYPE

std::string a_linktype() { return BOOST_PP_STRINGIZE(LINKTYPE); }






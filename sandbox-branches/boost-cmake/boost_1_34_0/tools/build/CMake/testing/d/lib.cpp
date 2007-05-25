#include <iostream>
#include <string>
#include <boost/preprocessor/stringize.hpp>

#include "a.hpp"
#include "b.hpp"
#include "c.hpp"

// we DO see these
#ifndef A_COMPILE_REQUIREMENTS
#error  A_COMPILE_REQUIREMENTS
#endif
#ifndef B_COMPILE_REQUIREMENTS
#error  B_COMPILE_REQUIREMENTS
#endif
#ifndef C_COMPILE_REQUIREMENTS
#error  C_COMPILE_REQUIREMENTS
#endif
#ifndef D_COMPILE_FLAGS
#error D_COMPILE_FLAGS
#endif

std::string d_linktype() { a_linktype(); b_linktype(); c_linktype(); return BOOST_PP_STRINGIZE(LINKTYPE); }


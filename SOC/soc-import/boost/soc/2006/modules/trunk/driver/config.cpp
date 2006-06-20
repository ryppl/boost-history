#include "driver/config.h"
#include "map/map.h"

#include <boost/filesystem/path.hpp>

using namespace std;
using namespace boost::filesystem;

// clearly set up for Mac OS X 10.4.6: gcc 4.0.0
//  this'll have to get converted into a script that probes
//  the C preprocessor and GCC installation for include paths
//  and macros.
//
// Other platforms, e.g. VC, can be hardcoded as selections.

static vector<string> s_sys_inc_paths;
static map<string,string> s_macros;

const vector<string>&
config::
system_include_paths () {
	if (!s_sys_inc_paths.size ()) {
		s_sys_inc_paths.push_back ("/usr/include");
		s_sys_inc_paths.push_back ("/usr/include/c++/4.0.0");
		s_sys_inc_paths.push_back ("/usr/include/c++/4.0.0/powerpc-apple-darwin8");
		s_sys_inc_paths.push_back ("/usr/lib/gcc/powerpc-apple-darwin8/4.0.1/include");
	}
	return s_sys_inc_paths;
}

void
configure_mapmanager (MapManager& map) {
	const vector<string>& s = config::system_include_paths ();
	for (vector<string>::const_iterator it = s.begin ();
	     it != s.end ();
	     ++it) {
		map.add(path(*it));
	}
	map.add (path ("."));
}

/*
1. touch foo.cpp
2. cpp -dM foo.cpp
3. sed/gawk/perl your way to this.
Godspeed.
*/
const map<string,string>&
config::
macros () {
	if (!s_macros.size ()) {
		s_macros["__DBL_MIN_EXP__"]="(-1021)";
		s_macros["__FLT_MIN__"]="1.17549435e-38F";
		s_macros["__CHAR_BIT__"]="8";
		s_macros["__WCHAR_MAX__"]="2147483647";
		s_macros["__DBL_DENORM_MIN__"]="4.9406564584124654e-324";
		s_macros["__FLT_EVAL_METHOD__"]="0";
		s_macros["__DBL_MIN_10_EXP__"]="(-307)";
		s_macros["__FINITE_MATH_ONLY__"]="0";
		s_macros["__GNUC_PATCHLEVEL__"]="1";
		s_macros["__SHRT_MAX__"]="32767";
		s_macros["__LDBL_MAX__"]="1.79769313486231580793728971405301e+308L";
		s_macros["__APPLE_CC__"]="5247";
		s_macros["__UINTMAX_TYPE__"]="long long unsigned int";
		s_macros["__LDBL_MAX_EXP__"]="1024";
		s_macros["__SCHAR_MAX__"]="127";
		s_macros["__USER_LABEL_PREFIX__"]="_";
		s_macros["__STDC_HOSTED__"]="1";
		s_macros["__LDBL_HAS_INFINITY__"]="1";
		s_macros["__DBL_DIG__"]="15";
		s_macros["__FLT_EPSILON__"]="1.19209290e-7F";
		s_macros["__LDBL_MIN__"]="2.00416836000897277799610805135016e-292L";
		s_macros["__ppc__"]="1";
		s_macros["__strong"]="";
		s_macros["__APPLE__"]="1";
		s_macros["__DECIMAL_DIG__"]="33";
		s_macros["__LDBL_HAS_QUIET_NAN__"]="1";
		s_macros["__DYNAMIC__"]="1";
		s_macros["__GNUC__"]="4";
		s_macros["__DBL_MAX__"]="1.7976931348623157e+308";
		s_macros["__DBL_HAS_INFINITY__"]="1";
		s_macros["__weak"]="";
		s_macros["__DBL_MAX_EXP__"]="1024";
		s_macros["__LONG_LONG_MAX__"]="9223372036854775807LL";
		s_macros["__GXX_ABI_VERSION"]="1002";
		s_macros["__FLT_MIN_EXP__"]="(-125)";
		s_macros["__DBL_MIN__"]="2.2250738585072014e-308";
		s_macros["__DBL_HAS_QUIET_NAN__"]="1";
		s_macros["__REGISTER_PREFIX__"]="";
		s_macros["__NO_INLINE__"]="1";
		s_macros["_ARCH_PPC"]="1";
		s_macros["__FLT_MANT_DIG__"]="24";
		s_macros["__VERSION__"]="\"4.0.1 (Apple Computer, Inc. build 5247)\"";
		s_macros["__BIG_ENDIAN__"]="1";
		s_macros["__SIZE_TYPE__"]="long unsigned int";
		s_macros["__FLT_RADIX__"]="2";
		s_macros["__LDBL_EPSILON__"]="4.94065645841246544176568792868221e-324L";
		s_macros["__NATURAL_ALIGNMENT__"]="1";
		s_macros["__FLT_HAS_QUIET_NAN__"]="1";
		s_macros["__FLT_MAX_10_EXP__"]="38";
		s_macros["__LONG_MAX__"]="2147483647L";
		s_macros["__FLT_HAS_INFINITY__"]="1";
		s_macros["_BIG_ENDIAN"]="1";
		s_macros["__LDBL_MANT_DIG__"]="106";
		s_macros["__WCHAR_TYPE__"]="int";
		s_macros["__FLT_DIG__"]="6";
		s_macros["__INT_MAX__"]="2147483647";
		s_macros["__LONG_DOUBLE_128__"]="1";
		s_macros["__FLT_MAX_EXP__"]="128";
		s_macros["__DBL_MANT_DIG__"]="53";
		s_macros["__WINT_TYPE__"]="int";
		s_macros["__LDBL_MIN_EXP__"]="(-968)";
		s_macros["__MACH__"]="1";
		s_macros["__LDBL_MAX_10_EXP__"]="308";
		s_macros["__DBL_EPSILON__"]="2.2204460492503131e-16";
		s_macros["__INTMAX_MAX__"]="9223372036854775807LL";
		s_macros["__FLT_DENORM_MIN__"]="1.40129846e-45F";
		s_macros["__PIC__"]="1";
		s_macros["__FLT_MAX__"]="3.40282347e+38F";
		s_macros["__FLT_MIN_10_EXP__"]="(-37)";
		s_macros["__INTMAX_TYPE__"]="long long int";
		s_macros["__GNUC_MINOR__"]="0";
		s_macros["__DBL_MAX_10_EXP__"]="308";
		s_macros["__LDBL_DENORM_MIN__"]="4.94065645841246544176568792868221e-324L";
		s_macros["__PTRDIFF_TYPE__"]="int";
		s_macros["__LDBL_MIN_10_EXP__"]="(-291)";
		s_macros["__LDBL_DIG__"]="31";
		s_macros["__POWERPC__"]="1";
	}
	return s_macros;
}

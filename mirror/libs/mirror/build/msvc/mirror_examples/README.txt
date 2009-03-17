In order to work properly this solution needs
an additional property sheet file called
msvc-build.vsprops, containing several necessary 
configuration options, to be placed in the same 
directory where the Mirror root directory is
(i.e. not inside of the mirror root directory
but one directory above).

The purpose of this file is to define several 
user macros like the BOOST_ROOT (if not already 
defined), MIRROR_ROOT and optionally others, which 
specify various directories containing include,
library files and the build directory path.

There is a sample called msvc-build-sample.vsprops,
(in the same directory as this README is), which 
can be used as a template for this property sheet
file.

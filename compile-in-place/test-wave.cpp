/* example to demonstrate compile-in-place for Boost.Wave

Compile with
  $CXX -I path/to/boost_X_YY_Z test-wave.cpp -l $THREADLIBS

Notes:

 - The thread libs are e.g. "pthread" for Linux.

$Id$
*/

#define BOOST_WAVE_COMPILE_IN_PLACE
#define BOOST_FILESYSTEM_COMPILE_IN_PLACE
#define BOOST_THREAD_COMPILE_IN_PLACE
#define BOOST_SYSTEM_COMPILE_IN_PLACE
/* just include the sourcecode for one of the examples
This has the unfortunate disadvantage that it requires filesystem, thread and
system libs, too, hence the definitions above. */
#include <boost/../libs/wave/samples/quick_start/quick_start.cpp>

#include <boost/wave/compile_in_place.cpp>
#include <boost/filesystem/compile_in_place.cpp>
#include <boost/thread/compile_in_place.cpp>
#include <boost/system/compile_in_place.cpp>

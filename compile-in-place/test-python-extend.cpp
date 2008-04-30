/* example to demonstrate compile-in-place for extending Python

Compile with
  $CXX -I path/to/boost_X_YY_Z -I path/to/python-includes test-python-extend.cpp -shared -o hello.so

Notes:

- This must be compiled into a shared object (hello.so) or DLL (hello.dll) so
that you can load it from Python. The exact syntax for that probably differs
from above commandline.

- To load it, simply open an interactive interpreter in the same dir as the
compiled module and "import hello". You can then call the exported function
using "hello.greet()".

- Compiling with g++ -Wall will give you lots of warnings that a string
literal is converted to a non-const pointer because the Python functions are
completely ignorant of 'const'. You should be able to ignore those warnings
or turn them off with -Wno-write-strings.


$Id$
*/

#include <boost/python.hpp>
#include <boost/python/compile_in_place.cpp>

using namespace boost::python;

char const* greet()
{
    return "hello, world";
}

BOOST_PYTHON_MODULE(hello)
{
    def("greet", greet);
}


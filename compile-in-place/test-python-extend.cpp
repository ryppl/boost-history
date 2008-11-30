/* example to demonstrate compile-in-place for extending Python

Compile with
  $CXX -I path/to/boost_X_YY_Z -I path/to/python-includes test-python-extend.cpp -shared -o test_python_extend.so

Notes:

- This must be compiled into a shared object (*.so) or DLL (*.dll) so that you
can load it from Python. The exact syntax for that probably differs from above
commandline. Note: Python 2.5 deprecates the use of *.dll for modules in favor
of *.pyd and Python 2.6 removes support for *.dll entirely. I'm not sure if a
similar problem exists under POSIX platforms, too.

- To load it, simply open an interactive interpreter in the same dir as the
compiled module and "import test_python_extend". You can then call the exported
function using "test_python_extend.greet()". Note that the name used below in
the macro BOOST_PYTHON_MODULE must be the same as the filename of the module
without the extension, otherwise Python can't load it!

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

BOOST_PYTHON_MODULE(test_python_extend)
{
    def("greet", greet);
}


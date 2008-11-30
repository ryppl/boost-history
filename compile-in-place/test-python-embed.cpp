/* example to demonstrate compile-in-place for embedding Python

Compile with
  $CXX -I path/to/boost_X_YY_Z -I path/to/python-includes test-python-embed.cpp -l python-libs

Notes:

- This was originally developed with Boost 1.34, where the support for
embedding Python looked incomplete (see e.g. the necessary calls to Py_*).
This might be changed with 1.35 released now, need to check that.

- Compiling with g++ -Wall will give you lots of warnings that a string
literal is converted to a non-const pointer because the Python functions are
completely ignorant of 'const'. You should be able to ignore those warnings
or turn them off with -Wno-write-strings.


$Id$
*/

#define BOOST_PYTHON_COMPILE_IN_PLACE
#include <boost/python.hpp>
#include <boost/python/compile_in_place.cpp>
#include <iostream>
#include <ostream>
#include <string>

using namespace boost::python;

int main()
{
    Py_Initialize();

    object main_module(
        handle<>(borrowed(PyImport_AddModule("__main__"))));
    object main_namespace = main_module.attr("__dict__");

    object res((handle<>(PyRun_String(
        "'x'+'y'",
        Py_eval_input,
        main_namespace.ptr(),
        main_namespace.ptr())
    )));

    std::string const str = extract<std::string>(res);
    std::cout << "str=" << str << std::endl;

    Py_Finalize();
}

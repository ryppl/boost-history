// Example by Ralf W. Grosse-Kunstleve
// See root/libs/python/doc/cross_module.html for an introduction.

#include <boost/python/cross_module.hpp>
namespace python = boost::python;

#include "noncopyable.h"

namespace { // Avoid cluttering the global namespace.

  // A function with store objects as both input and output parameters.
  // Because the copy constructor is disabled, we cannot pass a store
  // object by value. Instead, we pass a smart pointer.
  std::auto_ptr<store> add_stores(const store& s1, const store& s2)
  {
    int sum = s1.recall() + s2.recall();
    std::auto_ptr<store> ss = std::auto_ptr<store>(new store(sum));
    return ss;
  }
}

BOOST_PYTHON_MODULE_INIT(noncopyable_import)
{
  try
  {
    python::module_builder this_module("noncopyable_import");

    python::import_converters<store>
      dvect_converters("noncopyable_export", "store");

    // Imagine all the additional classes with member functions
    // that have store objects as input and output parameters.
    // Lots and lots of them.
    // However, to keep this example simple, we only define a
    // module-level function.
    this_module.def(add_stores, "add_stores");
  }
  catch(...)
  {
    python::handle_exception(); // Deal with the exception for Python
  }
}

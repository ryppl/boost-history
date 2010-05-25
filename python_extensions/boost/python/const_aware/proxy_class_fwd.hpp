// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_CONST_AWARE_PROXY_CLASS_FWD_HPP
#define BOOST_PYTHON_CONST_AWARE_PROXY_CLASS_FWD_HPP

#include <boost/python.hpp>

namespace boost { namespace python { namespace const_aware {

// Construct a proxy class for a registered target (steals reference to target).
PyObject * construct_proxy_class(PyObject * target);

// Extract the target of a proxy class (returns a borrowed reference).
// Returns 0 on error, but does not set a Python exception.
PyObject * extract_proxy_target(PyObject * proxy);

struct proxy_class_base;
class proxy_class;

}}} // namespace boost::python::const_aware

#endif // !BOOST_PYTHON_CONST_AWARE_PROXY_CLASS_FWD_HPP

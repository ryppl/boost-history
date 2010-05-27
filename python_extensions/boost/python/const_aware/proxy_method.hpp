// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_CONST_AWARE_PROXY_METHOD_HPP
#define BOOST_PYTHON_CONST_AWARE_PROXY_METHOD_HPP

#include <boost/python.hpp>

namespace boost { namespace python { namespace const_aware {

// Construct a proxy method that calls a regular method on a proxy class.
object make_proxy_method(object const & target);

// Construct a property object by replacing the given property's fget with
// a proxy method and removing fset.
object make_proxy_property(object const & target);

// Construct a new property object by making a proxy method from the given
// getter.
object make_new_proxy_property(object const & fget, char const * doc=0);

}}} // namespace boost::python::const_aware

#endif // !BOOST_PYTHON_CONST_AWARE_PROXY_METHOD_HPP

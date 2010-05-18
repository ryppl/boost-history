// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_TO_PYTHON_IMPLICIT_HPP
#define BOOST_PYTHON_TO_PYTHON_IMPLICIT_HPP

#include <boost/python.hpp>

namespace boost {
namespace python {

/**
 *  @brief A to-python converter that converts an object of type Source to Python by first converting
 *         it to an object of type Target (which must have a to-python converter registered).
 *
 *  Source must be explicitly convertible to Target in C++.
 */
template <typename Source, typename Target>
struct implicit_to_python {
    typedef typename boost::python::copy_const_reference::apply<Target const &>::type Converter;

    static PyObject * convert(Source const & source) {
        Converter converter;
        Target target(source);
        return converter(target);
    }
    
    static PyTypeObject const * get_pytype() {
        Converter converter;
        return converter.get_pytype();
    }

    implicit_to_python() { declare(); }

    static void declare() { boost::python::to_python_converter<Source,implicit_to_python,true>(); }

};

} // namespace boost::python
} // namespace boost

#endif // !BOOST_PYTHON_TO_PYTHON_IMPLICIT_HPP

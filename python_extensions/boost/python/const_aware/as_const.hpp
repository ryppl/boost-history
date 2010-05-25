// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_CONST_AWARE_AS_CONST_HPP
#define BOOST_PYTHON_CONST_AWARE_AS_CONST_HPP

#include <boost/python.hpp>
#include <boost/python/const_aware/proxy_class.hpp>

namespace boost { namespace python {

template <typename Base = default_call_policies>
struct as_const : Base {
    static PyObject * postcall(PyObject *, PyObject * result) {
        return const_aware::construct_proxy_class(result);
    }
};

}} // namespace boost::python

#endif // !BOOST_PYTHON_CONST_AWARE_AS_CONST_HPP

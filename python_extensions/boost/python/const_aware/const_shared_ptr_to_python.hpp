// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_CONST_AWARE_CONST_SHARED_PTR_TO_PYTHON_HPP
#define BOOST_PYTHON_CONST_AWARE_CONST_SHARED_PTR_TO_PYTHON_HPP

#include <boost/python.hpp>
#include <boost/python/const_aware/proxy_class.hpp>
#include <boost/python/const_aware/registry.hpp>

namespace boost { namespace python { namespace const_aware {

template <typename Value>
struct const_shared_ptr_to_python {
    typedef typename boost::python::copy_const_reference::apply<shared_ptr<Value> const &>::type Converter;
    
    static PyObject * convert(shared_ptr<Value const> const & source) {
        Converter converter;
        shared_ptr<Value> target = boost::const_pointer_cast<Value>(source);
        PyObject * result = converter(target);
        if (result != 0)
            result = const_aware::construct_proxy_class(result);
        return result;
    }
    
    static PyTypeObject const * get_pytype() {
        Converter converter;
        return const_aware::registry::query(converter.get_pytype());
    }

    const_shared_ptr_to_python() {
        boost::python::to_python_converter<shared_ptr<Value const>, const_shared_ptr_to_python, true>();
    }

};

}}} // namespace boost::python::const_aware

#endif // !BOOST_PYTHON_CONST_AWARE_CONST_SHARED_PTR_TO_PYTHON_HPP

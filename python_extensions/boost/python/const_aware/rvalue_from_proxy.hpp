// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_CONST_AWARE_RVALUE_FROM_PROXY_HPP
#define BOOST_PYTHON_CONST_AWARE_RVALUE_FROM_PROXY_HPP

#include <boost/python.hpp>
#include <boost/python/object/find_instance.hpp>
#include <boost/python/const_aware/proxy_class_fwd.hpp>

namespace boost { namespace python { namespace const_aware {

template <typename T>
struct rvalue_from_proxy {
    
    static void* convertible(PyObject * arg) {
        PyObject * target = extract_proxy_target(arg);
        if (target == 0) return 0;
        converter::registration const & converters = converter::registered<T>::converters;
        return objects::find_instance_impl(target, converters.target_type, converters.is_shared_ptr);
    }

    static void construct(PyObject* obj, converter::rvalue_from_python_stage1_data* data) {
        data->construct = 0;
    }

    rvalue_from_proxy() {
        converter::registry::insert(
            &convertible,
            &construct,
            type_id< T >()
        );
    }

};

}}} // namespace boost::python::const_aware

#endif // !BOOST_PYTHON_CONST_AWARE_RVALUE_FROM_PROXY_HPP

// Copyright 2010 Jim Bosch.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef BOOST_PYTHON_CONST_AWARE_SHARED_PTR_FROM_PROXY_HPP
#define BOOST_PYTHON_CONST_AWARE_SHARED_PTR_FROM_PROXY_HPP

#include <boost/python.hpp>
#include <boost/python/object/find_instance.hpp>
#include <boost/python/const_aware/proxy_class_fwd.hpp>

namespace boost { namespace python { namespace const_aware {

template <typename T>
struct shared_ptr_from_proxy {

    shared_ptr_from_proxy()
    {
        converter::registry::insert(&convertible, &construct, type_id<shared_ptr<T> >()
#ifndef BOOST_PYTHON_NO_PY_SIGNATURES
                                    , &converter::expected_from_python_type_direct<T>::get_pytype
#endif
        );
    }

 private:
    static void* convertible(PyObject* p)
    {
        if (p == Py_None)
            return p;
        PyObject * target = extract_proxy_target(p);
        if (target == 0) return 0;
        return converter::get_lvalue_from_python(target, converter::registered<T>::converters);
    }
    
    static void construct(PyObject* source, converter::rvalue_from_python_stage1_data* data)
    {
        void* const storage = ((converter::rvalue_from_python_storage<shared_ptr<T> >*)data)->storage.bytes;
        // Deal with the "None" case.
        if (data->convertible == Py_None)
            new (storage) shared_ptr<T>();
        else
        {
            boost::shared_ptr<void> hold_convertible_ref_count(
                (void*)0, converter::shared_ptr_deleter(handle<>(borrowed(source))) );
            // use aliasing constructor
            new (storage) shared_ptr<T>(
                hold_convertible_ref_count,
                static_cast<T*>(data->convertible));
        }
        
        data->convertible = storage;
    }

};

}}} // namespace boost::python::const_aware

#endif // !BOOST_PYTHON_CONST_AWARE_SHARED_PTR_FROM_PROXY_HPP

// Copyright 2010 Jim Bosch
// Adapted from Boost.Python code, Copyright David Abrahams, 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python.hpp>
#include <boost/python/const_aware/proxy_class.hpp>
#include <boost/python/const_aware/proxy_method.hpp>
#include <structmember.h>

// Most of this is copied with minor modifications from object/function.cpp in the main Boost.Python library.

namespace boost { namespace python { namespace const_aware { 

struct proxy_method {
    PyObject_HEAD
    PyObject* target;
};

extern "C"
{
    // Stolen from Python's funcobject.c
    static PyObject *
    proxy_method_descr_get(PyObject *func, PyObject *obj, PyObject *type_)
    {
#if PY_VERSION_HEX >= 0x03000000
        // The implement is different in Python 3 because of the removal of unbound method
        if (obj == Py_None || obj == NULL) {
            Py_INCREF(func);
            return func;
        }
        return PyMethod_New(func, obj);
#else
        if (obj == Py_None)
            obj = NULL;
        return PyMethod_New(func, obj, type_);
#endif
    }

    static void
    proxy_method_dealloc(PyObject* p) {
        proxy_method* self = (proxy_method*)p;
        Py_DECREF(self->target);
        Py_TYPE(p)->tp_free(p);
    }

    static PyObject *
    proxy_method_call(PyObject *func, PyObject *args, PyObject *kw) {
        proxy_method* self = downcast<proxy_method>(func);
        Py_ssize_t n_args = PyTuple_GET_SIZE(args);
        if (n_args < 1) {
            PyErr_SetString(PyExc_ValueError, "proxy_method.__call__ requires at least one argument");
            return 0;
        }
        PyObject * proxy_arg = PyTuple_GET_ITEM(args,0);
        PyObject * target_arg = extract_proxy_target(proxy_arg);
        if (target_arg == 0) {
            PyErr_SetString(PyExc_ValueError,
                            "proxy_method.__call__ requires the first argument to be a proxy_instance");
            return 0;
        }
        PyObject * new_args = PyTuple_New(n_args);
        Py_INCREF(target_arg);
        PyTuple_SET_ITEM(new_args, 0, target_arg);
        for (Py_ssize_t n = 1; n < n_args; ++n) {
            PyObject * arg_n = PyTuple_GET_ITEM(args, n);
            Py_INCREF(arg_n);
            PyTuple_SET_ITEM(new_args, n, arg_n);
        }
        PyObject * result = PyObject_Call(self->target, new_args, kw);
        Py_DECREF(new_args);
        return result;
    }

    //
    // Here we're using the function's tp_getset rather than its
    // tp_members to set up __doc__ and __name__, because tp_members
    // really depends on having a POD object type (it relies on
    // offsets). It might make sense to reformulate function as a POD
    // at some point, but this is much more expedient.
    //
    static PyObject* proxy_method_get_doc(PyObject* op, void*) {
        proxy_method* self = downcast<proxy_method>(op);
        return ::PyObject_GetAttrString(self->target, "__doc__");
    }
    
    static int proxy_method_set_doc(PyObject* op, PyObject* doc, void*) {
        proxy_method* self = downcast<proxy_method>(op);
        return ::PyObject_SetAttrString(self->target, "__doc__", doc);
    }
    
    static PyObject* proxy_method_get_name(PyObject* op, void*) {
        proxy_method* self = downcast<proxy_method>(op);
        return ::PyObject_GetAttrString(self->target, "__name__");
    }

    // We add a dummy __class__ attribute in order to fool PyDoc into
    // treating these as built-in functions and scanning their
    // documentation
    static PyObject* proxy_method_get_class(PyObject* /*op*/, void*) {
        return python::incref(upcast<PyObject>(&PyCFunction_Type));
    }
}

static PyGetSetDef proxy_method_getsetlist[] = {
    {const_cast<char*>("__name__"), (getter)proxy_method_get_name, 0, 0, 0 },
    {const_cast<char*>("func_name"), (getter)proxy_method_get_name, 0, 0, 0 },
    {const_cast<char*>("__class__"), (getter)proxy_method_get_class, 0, 0, 0 },    // see note above
    {const_cast<char*>("__doc__"), (getter)proxy_method_get_doc, (setter)proxy_method_set_doc, 0, 0},
    {const_cast<char*>("func_doc"), (getter)proxy_method_get_doc, (setter)proxy_method_set_doc, 0, 0},
    {NULL, 0, 0, 0, 0} /* Sentinel */
};

PyTypeObject proxy_method_type_object = {
    PyVarObject_HEAD_INIT(NULL, 0)
    const_cast<char*>("Boost.Python.proxy_method"),
    sizeof(proxy_method),
    0,
    (destructor)proxy_method_dealloc,   /* tp_dealloc */
    0,                                  /* tp_print */
    0,                                  /* tp_getattr */
    0,                                  /* tp_setattr */
    0,                                  /* tp_compare */
    0, //(reprfunc)func_repr,           /* tp_repr */
    0,                                  /* tp_as_number */
    0,                                  /* tp_as_sequence */
    0,                                  /* tp_as_mapping */
    0,                                  /* tp_hash */
    proxy_method_call,                  /* tp_call */
    0,                                  /* tp_str */
    0, // PyObject_GenericGetAttr,      /* tp_getattro */
    0, // PyObject_GenericSetAttr,      /* tp_setattro */
    0,                                  /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT /* | Py_TPFLAGS_HAVE_GC */,/* tp_flags */
    0,                                  /* tp_doc */
    0, // (traverseproc)func_traverse,  /* tp_traverse */
    0,                                  /* tp_clear */
    0,                                  /* tp_richcompare */
    0, //offsetof(PyFunctionObject, func_weakreflist), /* tp_weaklistoffset */
    0,                                  /* tp_iter */
    0,                                  /* tp_iternext */
    0,                                  /* tp_methods */
    0, // func_memberlist,              /* tp_members */
    proxy_method_getsetlist,            /* tp_getset */
    0,                                  /* tp_base */
    0,                                  /* tp_dict */
    proxy_method_descr_get,             /* tp_descr_get */
    0,                                  /* tp_descr_set */
    0, //offsetof(PyFunctionObject, func_dict),      /* tp_dictoffset */
    0,                                      /* tp_init */
    0,                                      /* tp_alloc */
    0,                                      /* tp_new */
    0,                                      /* tp_free */
    0,                                      /* tp_is_gc */
    0,                                      /* tp_bases */
    0,                                      /* tp_mro */
    0,                                      /* tp_cache */
    0,                                      /* tp_subclasses */
    0,                                      /* tp_weaklist */
#if PYTHON_API_VERSION >= 1012
    0                                       /* tp_del */
#endif
};

type_handle proxy_method_type() {
    if (proxy_method_type_object.tp_dict == 0) {
        if (PyType_Ready(&proxy_method_type_object))
            return type_handle();
    }
    return type_handle(borrowed(&proxy_method_type_object));
}

object make_proxy_method(object const & target) {
    if (!PyCallable_Check(target.ptr())) {
        PyErr_SetString(PyExc_TypeError, "proxy_method target must be callable");
        throw_error_already_set();
    }
    PyObject * raw_proxy = proxy_method_type()->tp_alloc(proxy_method_type().get(), 0);
    if (raw_proxy != 0) {
        proxy_method * self = (proxy_method*)raw_proxy;
        self->target = python::incref(target.ptr());
    }
    return object(handle<>(raw_proxy));
}

object make_proxy_property(object const & target) {
    object old_fget = target.attr("fget");
    object doc = target.attr("__doc__");
    object new_fget = make_proxy_method(old_fget);
    object result(
        (python::detail::new_reference)
        PyObject_CallFunction((PyObject*)&PyProperty_Type, 
                              const_cast<char*>("OssO"), new_fget.ptr(), 0, 0, doc.ptr())
    );
    return result;
}

object make_new_proxy_property(object const & fget, char const * doc) {
    object new_fget = make_proxy_method(fget);
    object result(
        (python::detail::new_reference)
        PyObject_CallFunction((PyObject*)&PyProperty_Type, 
                              const_cast<char*>("Osss"), new_fget.ptr(), 0, 0, doc)
    );
    return result;
}

}}} // namespace boost::python::const_aware

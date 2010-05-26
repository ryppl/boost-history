// Copyright 2010 Jim Bosch
// Adapted from Boost.Python code, Copyright David Abrahams, 2002.
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python.hpp>
#include <boost/python/const_aware/proxy_class.hpp>
#include <boost/python/const_aware/registry.hpp>
#include <structmember.h>

// Most of this is copied with minor modifications from object/class.cpp in the main Boost.Python library.

namespace boost { namespace python { namespace const_aware { 

struct proxy_instance {
    PyObject_HEAD
    PyObject* dict;
    PyObject* weakrefs;
    PyObject* target;
};

extern "C" {
    
    static int
    type_is_gc(PyTypeObject *python_type) {
        return python_type->tp_flags & Py_TPFLAGS_HEAPTYPE;
    }
      
    static void proxy_instance_dealloc(PyObject* inst) {
        proxy_instance* kill_me = (proxy_instance*)inst;
        
        Py_DECREF(kill_me->target);

        // Python 2.2.1 won't add weak references automatically when
        // tp_itemsize > 0, so we need to manage that
        // ourselves. Accordingly, we also have to clean up the
        // weakrefs ourselves.
        if (kill_me->weakrefs != NULL)
            PyObject_ClearWeakRefs(inst);
        
        Py_XDECREF(kill_me->dict);
        
        Py_TYPE(inst)->tp_free(inst);
    }
    
    static PyObject *
    proxy_instance_new(PyTypeObject* type_, PyObject* args, PyObject* kw) {
        static char * kwds[] = {const_cast<char*>("target"), NULL};
        PyObject * target_type = PyObject_GetAttrString((PyObject*)type_, "__proxy_target__");
        if (target_type == 0) return 0;
        PyObject * target = 0;
        if (!PyArg_ParseTupleAndKeywords(args, kw, "O!", kwds, target_type, &target))
            return 0;
        proxy_instance* result = (proxy_instance*)type_->tp_alloc(type_, 0);
        if (result != 0) {
            Py_INCREF(target);
            result->target = target;
        }
        return (PyObject*)result;
    }
    
    static PyObject* proxy_instance_get_dict(PyObject* op, void*) {
        proxy_instance* inst = downcast<proxy_instance>(op);
        if (inst->dict == 0)
            inst->dict = PyDict_New();
        return python::xincref(inst->dict);
    }
    
    static int proxy_instance_set_dict(PyObject* op, PyObject* dict, void*) {
        proxy_instance* inst = downcast<proxy_instance>(op);
        python::xdecref(inst->dict);
        inst->dict = python::incref(dict);
        return 0;
    }

}  // extern "C"

// Mostly copied from class_metatype_object from object/class.cpp in the main Boost.Python library.
static PyTypeObject proxy_class_metatype_object = {
    PyVarObject_HEAD_INIT(NULL, 0)
    const_cast<char*>("Boost.Python.proxy_class"),
    PyType_Type.tp_basicsize,
    0,
    0,                                      /* tp_dealloc */
    0,                                      /* tp_print */
    0,                                      /* tp_getattr */
    0,                                      /* tp_setattr */
    0,                                      /* tp_compare */
    0,                                      /* tp_repr */
    0,                                      /* tp_as_number */
    0,                                      /* tp_as_sequence */
    0,                                      /* tp_as_mapping */
    0,                                      /* tp_hash */
    0,                                      /* tp_call */
    0,                                      /* tp_str */
    0,                                      /* tp_getattro */
    0,                                      /* tp_setattro */
    0,                                      /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT // | Py_TPFLAGS_HAVE_GC
    | Py_TPFLAGS_BASETYPE,          /* tp_flags */
    0,                                      /* tp_doc */
    0,                                      /* tp_traverse */
    0,                                      /* tp_clear */
    0,                                      /* tp_richcompare */
    0,                                      /* tp_weaklistoffset */
    0,                                      /* tp_iter */
    0,                                      /* tp_iternext */
    0,                                      /* tp_methods */
    0,                                      /* tp_members */
    0,                                      /* tp_getset */
    0, //&PyType_Type,                           /* tp_base */
    0,                                      /* tp_dict */
    0,                                      /* tp_descr_get */
    0,                                      /* tp_descr_set */
    0,                                      /* tp_dictoffset */
    0,                                      /* tp_init */
    0,                                      /* tp_alloc */
    0, // filled in with type_new           /* tp_new */
    0, // filled in with __PyObject_GC_Del  /* tp_free */
    (inquiry)type_is_gc,                    /* tp_is_gc */
    0,                                      /* tp_bases */
    0,                                      /* tp_mro */
    0,                                      /* tp_cache */
    0,                                      /* tp_subclasses */
    0,                                      /* tp_weaklist */
#if PYTHON_API_VERSION >= 1012
    0                                       /* tp_del */
#endif
};

static PyGetSetDef proxy_instance_getsets[] = {
    {const_cast<char*>("__dict__"),  proxy_instance_get_dict,  proxy_instance_set_dict, NULL, 0},
    {0, 0, 0, 0, 0}
};

  
static PyMemberDef proxy_instance_members[] = {
    {const_cast<char*>("__weakref__"), T_OBJECT, offsetof(proxy_instance, weakrefs), 0, 0},
    {0, 0, 0, 0, 0}
};

static PyTypeObject proxy_class_type_object = {
    PyVarObject_HEAD_INIT(NULL,0)
    const_cast<char*>("Boost.Python.proxy_instance"),
    sizeof(proxy_instance),                 /* tp_basicsize */
    0,                                      /* tp_itemsize */
    proxy_instance_dealloc,                 /* tp_dealloc */
    0,                                      /* tp_print */
    0,                                      /* tp_getattr */
    0,                                      /* tp_setattr */
    0,                                      /* tp_compare */
    0,                                      /* tp_repr */
    0,                                      /* tp_as_number */
    0,                                      /* tp_as_sequence */
    0,                                      /* tp_as_mapping */
    0,                                      /* tp_hash */
    0,                                      /* tp_call */
    0,                                      /* tp_str */
    0,                                      /* tp_getattro */
    0,                                      /* tp_setattro */
    0,                                      /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT // | Py_TPFLAGS_HAVE_GC
    | Py_TPFLAGS_BASETYPE,          /* tp_flags */
    0,                                      /* tp_doc */
    0,                                      /* tp_traverse */
    0,                                      /* tp_clear */
    0,                                      /* tp_richcompare */
    offsetof(proxy_instance,weakrefs),      /* tp_weaklistoffset */
    0,                                      /* tp_iter */
    0,                                      /* tp_iternext */
    0,                                      /* tp_methods */
    proxy_instance_members,                 /* tp_members */
    proxy_instance_getsets,                 /* tp_getset */
    0, //&PyBaseObject_Type,                /* tp_base */
    0,                                      /* tp_dict */
    0,                                      /* tp_descr_get */
    0,                                      /* tp_descr_set */
    offsetof(proxy_instance,dict),          /* tp_dictoffset */
    0,                                      /* tp_init */
    PyType_GenericAlloc,                    /* tp_alloc */
    proxy_instance_new,                     /* tp_new */
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

type_handle proxy_class_metatype()   {
    if (proxy_class_metatype_object.tp_dict == 0) {
        Py_TYPE(&proxy_class_metatype_object) = &PyType_Type;
        proxy_class_metatype_object.tp_base = &PyType_Type;
        if (PyType_Ready(&proxy_class_metatype_object))
            return type_handle();
    }
    return type_handle(borrowed(&proxy_class_metatype_object));
}

type_handle proxy_class_type() {
    if (proxy_class_type_object.tp_dict == 0) {
        Py_TYPE(&proxy_class_type_object) = incref(proxy_class_metatype().get());
        proxy_class_type_object.tp_base = &PyBaseObject_Type;
        if (PyType_Ready(&proxy_class_type_object))
            return type_handle();
    }
    return type_handle(borrowed(&proxy_class_type_object));
}

namespace { // <unnamed>

object module_prefix() {
    return object(
        PyObject_IsInstance(scope().ptr(), upcast<PyObject>(&PyModule_Type))
        ? object(scope().attr("__name__"))
        : api::getattr(scope(), "__module__", str())
    );
}

// Find a registered extension class object corresponding to id. Return a
// null handle if no such class is registered.
inline type_handle query_target_class(type_info id) {
    converter::registration const* p = converter::registry::query(id);
    return type_handle(
        python::borrowed(
                python::allow_null(p ? p->m_class_object : 0))
    );
}

// Find a registered extension corresponding to id. If not found,
// throw an appropriate exception.
type_handle get_target_class(type_info id) {
    type_handle result(query_target_class(id));

    if (result.get() == 0)
        {
            object report("extension class wrapper for proxy target class ");
            report = report + id.name() + " has not been created yet";
            PyErr_SetObject(PyExc_RuntimeError, report.ptr());
            throw_error_already_set();
        }
    return result;
}

// Find a registered proxy class object corresponding to id. Return a
// null handle if no such class is registered.
inline type_handle query_proxy_class(type_info id) {
    converter::registration const* p = converter::registry::query(id);
    return type_handle(
        python::borrowed(
                python::allow_null(p ? const_aware::registry::query(p->m_class_object) : 0))
    );
}

// Find a registered proxy class corresponding to id. If not found,
// throw an appropriate exception.
type_handle get_proxy_class(type_info id) {
    type_handle result(query_proxy_class(id));

    if (result.get() == 0)
        {
            object report("proxy class wrapper for base class ");
            report = report + id.name() + " has not been created yet";
            PyErr_SetObject(PyExc_RuntimeError, report.ptr());
            throw_error_already_set();
        }
    return result;
}

// proxy_class_base constructor
//
// name -      the name of the new Python class
//
// num_types - one more than the number of declared bases
//
// types -     array of python::type_info, the first item
//             corresponding to the class being created, and the
//             rest corresponding to its declared bases.
//
inline object
new_proxy_class(char const* name, std::size_t num_types, type_info const* const types, char const* doc) {
    assert(num_types >= 1);
    
    // Build a tuple of the base Python type objects. If no bases
    // were declared, we'll use our class_type() as the single base
    // class.
    ssize_t const num_bases = (std::max)(num_types - 1, static_cast<std::size_t>(1));
    handle<> bases(PyTuple_New(num_bases));

    for (ssize_t i = 1; i <= num_bases; ++i) {
        type_handle c = (i >= static_cast<ssize_t>(num_types)) 
            ? proxy_class_type() : get_proxy_class(types[i]);
        // PyTuple_SET_ITEM steals this reference
        PyTuple_SET_ITEM(bases.get(), static_cast<ssize_t>(i - 1), upcast<PyObject>(c.release()));
    }
    
    // Call the class metatype to create a new class
    dict d;
    
    object m = module_prefix();
    if (m) d["__module__"] = m;
        
    if (doc != 0)
        d["__doc__"] = doc;

    type_handle target = get_target_class(types[0]);
    d["__proxy_target__"] = object(target);

    object result = object(proxy_class_metatype())(name, bases, d);
    assert(PyType_IsSubtype(Py_TYPE(result.ptr()), &PyType_Type));
    
    if (scope().ptr() != Py_None)
        scope().attr(name) = result;
    
    return result;
}

} // <unnamed>
  
proxy_class_base::proxy_class_base(
    char const* name, std::size_t num_types, type_info const* const types, char const* doc)
    : object(new_proxy_class(name, num_types, types, doc))
{
    // Insert the new class object in the const_registry
    // Class object is leaked, for now
    object target = this->attr("__proxy_target__");
    const_aware::registry::insert(
        (PyTypeObject*)target.ptr(),
        (PyTypeObject*)incref(this->ptr())
    );
}

void proxy_class_base::setattr(char const* name, object const& x) {
    if (PyObject_SetAttrString(this->ptr(), const_cast<char*>(name), x.ptr()) < 0)
        throw_error_already_set();
}

PyObject * construct_proxy_class(PyObject * target_arg) {
    handle<> target(target_arg);
    PyTypeObject * target_type = Py_TYPE(target.get());
    PyTypeObject * proxy_type = const_aware::registry::query(target_type);
    if (proxy_type == 0) {
        object report("no const proxy class registered for objects of type ");
        report = report + str(object(type_handle(borrowed(target_type))));
        PyErr_SetObject(PyExc_RuntimeError, report.ptr());
        throw_error_already_set();
    }
    PyObject * raw_proxy = proxy_type->tp_alloc(proxy_type, 0);
    if (raw_proxy != 0) {
        proxy_instance * proxy = (proxy_instance*)raw_proxy;
        proxy->target = target.release();
    }
    return raw_proxy;
}

PyObject * extract_proxy_target(PyObject * proxy) {
    if (Py_TYPE(Py_TYPE(proxy)) != &proxy_class_metatype_object)
        return 0;
    proxy_instance* self = reinterpret_cast<proxy_instance*>(proxy);
    return self->target;
}

}}} // namespace boost::python::const_aware

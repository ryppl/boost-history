#include <boost/python.hpp>
#include <boost/python/const_aware_class.hpp>

#include <iostream>
#include <map>

namespace bp = boost::python;

namespace boost { namespace python { namespace const_aware_registry {

namespace {
typedef std::map<PyTypeObject*,PyTypeObject*> registry_t;
registry_t registry;
}

PyTypeObject * lookup(PyTypeObject* non_const) {
    registry_t::const_iterator iter = registry.find(non_const);
    if (iter != registry.end()) return iter->second;
    return NULL;
}

void insert(object const & non_const, object const & const_) {
    registry.insert(
        registry_t::value_type(
            reinterpret_cast<PyTypeObject*>(non_const.ptr()),
            reinterpret_cast<PyTypeObject*>(const_.ptr())
        )
    );
}

}}}

class Example {
public:
    std::size_t get_address() const { return std::size_t(this); }

    bool non_const_method() { return true; }

    bool const_method() const { return true; }
};

class Owner {
public:
    Example by_value() const { return _example; }
    Example const by_const_value() const { return _example; }
    Example & by_reference() { return _example; }
    Example const & by_const_reference() const { return _example; }
private:
    Example _example;
};

static void export_module() {
    
    bp::const_aware_class<Example>("Example", "FrozenExample")
        .add_property("address", &Example::get_address)
        .def("non_const_method", &Example::non_const_method)
        .def("const_method", &Example::const_method)
        ;

    bp::class_<Owner>("Owner")
        .def("by_value", &Owner::by_value)
        .def("by_const_value", &Owner::by_const_value)
        .def("by_reference", &Owner::by_reference, bp::return_internal_reference<>())
        .def("by_const_reference", &Owner::by_const_reference, bp::return_internal_const_reference<>())
        ;

}

BOOST_PYTHON_MODULE(const_aware) {
    export_module();
}

#include <boost/python.hpp>
#include <boost/python/const_aware.hpp>
#include <boost/shared_ptr.hpp>

namespace bp = boost::python;

class Example {
public:
    Example() : value(0) {}

    std::size_t get_address() const { return std::size_t(this); }

    bool non_const_method() { return true; }
    bool const_method() const { return true; }

    int value;

    int get_value() const { return value; }
    void set_value(int v) { value = v; }
};

class Owner {
public:
    Example by_value() const { return *_example; }
    Example const by_const_value() const { return *_example; }
    Example & by_reference() { return *_example; }
    Example const & by_const_reference() const { return *_example; }
    boost::shared_ptr<Example> by_shared_ptr() { return _example; }
    boost::shared_ptr<Example const> by_const_shared_ptr() const { return _example; }

    bool accept_by_value(Example x) { return true; }
    bool accept_by_const_value(Example const x) { return true; }
    bool accept_by_reference(Example & x) { return true; }
    bool accept_by_const_reference(Example const & x) { return true; }
    bool accept_by_shared_ptr(boost::shared_ptr<Example> const & x) { return true; }
    bool accept_by_const_shared_ptr(boost::shared_ptr<Example const> const & x) { return true; }

    explicit Owner() : _example(new Example()) {}
private:
    boost::shared_ptr<Example> _example;
};

static void export_module() {

    bp::class_<Example> pyExample("Example");
    pyExample.def(bp::init<Example const &>());
        
    bp::make_const_aware(pyExample, "FrozenExample")
        .add_property("address", &Example::get_address)
        .def("non_const_method", &Example::non_const_method)
        .def("const_method", &Example::const_method)
        .add_property("value_prop", &Example::get_value, &Example::set_value)
        .def_readonly("value_ro", &Example::value)
        .def_readwrite("value_rw", &Example::value)
        .enable_shared_ptr();
        ;

    bp::class_<Owner>("Owner")
        .def("by_value", &Owner::by_value)
        .def("by_const_value", &Owner::by_const_value, bp::as_const<>())
        .def("by_reference", &Owner::by_reference, bp::return_internal_reference<>())
        .def("by_const_reference", &Owner::by_const_reference, 
             bp::as_const< bp::return_internal_reference<> >())
        .def("by_shared_ptr", &Owner::by_shared_ptr)
        .def("by_const_shared_ptr", &Owner::by_const_shared_ptr)
        .def("accept_by_value", &Owner::accept_by_value)
        .def("accept_by_const_value", &Owner::accept_by_const_value)
        .def("accept_by_reference", &Owner::accept_by_reference)
        .def("accept_by_const_reference", &Owner::accept_by_const_reference)
        .def("accept_by_shared_ptr", &Owner::accept_by_shared_ptr)
        .def("accept_by_const_shared_ptr", &Owner::accept_by_const_shared_ptr)
        ;

}

BOOST_PYTHON_MODULE(const_aware) {
    export_module();
}

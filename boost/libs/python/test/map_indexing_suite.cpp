// Copyright Joel de Guzman 2004. Distributed under the Boost
// Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/module.hpp>
#include <boost/python/def.hpp>
#include <boost/python/implicit.hpp>

using namespace boost::python;

struct X // a container element
{
    std::string s;
    X():s("default") {}
    X(std::string s):s(s) {}
    std::string repr() const { return s; }
    void reset() { s = "reset"; }
    void foo() { s = "foo"; }
    bool operator==(X const& x) const { return s == x.s; }
    bool operator!=(X const& x) const { return s != x.s; }
};

std::string x_value(X const& x)
{
    return "gotya " + x.s;
}

struct A
{
  int value;
  A() : value(0){};
  A(int v) : value(v) {};
};

bool operator==(const A& v1, const A& v2)
{
  return (v1.value == v2.value);
}

struct B
{
  A a;
};

// Converter from A to python int
struct AToPython
{
  static PyObject* convert(const A& s)
  {
    return boost::python::incref(boost::python::object((int)s.value).ptr());
  }
};

// Conversion from python int to A
struct AFromPython
{
  AFromPython()
  {
    boost::python::converter::registry::push_back(
        &convertible,
        &construct,
        boost::python::type_id< A >());
  }

  static void* convertible(PyObject* obj_ptr)
  {
    if (!PyInt_Check(obj_ptr)) return 0;
    return obj_ptr;
  }

  static void construct(
      PyObject* obj_ptr,
      boost::python::converter::rvalue_from_python_stage1_data* data)
  {
    void* storage = (
        (boost::python::converter::rvalue_from_python_storage< A >*)
        data)-> storage.bytes;

    new (storage) A((int)PyInt_AsLong(obj_ptr));
    data->convertible = storage;
  }
};

BOOST_PYTHON_MODULE(map_indexing_suite_ext)
{
    class_<X>("X")
        .def(init<>())
        .def(init<X>())
        .def(init<std::string>())
        .def("__repr__", &X::repr)
        .def("reset", &X::reset)
        .def("foo", &X::foo)
    ;

    def("x_value", x_value);
    implicitly_convertible<std::string, X>();

    class_<std::map<std::string, X> >("XMap")
        .def(map_indexing_suite<std::map<std::string, X> >())
    ;

    void int_map_indexing_suite(); // moved to int_map_indexing_suite.cpp to 
    int_map_indexing_suite();      // avoid MSVC 6/7 internal structure overflow
    
#if 0
    // Compile check only...
    class_<std::map<int, int> >("IntMap")
        .def(map_indexing_suite<std::map<int, int> >())
    ;
#endif 

    // Some more..
    class_<std::map<std::string, boost::shared_ptr<X> > >("TestMap")
        .def(map_indexing_suite<std::map<std::string, boost::shared_ptr<X> >, true>())
    ;

  to_python_converter< A , AToPython >();
  AFromPython();

  class_< std::map<int, A> >("AMap")
    .def(map_indexing_suite<std::map<int, A>, true >())
    ;

  class_< B >("B")
    .add_property("a", make_getter(&B::a, return_value_policy<return_by_value>()),
        make_setter(&B::a, return_value_policy<return_by_value>()))
    ;
}

#include "module_tail.cpp"

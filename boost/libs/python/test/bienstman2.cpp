#include <boost/python/module_init.hpp>
#include <boost/python/def.hpp>
#include <boost/python/class.hpp>

struct C {};

struct D {};

struct E
{
   const D fe (const C&)           {return D();}
   const D fe2(const C&, const C&) {return D();}
};

BOOST_PYTHON_MODULE_INIT(bienstman2_ext)
{
  using namespace boost::python;

  class_<C>("C");
  class_<D>("D");
  class_<E>("E")
      .def("fe",  &E::fe)  // this compiles.
      .def("fe2", &E::fe2) // this doesn't... well, now it does ;-)
      ;
}

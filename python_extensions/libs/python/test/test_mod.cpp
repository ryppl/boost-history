#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/to_python/copy_to_list.hpp>
#include <boost/python/from_python/container.hpp>

namespace bp = boost::python;

static std::vector<int> return_int_vector() {
    std::vector<int> result;
    result.push_back(5);
    result.push_back(3);
    return result;
}

static bool accept_int_vector_cref(std::vector<int> const & v) {
    return v[0] == 5 && v[1] == 3;
}

static bool accept_int_vector_ref(std::vector<int> & v) {
    return v[0] == 5 && v[1] == 3;
}

BOOST_PYTHON_MODULE(test_mod) {
    bp::class_< std::vector<int> >("int_vector")
        .def(bp::vector_indexing_suite< std::vector<int> >())
        ;
    bp::container_from_python_sequence< std::vector<int>, int >::declare();

    bp::def("return_int_vector_as_list", &return_int_vector, bp::return_value_policy<bp::copy_to_list>());
    bp::def("return_int_vector_wrapped", &return_int_vector);

    bp::def("accept_int_vector_cref", &accept_int_vector_cref);
    bp::def("accept_int_vector_ref", &accept_int_vector_ref);
}


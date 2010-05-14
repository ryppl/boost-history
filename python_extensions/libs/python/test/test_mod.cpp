#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>
#include <boost/python/suite/indexing/map_indexing_suite.hpp>
#include <boost/python/to_python/copy_to_list.hpp>
#include <boost/python/to_python/copy_to_tuple.hpp>
#include <boost/python/to_python/copy_to_dict.hpp>
#include <boost/python/from_python/container.hpp>
#include <boost/python/to_python/boost_fusion.hpp>
#include <boost/python/from_python/boost_fusion.hpp>
#include <boost/python/to_python/std_pair.hpp>
#include <boost/python/from_python/std_pair.hpp>
#include <boost/python/to_python/const_cast_shared_ptr.hpp>

#include <boost/fusion/tuple.hpp>
#include <boost/fusion/sequence/intrinsic/at_c.hpp>

#include <string>
#include <vector>
#include <map>

namespace bp = boost::python;

static std::vector<int> return_vector() {
    std::vector<int> result;
    result.push_back(5);
    result.push_back(3);
    return result;
}

static bool accept_vector_cref(std::vector<int> const & v) {
    return v[0] == 5 && v[1] == 3;
}

static bool accept_vector_ref(std::vector<int> & v) {
    return v[0] == 5 && v[1] == 3;
}


static std::map<int,std::string> return_map() {
    std::map<int,std::string> result;
    result.insert(std::make_pair(3,"three"));
    result.insert(std::make_pair(5,"five"));
    return result;
}

static bool accept_map_cref(std::map<int,std::string> const & v) {
    std::map<int,std::string>::const_iterator i3 = v.find(3);
    std::map<int,std::string>::const_iterator i5 = v.find(5);
    if (i3 == v.end() || i5 == v.end()) return false;
    return i3->second == "three" && i5->second == "five";
}

static bool accept_map_ref(std::map<int,std::string> & v) {
    std::map<int,std::string>::const_iterator i3 = v.find(3);
    std::map<int,std::string>::const_iterator i5 = v.find(5);
    if (i3 == v.end() || i5 == v.end()) return false;
    return i3->second == "three" && i5->second == "five";
}


typedef boost::fusion::tuple<int,double,std::string> test_tuple;

static test_tuple return_tuple() {
    return boost::fusion::make_tuple(3, 0.0, "test string");
}

static bool accept_tuple(test_tuple const & tuple) {
    return boost::fusion::at_c<0>(tuple) == 3 
        && boost::fusion::at_c<1>(tuple) == 0.0 
        && boost::fusion::at_c<2>(tuple) == "test string";
}


typedef std::pair<int,std::string> test_pair;

static test_pair return_pair() {
    return test_pair(3, "test string");
}

static bool accept_pair(test_pair const & pair) {
    return pair.first == 3 
        && pair.second == "test string";
}


struct shared_ptr_example_class {
    static boost::shared_ptr<shared_ptr_example_class> return_shared_ptr() {
        return boost::shared_ptr<shared_ptr_example_class>(new shared_ptr_example_class());
    }
    static boost::shared_ptr<shared_ptr_example_class const> return_const_shared_ptr() {
        return boost::shared_ptr<shared_ptr_example_class const>(new shared_ptr_example_class());
    }
};

BOOST_PYTHON_MODULE(test_mod) {
    bp::class_< std::vector<int> >("vector")
        .def(bp::vector_indexing_suite< std::vector<int> >())
        ;
    bp::container_from_python_sequence< std::vector<int> >();

    bp::def("return_vector_as_list", &return_vector, bp::return_value_policy<bp::copy_to_list>());
    bp::def("return_vector_as_tuple", &return_vector, bp::return_value_policy<bp::copy_to_tuple>());
    bp::def("return_vector_wrapped", &return_vector);

    bp::def("accept_vector_cref", &accept_vector_cref);
    bp::def("accept_vector_ref", &accept_vector_ref);


    bp::class_< std::map<int,std::string> >("map")
        .def(bp::map_indexing_suite< std::map<int,std::string> >())
        ;
    bp::container_from_python_mapping< std::map<int,std::string> >();

    bp::def("return_map_as_dict", &return_map, bp::return_value_policy<bp::copy_to_dict>());
    bp::def("return_map_wrapped", &return_map);

    bp::def("accept_map_cref", &accept_map_cref);
    bp::def("accept_map_ref", &accept_map_ref);


    bp::boost_fusion_to_python<test_tuple>();
    bp::boost_fusion_from_python<test_tuple>();

    bp::def("return_tuple", &return_tuple);
    bp::def("accept_tuple", &accept_tuple);


    bp::std_pair_to_python<int,std::string>();
    bp::std_pair_from_python<int,std::string>();

    bp::def("return_pair", &return_pair);
    bp::def("accept_pair", &accept_pair);


    bp::class_<shared_ptr_example_class>("shared_ptr_example_class")
        .def("return_shared_ptr", &shared_ptr_example_class::return_shared_ptr)
        .staticmethod("return_shared_ptr")
        .def("return_const_shared_ptr", &shared_ptr_example_class::return_const_shared_ptr)
        .staticmethod("return_const_shared_ptr")
        ;
    bp::register_ptr_to_python< boost::shared_ptr<shared_ptr_example_class> >();
    bp::const_cast_shared_ptr_to_python< shared_ptr_example_class >();
}


#include <boost/field_map/field_map.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/facilities/expand.hpp>


using namespace boost;

using namespace boost::field_map;

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, 20, <libs/guigl/test/test_field_map_compilation.hpp>))
    #include BOOST_PP_ITERATE()

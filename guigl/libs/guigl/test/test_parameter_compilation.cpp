#include <boost/parameter/keyword.hpp>
#include <boost/parameter/name.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/make_vector.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/cat.hpp>

using namespace boost;

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, 50, <libs/guigl/test/test_parameter_compilation.hpp>))
    #include BOOST_PP_ITERATE()

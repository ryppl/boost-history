#include <boost/parameter/keyword.hpp>
#include <boost/parameter/name.hpp>
#include <boost/parameter/preprocessor.hpp>

#define BOOST_TEST_MAIN
#include <boost/test/unit_test.hpp>

#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/cat.hpp>

using namespace boost;

#define BOOST_PP_ITERATION_PARAMS_1 (3, (0, 20, <libs/guigl/test/test_parameter_dispatch_compilation.hpp>))
    #include BOOST_PP_ITERATE()


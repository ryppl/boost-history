#if !defined(BOOST_PP_IS_ITERATING)
# error Signal Network - do not include this file!
#endif

#define _inputs 2
#define _inputs_inc 3

#include <boost/signal_network/detail/template_loader.hpp>

#undef _inputs_inc
#undef _inputs

#define _inputs 3
#define _inputs_inc 4

#include <boost/signal_network/detail/template_loader.hpp>

#undef _inputs_inc
#undef _inputs

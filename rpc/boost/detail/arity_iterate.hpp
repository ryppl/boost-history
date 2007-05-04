#ifdef BOOST_ARITY_ITERATION_PARAMS
#define BOOST_PP_ITERATION_PARAMS_1 BOOST_ARITY_ITERATION_PARAMS
#endif

#ifndef BOOST_ARITY_SEPARATE_VOID_RETURN

#include <boost/detail/arity_prologue.hpp>
#include BOOST_PP_ITERATE()
#include <boost/detail/arity_epilogue.hpp>

#else // BOOST_ARITY_SEPARATE_VOID_RETURN

#include <boost/detail/arity_prologue.hpp>

#define BOOST_ARITY_ENABLE_DISABLE_VOID \
   typename boost::enable_if<boost::is_void< \
     typename boost::function_traits<Signature>::result_type>, void >::type
#define BOOST_ARITY_VOID_RETURN_TYPE
#include BOOST_PP_ITERATE()
#undef BOOST_ARITY_ENABLE_DISABLE_VOID
#undef BOOST_ARITY_VOID_RETURN_TYPE

#undef BOOST_PP_ITERATION_PARAMS_1
#define BOOST_PP_ITERATION_PARAMS_1 BOOST_ARITY_ITERATION_PARAMS

#define BOOST_ARITY_ENABLE_DISABLE_VOID \
   typename boost::disable_if<boost::is_void< \
        typename boost::function_traits<Signature>::result_type>, void >::type
#define BOOST_ARITY_NON_VOID_RETURN_TYPE
#include BOOST_PP_ITERATE()
#undef BOOST_ARITY_ENABLE_DISABLE_VOID
#undef BOOST_ARITY_NON_VOID_RETURN_TYPE

#include <boost/detail/arity_epilogue.hpp>

#endif // BOOST_ARITY_SEPARATE_VOID_RETURN
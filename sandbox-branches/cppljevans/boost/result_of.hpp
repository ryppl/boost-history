#ifndef BOOST_RESULT_OF_HPP
#define BOOST_RESULT_OF_HPP

#include <boost/type_traits/ice.hpp>
#include <boost/type.hpp>
#include <boost/preprocessor.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/mpl/aux_/has_xxx.hpp>

#ifndef BOOST_RESULT_OF_NUM_ARGS
#  define BOOST_RESULT_OF_NUM_ARGS 10
#endif

namespace boost {

template<typename F> struct result_of;

namespace detail {

BOOST_MPL_HAS_XXX_TRAIT_DEF(result_type)
    
template<typename F, typename FArgs, bool HasResultType> struct get_result_of;

template<typename F, typename FArgs>
struct get_result_of<F, FArgs, true>
{
  typedef typename F::result_type type;
};

template<typename F, typename FArgs>
struct get_result_of<F, FArgs, false>
{
  typedef typename F::template result<FArgs>::type type;
};

template<typename F>
struct get_result_of<F, F(void), false>
{
  typedef void type;
};

template<typename F, typename FArgs>
struct result_of : get_result_of<F, FArgs, (has_result_type<F>::value)> {};

} // end namespace detail

#define BOOST_PP_ITERATION_PARAMS_1 (3,(0,BOOST_RESULT_OF_NUM_ARGS,<boost/detail/result_of_iterate.hpp>))
#include BOOST_PP_ITERATE()

}
#endif // BOOST_RESULT_OF_HPP

#ifndef BOOST_RESULT_OF_HPP
#define BOOST_RESULT_OF_HPP

#include <boost/type_traits/ice.hpp>
#include <boost/type.hpp>
#include <boost/preprocessor.hpp>

#ifndef BOOST_RESULT_OF_NUM_ARHS
#  define BOOST_RESULT_OF_NUM_ARGS 10
#endif

namespace boost {

namespace detail {

template<typename T>
type_traits::yes_type has_result_type_helper(type<typename T::result_type>*);

template<typename T> type_traits::no_type has_result_type_helper(...);

template<typename T>
struct has_result_type
{
  BOOST_STATIC_CONSTANT(bool,
                        value = (sizeof(has_result_type_helper<T>(0))
                                   == sizeof(type_traits::yes_type)));
};

template<typename F, typename FArgs, bool HasResultType> struct get_result_of;

template<typename F, typename FArgs>
struct get_result_of<F, FArgs, true>
{
  typedef typename F::result_type type;
};

template<typename F, typename FArgs>
struct get_result_of<F, FArgs, false>
{
  typedef typename F::template result_of<FArgs>::type type;
};

template<typename F, typename FArgs>
struct result_of : get_result_of<F, FArgs, (has_result_type<F>::value)> {};

}

template<typename F> struct result_of;

#define BOOST_PP_ITERATION_PARAMS_1 (3,(0,BOOST_RESULT_OF_NUM_ARGS,<boost/detail/result_of_iterate.hpp>))
#include BOOST_PP_ITERATE()

}
#endif // BOOST_RESULT_OF_HPP

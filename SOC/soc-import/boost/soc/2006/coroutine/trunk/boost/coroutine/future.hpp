#ifndef BOOST_COROUTINE_FUTURE_HPP_20060728
#define BOOST_COROUTINE_FUTURE_HPP_20060728
/*
 * Max number of futures that can be waited for.
 */
#ifndef  BOOST_COROUTINE_WAIT_MAX
#define BOOST_COROUTINE_WAIT_MAX 10
#endif

#include <boost/preprocessor/repetition.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/coroutine/detail/arg_max.hpp>
#include <boost/coroutine/detail/future_impl.hpp>
#include <boost/coroutine/detail/call_impl.hpp>
#include <boost/coroutine/detail/wait_impl.hpp>

namespace boost { namespace coroutines {

#define BOOST_COROUTINE_future_params(z, n, unused) \
  typename BOOST_PP_CAT(T, n) = void                \
  /**/
  
  template<BOOST_PP_ENUM
  (BOOST_COROUTINE_ARG_MAX, 
   BOOST_COROUTINE_future_params,
   ~)>
  class future;

#define BOOST_COROUTINE_gen_future(z, n, unused)   \
  template<BOOST_PP_ENUM_PARAMS(n, typename T)>    \
  class future<BOOST_PP_ENUM_PARAMS(n, T)> :       \
    public detail::future_impl<                    \
    BOOST_PP_CAT(boost::mpl::vector, n)            \
    <BOOST_PP_ENUM_PARAMS(n, T)>                   \
   > {                                             \
    typedef detail::future_impl<                   \
      BOOST_PP_CAT(boost::mpl::vector, n)          \
      <BOOST_PP_ENUM_PARAMS(n, T)>                 \
      > super_type;                                \
    typedef void (future::*safe_bool)();           \
    void safe_bool_true() {}                       \
    public:                                        \
    typedef BOOST_PP_IF(n, BOOST_PP_IDENTITY(BOOST_DEDUCED_TYPENAME), BOOST_PP_EMPTY)()    \
      super_type::value_type value_type;           \
    future& operator=(const value_type&rhs){       \
      super_type::operator=(rhs);                  \
      return *this;                                \
    }                                              \
    operator safe_bool() const {                   \
      return this->is_valid()?                     \
       &future::safe_bool_true: 0;                 \
    }                                              \
  };                                               \
/**/
  
  BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX,
		BOOST_COROUTINE_gen_future,
		~);

#undef BOOST_COROUTINE_gen_future
#undef BOOST_COROUTINE_future_params

#define BOOST_COROUTINE_gen_call_overload(z, n, unused) \
  template<                                             \
    BOOST_PP_ENUM_PARAMS(n, typename T)                      \
    BOOST_PP_COMMA_IF(n)                                \
    typename Functor,                                   \
    typename Coroutine>                                 \
  future<BOOST_PP_ENUM_PARAMS(n, T)>                         \
  call(const Functor f, const Coroutine& coro) {        \
    return detail::call_impl<future<BOOST_PP_ENUM_PARAMS (n,T)> >(f, coro);  \
  }                                                     \
/**/

  /*
   * Generate overloads of call<...>(function, coroutine) for
   * an arbitrary argument numbers that will forward to 
   * detail::call_impl<future<...> >(function, coroutine)
   */
  BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX,
		  BOOST_COROUTINE_gen_call_overload,
		  ~);
    
#define BOOST_COROUTINE_empty(z, n, name) \
  /**/

#define BOOST_COROUTINE_gen_wait_non_zero(z, n, name)\
  template<typename Coroutine,                       \
	   BOOST_PP_ENUM_PARAMS(n, typename T)>      \
  void name (Coroutine& coro, BOOST_PP_ENUM_BINARY_PARAMS(n, T, arg)) {  \
    detail::BOOST_PP_CAT(name, _impl)                \
      (coro, boost::make_tuple                       \
       (BOOST_PP_ENUM_PARAMS(n, arg)));              \
  }                                                  \
  /**/

#define BOOST_COROUTINE_gen_wait(z, n, name)     \
  BOOST_PP_IF(n,                                 \
	      BOOST_COROUTINE_gen_wait_non_zero, \
	      BOOST_COROUTINE_empty)(z, n, name) \
/**/

  /*
   * Generate overloads of wait(coro, ...) for
   * an arbitrary argument number that will
   * forward to detail::wait_impl(coro, tuple<...>)
   */
  BOOST_PP_REPEAT(BOOST_COROUTINE_WAIT_MAX,
		  BOOST_COROUTINE_gen_wait,
		  wait);

  /*
   * Generate wait_all(coro, ...) for an arbitrary arguement
   * number that will forward to
   * detail::wait_all_impl(coro, tuple<...>)
   */
  BOOST_PP_REPEAT(BOOST_COROUTINE_WAIT_MAX,
		  BOOST_COROUTINE_gen_wait,
		  wait_all);

#undef BOOST_COROUTINE_gen_wait
#undef BOOST_COROUTINE_empty
#undef BOOST_COROUTINE_gen_wait_non_zero

  template<typename Future, typename Coroutine>
  detail::callback<Future, Coroutine>
  make_callback(const Future& future, const Coroutine& coroutine) {
    return detail::callback<Future, Coroutine>(future, coroutine);
  }

} } 
#endif


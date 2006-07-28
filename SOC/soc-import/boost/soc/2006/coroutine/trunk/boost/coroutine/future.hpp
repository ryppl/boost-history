#ifndef BOOST_COROUTINE_FUTURE_HPP_20060728
#define BOOST_COROUTINE_FUTURE_HPP_20060728
#include <boost/coroutine/detail/future_impl.hpp>
#include <boost/coroutine/detail/do_call.hpp>
#include <boost/preprocessor/repetition.hpp>
namespace boost { namespace coroutines {

#define BOOST_COROUTINE_future_params(z, n, unused) \
  typename BOOST_PP_CAT(T, n) = void                \
  /**/
  
  template<BOOST_PP_ENUM_PARAMS
  (BOOST_COROUTINE_ARG_MAX, 
   BOOST_COROUTINE_fugure_params,
   ~)>
  class future;


#define BOOST_COROUTINE_gen_future(z, n, unused)   \
  template<BOOST_PP_PARAMS(n, typename T)>         \
  class future<BOOST_PP_PARAMS(n, T)> :            \
    public detail::future_impl<                    \
    BOOST_PP_CAT(boost::mpl::vector, n)            \
    <BOOST_PP_PARAMS(n, T)>                        \
   > {                                             \
    typedef fugure_impl<                           \
      BOOST_PP_CAT(boost::mpl::vector, n)          \
      <BOOST_PP_PARAMS(n, T)>                      \
      > super_type;                                \
    using BOOST_DEDUCED_TYPENAME                   \
      super_type::value_type;			   \
    public:                                        \
    future& operator=(const value_type&rhs)        \
      super_type::operator=(rhs);                  \
      return *this                                 \
    }                                              \
    };                                             \
/**/
  
  BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX,
		BOOST_COROUTINE_gen_future,
		~);

#undef BOOST_COROUTINE_gen_future
#undef BOOST_COROUTINE_future_params

#define BOOST_COROUTINE_gen_call_overload(z, n, unused) \
  template<                                             \
    BOOST_PP_PARAMS(n, typename T)                      \
    BOOST_PP_COMMA_IF(n)                                \
    typename Functor,                                   \
    typename Coroutine>                                 \
  future<BOOST_PP_PARAMS(n, T)>                         \
  call(const Functor f, const Coroutine& coro) {        \
    return detail::do_call<future<BOOST_PP_PARAMS (n,T)> >(f, coro);  \
  }                                                     \
/**/

  BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX,
		BOOST_COROUTINE_gen_call_overload,
		~);
    
  

} } 
#endif

#ifndef BOOST_COROUTINE_FUTURE_HPP_20060728
#define BOOST_COROUTINE_FUTURE_HPP_20060728
/*
 * Max number of futures that can be waited for.
 */
#ifndef  BOOST_COROUTINE_WAIT_MAX
#define BOOST_COROUTINE_WAIT_MAX 10
#endif

#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/config.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/none.hpp>
#include <boost/coroutine/detail/arg_max.hpp>
#include <boost/coroutine/detail/call_impl.hpp>
#include <boost/coroutine/detail/wait_impl.hpp>
#include <boost/coroutine/tuple_traits.hpp>
#include <boost/coroutine/detail/signal.hpp>

namespace boost { namespace coroutines {

#if 0
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
    public:                                        \
    future() {}                                    \
    future(none_t) : future_impl(none_t) {}        \
    template<typename T>                           \
    future(const T& val) :                         \
      future_impl (val) {}                         \
    future(const future& rhs) :
       
    typedef BOOST_PP_IF                            \
      (n, BOOST_PP_IDENTITY                        \
       (BOOST_DEDUCED_TYPENAME),                   \
       BOOST_PP_EMPTY)()                           \
      super_type::value_type value_type;           \
    future& operator=(const value_type&rhs){       \
      super_type::operator=(rhs);                  \
      return *this;                                \
    }                                              \
    future& operator=(none_t){                     \
      super_type::operator=(none_t);               \
      return *this;                                \
    }                                              \
  };                                               \
/**/
  
  BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX,
		BOOST_COROUTINE_gen_future,
		~);
#endif

  template<
    BOOST_PP_ENUM_BINARY_PARAMS
  (BOOST_COROUTINE_ARG_MAX,
   typename T, 
   = boost::tuples::null_type BOOST_PP_INTERCEPT)>
  class future {
    friend class detail::wait_gateway;
    typedef void (future::*safe_bool)();           
    void safe_bool_true() {}                       
  public:
    typedef tuple_traits<
      BOOST_PP_ENUM_PARAMS
    (BOOST_COROUTINE_ARG_MAX, T)
      > tuple_traits_type;

    typedef boost::mpl::bool_<tuple_traits_type::length != 1>
    is_singular;

    typedef BOOST_DEDUCED_TYPENAME tuple_traits_type::as_tuple tuple_type;
    typedef BOOST_DEDUCED_TYPENAME boost::mpl::eval_if<
      is_singular,
      boost::mpl::identity<tuple_type>,
      BOOST_DEDUCED_TYPENAME tuple_traits_type::template at<0>
    >::type  value_type;
    
  private:
    template<typename T>
    struct opt_impl   {
      opt_impl() : waited(false)  {}
      bool waited;
      boost::optional<T> optional;
    };

    typedef boost::shared_ptr<opt_impl<tuple_type> > 
    optional_pointer;

  public:
    future() :
      m_ptr(new opt_impl<tuple_type>()) {}

    future(const future& rhs) :
      m_ptr(rhs.m_ptr) {}

    future(none_t) :
      m_ptr(new opt_impl<tuple_type>()) {}

    value_type& operator *() {
      BOOST_ASSERT(m_ptr);
      BOOST_ASSERT(m_ptr->optional);
      return remove_tuple(*m_ptr->optional,  boost::mpl::not_<is_singular>());
    }

    const value_type& operator *() const{
      BOOST_ASSERT(m_ptr);
      BOOST_ASSERT(m_ptr->optional);
      return remove_tuple(*m_ptr->optional, boost::mpl::not_<is_singular>());
    }

    future&
    operator=(const value_type& rhs) {
      m_ptr->optional = tuple_type(rhs);
      return *this;
    }

    future&
    operator=(none_t) {
      m_ptr->optional = (none);
      return *this;
    }

    operator safe_bool() const {    
      BOOST_ASSERT(m_ptr);
      return m_ptr->optional?                     
       &future::safe_bool_true: 0;                 
    }                                              
  private:

    template<typename T>
    value_type& remove_tuple(T& x, boost::mpl::true_) {
      return boost::get<0>(x);
    }

    template<typename T>
    value_type& remove_tuple(T& x, boost::mpl::false_) {
      return x;
    }

    void mark_wait(bool how) {
      m_ptr->waited = how;
    }

    bool waited() const {
      return m_ptr->waited;
    }

    void assign(const tuple_type& val) {
      m_ptr->optional =  val;
    }

    optional_pointer m_ptr;    
  };



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


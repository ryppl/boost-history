//  (C) Copyright Giovanni P. Deretta 2006. Distributed under the Boost
//  Software License, Version 1.0. (See accompanying file
//  LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
#ifndef BOOST_COROUTINE_FUTURE_HPP_20060728
#define BOOST_COROUTINE_FUTURE_HPP_20060728

// Max number of futures that can be waited for.
#ifndef BOOST_COROUTINE_WAIT_MAX
#define BOOST_COROUTINE_WAIT_MAX 10
#endif
#include <boost/none.hpp>
#include <boost/config.hpp>
#include <boost/optional.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/mpl/eval_if.hpp>
#include <boost/mpl/identity.hpp>
#include <boost/preprocessor/repetition.hpp>
#include <boost/preprocessor/facilities/intercept.hpp>

#include <boost/coroutine/move.hpp>
#include <boost/coroutine/tuple_traits.hpp>
#include <boost/coroutine/detail/signal.hpp>
#include <boost/coroutine/detail/arg_max.hpp>
#include <boost/coroutine/detail/call_impl.hpp>
#include <boost/coroutine/detail/wait_impl.hpp>
#include <boost/coroutine/detail/future_impl.hpp>

namespace boost { namespace coroutines {

  template<
    BOOST_PP_ENUM_BINARY_PARAMS
  (BOOST_COROUTINE_ARG_MAX,
   typename T, 
   = boost::tuples::null_type BOOST_PP_INTERCEPT)>
  class future : 
    public movable
    <future<BOOST_PP_ENUM_PARAMS(BOOST_COROUTINE_ARG_MAX, T)> > 
  {
 
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
    
    typedef boost::optional<tuple_type> optional_type;

  private:
    typedef detail::future_impl<tuple_type> opt_impl;
    typedef opt_impl * optional_pointer;

  public:
    future() :
      m_ptr(new opt_impl()) {}

    future(move_from<future> rhs) :
      m_ptr(rhs->pilfer()) {
    }

    future(none_t) :
      m_ptr(new opt_impl()) {}

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
    operator=(move_from<future> rhs) {
      future(rhs).swap(*this);
      return *this;
    }

    future&
    operator=(const value_type& rhs) {
      BOOST_ASSERT(!pending());
      m_ptr->optional = tuple_type(rhs);
      return *this;
    }

    future&
    operator=(none_t) {
      BOOST_ASSERT(!pending());
      m_ptr->optional = (none);
      return *this;
    }

    operator safe_bool() const {    
      BOOST_ASSERT(m_ptr);
      return m_ptr->optional?                     
       &future::safe_bool_true: 0;                 
    }       

    optional_type & operator ->() {
      BOOST_ASSERT(m_ptr);
      return m_ptr->optional;
    }

    const optional_type & operator ->() const {
      BOOST_ASSERT(m_ptr);
      return m_ptr->optional;
    }

    friend 
    void swap(future& lhs, future& rhs) {
      std::swap(lhs.m_ptr, rhs.m_ptr);
    }

    // Destroying a pending future is an error.
    ~future() {
      if(m_ptr) {
	BOOST_ASSERT(!pending());
      	delete m_ptr;
      }
    }

    /*
     * Return true if an async call has
     * been scheduled for this future.
     */
    bool pending() const {
      BOOST_ASSERT(m_ptr);
      return m_ptr->pending;
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
      BOOST_ASSERT(m_ptr);
      m_ptr->waited = how;
    }

    bool waited() const {
      BOOST_ASSERT(m_ptr);
      return m_ptr->waited;
    }

    optional_pointer pilfer() {
      optional_pointer ptr = m_ptr;
      m_ptr = 0;
      return ptr;
    }

    optional_pointer m_ptr;    
  };

#define BOOST_COROUTINE_gen_call_overload(z, n, unused) \
  template<                                             \
    BOOST_PP_ENUM_PARAMS(n, typename T)                 \
    BOOST_PP_COMMA_IF(n)                                \
    typename Functor,                                   \
    typename Coroutine>                                 \
  future<BOOST_PP_ENUM_PARAMS(n, T)>                    \
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

#define BOOST_COROUTINE_gen_reference(z, n, unused) \
    BOOST_PP_CAT(T, n) &                            \
/**/

#define BOOST_COROUTINE_gen_wait_non_zero(z, n, name)\
  template<typename Coroutine,                       \
	   BOOST_PP_ENUM_PARAMS(n, typename T)>      \
  void name (Coroutine& coro, BOOST_PP_ENUM_BINARY_PARAMS(n, T, &arg)) {  \
    detail::BOOST_PP_CAT(name, _impl)                \
      (coro, boost::tuple<BOOST_PP_ENUM(n, BOOST_COROUTINE_gen_reference, ~)> \
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
#undef BOOST_COROUTINE_gen_reference
#undef BOOST_COROUTINE_gen_wait_non_zero
#undef BOOST_COROUTINE_gen_call_overload

  template<typename Future >
  struct make_callback_result {
    typedef detail::callback<Future, coroutine<void()>::self> type;
  };

  /*
   * Returns a callback object that when invoked
   * will signal the associated coroutine::self object.
   * It will extend the lifetime of the object until
   * it is signaled. More than one callback object
   * can be pending at any time. The coroutine self
   * will last at least untill the last pending callback 
   * is fired.
   */
  template<typename Future>
  BOOST_DEDUCED_TYPENAME 
  make_callback_result<Future>
  ::type
  make_callback(Future& future, coroutine<void()>::self& coroutine_self) {
    return BOOST_DEDUCED_TYPENAME make_callback_result<Future>::type
      (future, coroutine_self);
  }

  /*
   * A coroutine cannot directly posted in a demuxer, as it is
   * not movable. Use this function to workaround the lack
   * of movability
   */
  template<typename Demuxer>
  void post(move_from<coroutine<void()> > coro, Demuxer& demux) {
    demux.post(detail::asio_adapter(coro));    
  }
} } 
#endif


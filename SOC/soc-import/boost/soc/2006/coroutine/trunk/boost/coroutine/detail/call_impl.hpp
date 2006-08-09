#ifndef BOOST_COROUTINE_DETAIL_CALL_IMPL_HPP_20060728
#define BOOST_COROUTINE_DETAIL_CALL_IMPL_HPP_20060728

#include <boost/preprocessor/repetition.hpp>
#include <boost/call_traits.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/bind.hpp>
#include <boost/coroutine/detail/arg_max.hpp>
#include <boost/coroutine/detail/signal.hpp>
#include <boost/coroutine/detail/coroutine_accessor.hpp>
namespace boost { namespace coroutines { namespace detail {

#define BOOST_COROUTINE_tuple_param_n(z, n, tuple)\
  BOOST_DEDUCED_TYPENAME                          \
  boost::tuples::element<n, tuple>::type          \
  BOOST_PP_CAT(arg, n)                            \
  /**/

  template<typename Future, typename Coroutine>                       
  class callback {
  public:
    typedef void result_type;
    
    callback(Future& future, 
	     Coroutine& coroutine) :
      m_future(future),
      m_coroutine(coroutine){
      coroutine_accessor::acquire(m_coroutine);
    }

    callback(const callback& rhs) :
      m_future(rhs.m_future),
      m_coroutine(rhs.m_coroutine) {
      coroutine_accessor::acquire(m_coroutine);
    }

    ~callback() {
      coroutine_accessor::release(m_coroutine);
    }
    
    typedef BOOST_DEDUCED_TYPENAME                
    Future::tuple_type tuple_type;              

    typedef BOOST_DEDUCED_TYPENAME                
    Future::tuple_traits_type tuple_traits_type;              


#define BOOST_COROUTINE_gen_argn_type(z, n, unused) \
    typedef BOOST_DEDUCED_TYPENAME                  \
    tuple_traits_type::                             \
    template at<n>::type                            \
    BOOST_PP_CAT(BOOST_PP_CAT(arg, n), _type);      \
    /**/

    BOOST_PP_REPEAT(BOOST_COROUTINE_ARG_MAX,
		    BOOST_COROUTINE_gen_argn_type,
		    ~);
      
#define BOOST_COROUTINE_param_with_default(z, n, type_prefix) \
    BOOST_DEDUCED_TYPENAME call_traits                   \
    <BOOST_PP_CAT(BOOST_PP_CAT(type_prefix, n), _type)>  \
    ::param_type                                         \
    BOOST_PP_CAT(arg, n) =                               \
    BOOST_PP_CAT(BOOST_PP_CAT(type_prefix, n), _type)()  \
/**/
    
    void operator() 
      (BOOST_PP_ENUM
       (BOOST_COROUTINE_ARG_MAX,
	BOOST_COROUTINE_param_with_default,
	arg)) {
      wait_gateway::assign(m_future,tuple_type
	(BOOST_PP_ENUM_PARAMS
	 (BOOST_COROUTINE_ARG_MAX, arg)));
      if(wait_gateway::signal(m_coroutine))
	wait_gateway::wake_up(m_coroutine);
    }

  private:
    //operator= would be disabled anyway because references are not 
    //rebindable.
    void operator=(const callback&);
    Future& m_future;
    Coroutine& m_coroutine;
  };  

#undef BOOST_COROUTINE_gen_future_assigner
#undef BOOST_COROUTINE_tuple_param_n

  template<typename Future, typename Functor, typename Coroutine>
  Future call_impl(Functor fun, const Coroutine& coro) {
    Future future;
    fun(callback<Future, Coroutine>(future, coro));
    return future;
  }

} } }

#endif

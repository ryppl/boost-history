#ifndef BOOST_COROUTINE_DETAIL_WAIT_IMPL_HPP_20060728
#define BOOST_COROUTINE_DETAIL_WAIT_IMPL_HPP_20060728
#include <boost/tuple/tuple.hpp>
#include <boost/coroutine/detail/signal.hpp>
namespace boost { namespace coroutines { namespace detail {

  /*
   * Inokes mark_wait('how') on the 'Idx'th element of 't'
   * then calls marker::wait<Idx-1> with the same arguments.
   * returns the result of marker::wait<Idx-1>(...) plus 1 if
   * the 'Idx'th is true or plus 0 otherwise.
   * 
   */
  template<int Idx>
  struct marker {
    template<typename Tuple>
    static int mark(Tuple& t, bool how) {
      wait_gateway::mark_wait(t.template get<Idx-1>(), how);
      return marker<Idx -1>::mark(t, how) + 
	(t.template get<Idx-1>()? 1:0);
    }
  };

  /* Recursion terminator */
  template<>
  struct marker<0> {
    template<typename Tuple>
    static int mark(Tuple&, bool) { return 0;}
  };

  /*
   * Wait for at least one future in 'wt' tuple to be signaled.
   * If at least one future has been already signaled does not
   * block but returns immediatley (prevents deadlocks).
   */
  template<typename Coroutine, typename Tuple>
  void wait_impl(Coroutine& coro, Tuple wt) {
    if(marker<boost::tuples::length<Tuple>::value>::mark(wt, true) == 0)
      coroutine_accessor::get_impl(coro)->wait(1);
    int res = marker<boost::tuples::length<Tuple>::value>::mark(wt, false);
    BOOST_ASSERT(res > 0);
    (void)res;
  }

  /*
   * Wait for all futures in 'wt' tuple to be signaled.
   */
  template<typename Coroutine, typename Tuple>
  void wait_all_impl(Coroutine& coro, Tuple wt) {
    int res = marker<boost::tuples::length<Tuple>::value>::mark(wt, true);
    coroutine_accessor::get_impl(coro)->wait(boost::tuples::length<Tuple>::value - res);
    res = marker<boost::tuples::length<Tuple>::value>::mark(wt, false);
    BOOST_ASSERT(res == boost::tuples::length<Tuple>::value);
  }

} } }
#endif

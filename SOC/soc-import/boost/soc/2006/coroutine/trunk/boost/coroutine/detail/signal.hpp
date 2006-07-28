#ifndef BOOST_COROUTINE_DETAIL_SIGNAL_HPP_20060728
#define BOOST_COROUTINE_DETAIL_SIGNAL_HPP_20060728
namespace boost { namespace coroutines { namespace detail {
  /*
   * Private interface for coroutine signaling/waiting.
   * These functions are friend of coroutine and are
   * invoked by asynchronous callback functions.
   */

  

    template<typename Coroutine>
    void signal(Coroutine& c) {
      c.signal();
    }

    template<typename Coroutine>
    void wait(Coroutine& c, int n) {
      c.wait(n);
    }

} } }
#endif

#ifndef BOOST_COROUTINE_DETAIL_SIGNAL_HPP_20060728
#define BOOST_COROUTINE_DETAIL_SIGNAL_HPP_20060728
namespace boost { namespace coroutines { namespace detail {
  /*
   * Private interface for coroutine signaling/waiting.
   * These class is friend of coroutine and future_impl and
   * its stati members are invoked by asynchronous callback functions.
   */
  struct wait_gateway {
    template<typename Coroutine>
    static
    bool signal(Coroutine& c) {
      return c.signal();
    }

    template<typename Coroutine>
    static
    void wait(Coroutine& c, int n) {
      c.wait(n);
    }

    template<typename Future>
    static 
    bool waited(const Future& f) {
      return f.waited();
    }

    template<typename Future>
    static 
    void mark_wait(Future& f, bool how) {
      f.mark_wait(how);
    }
  };


} } }
#endif

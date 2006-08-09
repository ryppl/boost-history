#ifndef BOOST_COROUTINE_DETAIL_SIGNAL_HPP_20060728
#define BOOST_COROUTINE_DETAIL_SIGNAL_HPP_20060728
namespace boost { namespace coroutines { namespace detail {
  /*
   * Private interface for coroutine signaling/waiting.
   * These class is friend of future_impl and
   * its static members are invoked by asynchronous callback functions.
   */
  struct wait_gateway {
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

    template<typename Future, typename Tuple>
    static
    void assign(Future& future, const Tuple& tuple) {
      future.assign(tuple);
    }
  };


} } }
#endif

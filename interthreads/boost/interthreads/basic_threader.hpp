#ifndef BOOST_INTERTHREADS_BASIC_THREADER__HPP
#define BOOST_INTERTHREADS_BASIC_THREADER__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the threader/joiner design from of Kevlin Henney (n1883)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/thread/detail/move.hpp>
#include <boost/thread/thread.hpp>

#include <boost/interthreads/algorithm.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {


class basic_threader {
#ifdef BOOST_THREAD_HAS_THREAD_ATTR
private:
    thread::native_handle_attr_type attr_;
public:
    thread::native_handle_attr_type& attr() {
        return attr_;
    }
#endif

public:
    template <typename T>
    struct handle {
        typedef thread type;
    };

    template <typename F>
    thread fork(F f) {
#ifdef BOOST_THREAD_HAS_THREAD_ATTR
        thread th(attr(), f);
#else
        thread th(f);
#endif
        return boost::move(th);
    }
};

    template<>
    struct act_traits<thread >  {
            typedef void move_dest_type;
    };

    template <>
    struct is_movable<thread > : mpl::true_{};

    template <>
    struct has_future_if<thread > : mpl::true_{};

    template <>
    struct has_thread_if<thread > : mpl::true_{};

    namespace partial_specialization_workaround {
        #if 0
        template<typename F >
        struct fork<basic_threader, F> {
            static boost::detail::thread_move_t<typename result_of::fork<AE,F>::type> apply(basic_threader& ae, F fn ) {
                return ae.fork(fn);
            }
        };
        #endif
        template <>
        struct wait<thread> {
            static result_of::wait<thread>::type apply( thread& act) {
                return act.join();
            }
        };
        template<>
        struct wait<boost::detail::thread_move_t<thread> > {
            static result_of::wait<thread>::type apply( boost::detail::thread_move_t<thread> act ) {
                return act->join();
            }
        };

        template <>
        struct wait_until<thread> {
            static result_of::wait_until<thread>::type apply( thread& act, const system_time& abs_time ) {
                return act.timed_join(abs_time);
            }
        };
        template <>
        struct wait_until<boost::detail::thread_move_t<thread> > {
            static result_of::wait_until<thread>::type apply( boost::detail::thread_move_t<thread> act, const system_time& abs_time ) {
                return act->timed_join(abs_time);
            }
        };
        template <typename Duration>
        struct wait_for<thread, Duration> {
            static typename result_of::template wait_for<thread,Duration>::type apply( thread& act, Duration abs_time ) {
                return act.timed_join(abs_time);
            }
        };

        template <typename Duration>
        struct wait_for<boost::detail::thread_move_t<thread>, Duration> {
            static typename result_of::template wait_for<thread,Duration>::type apply( boost::detail::thread_move_t<thread> act, Duration abs_time ) {
                return act->timed_join(abs_time);
            }
        };

        template <>
        struct join_until<thread> {
            static result_of::join_until<thread>::type apply( thread& act, const system_time& abs_time ) {
                return act.timed_join(abs_time);
            }
        };
        template <>
        struct join_until<boost::detail::thread_move_t<thread> >{
            static result_of::join_until<thread>::type apply( boost::detail::thread_move_t<thread> act, const system_time& abs_time ) {
                return act->timed_join(abs_time);
            }
        };
        template <typename Duration>
        struct join_for<thread, Duration> {
            static typename result_of::template join_for<thread,Duration>::type apply( thread& act, Duration abs_time ) {
                return act.timed_join(abs_time);
            }
        };
        template <typename Duration>
        struct join_for<boost::detail::thread_move_t<thread>, Duration> {
            static typename result_of::template join_for<thread,Duration>::type apply( boost::detail::thread_move_t<thread> act, Duration abs_time ) {
                return act->timed_join(abs_time);
            }
        };
    }


}
}


#include <boost/config/abi_suffix.hpp>

#endif


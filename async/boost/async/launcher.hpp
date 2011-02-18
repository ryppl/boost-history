#ifndef BOOST_ASYNC_LAUNCHER__HPP
#define BOOST_ASYNC_LAUNCHER__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the threader/joiner design from of Kevlin Henney (n1883)
//
// See http://www.boost.org/libs/async for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/thread/detail/move.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/future.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/async/algorithm.hpp>
#include <boost/async/act_traits.hpp>
#include <boost/async/future_traits.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace async {

class launcher {
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
        typedef unique_future<T> type;
    };

    template <typename F>
    unique_future<typename boost::result_of<F()>::type>
    fork(F f) {
        typedef typename boost::result_of<F()>::type result_type;
        packaged_task<result_type> tsk(f);
        unique_future<result_type> act = tsk.get_future();
#ifdef BOOST_THREAD_HAS_THREAD_ATTR
        thread th(attr(), boost::move(tsk));
#else
        thread th(boost::move(tsk));
#endif
//        return boost::move(act);
        boost::detail::thread_move_t<boost::unique_future<result_type> > m(act);
        return unique_future<result_type>(m);
    }

};


class shared_launcher {
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
        typedef shared_future<T> type;
    };

    template <typename F>
    shared_future<typename boost::result_of<F()>::type>
    fork(F f) {
        typedef typename boost::result_of<F()>::type result_type;
        packaged_task<result_type> tsk(f);
        shared_future<result_type> act(tsk.get_future());
#ifdef BOOST_THREAD_HAS_THREAD_ATTR
        thread th(attr(), boost::move(tsk));
#else
        thread th(boost::move(tsk));
#endif
        return act;
    }

};

template <>
struct get_future<launcher> {
    template <typename T>
    struct future_type {
        typedef unique_future<T> type;
    };
    template <typename T>
    unique_future<T>& operator()(unique_future<T>& f) { return f; }
};

template <>
struct get_future<shared_launcher> {
    template <typename T>
    struct future_type {
        typedef shared_future<T> type;
    };
    template <typename T>
    shared_future<T>& operator()(shared_future<T>& f) { return f; }
};



}
}


#include <boost/config/abi_suffix.hpp>

#endif


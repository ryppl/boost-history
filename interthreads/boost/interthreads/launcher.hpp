#ifndef BOOST_INTERTHREADS_LAUNCHER__HPP
#define BOOST_INTERTHREADS_LAUNCHER__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
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
#include <boost/futures/future.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/interthreads/fork.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {


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
        unique_future<result_type> res = tsk.get_future();
#ifdef BOOST_THREAD_HAS_THREAD_ATTR    
        thread th(attr(), boost::move(tsk));
#else
        thread th(boost::move(tsk));
#endif        
        return boost::move(res);        
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
        shared_future<result_type> res(tsk.get_future());
#ifdef BOOST_THREAD_HAS_THREAD_ATTR    
        thread th(attr(), boost::move(tsk));
#else
        thread th(boost::move(tsk));
#endif        
        return res;
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


template <typename ResultType>
struct act_value<unique_future<ResultType> > {
    typedef ResultType type;
};

template <typename R>
struct is_movable<unique_future<R> > : mpl::true_{};

template <typename R>
struct has_future_if<unique_future<R> > : mpl::true_{};

template <typename R>
struct has_thread_if<unique_future<R> > : mpl::false_{};

    
template <typename ResultType>
struct act_value<shared_future<ResultType> > {
    typedef ResultType type;
};

template <typename R>
struct is_movable<shared_future<R> > : mpl::true_{};

template <typename R>
struct has_future_if<shared_future<R> > : mpl::true_{};

template <typename R>
struct has_thread_if<shared_future<R> > : mpl::false_{};
}
}


#include <boost/config/abi_suffix.hpp>

#endif


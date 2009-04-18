#ifndef BOOST_ASYNC_THREADER__HPP
#define BOOST_ASYNC_THREADER__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the unique_threader/unique_joiner design from of Kevlin Henney (n1883)
//
// See http://www.boost.org/libs/async for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/thread/detail/move.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/tss.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/futures/future.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/async/fork.hpp>
#include <boost/async/launcher.hpp>
#include <cstdlib>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace async {

namespace on_destruction {
    enum type {
        do_terminate,
        do_join,
        do_detach
    };
}
            

template <typename ResultType>
class unique_joiner;

template<typename T>
struct act_traits<unique_joiner<T> > : act_traits<unique_future<T> > {};


template <typename ResultType>
class unique_joiner {
public:
    typedef ResultType result_type;
    typedef typename act_traits<unique_joiner<ResultType> >::move_dest_type move_dest_type;
private:
    struct unique_joiner_data {
        unique_future< result_type > fut_;
        thread th_;
        on_destruction::type on_destruction_;

        unique_joiner_data(on_destruction::type on_destruction_do= on_destruction::do_join) 
            : on_destruction_(on_destruction_do) 
        {}
        ~unique_joiner_data() {
            if (th_.joinable()) {
                if (on_destruction_== on_destruction::do_terminate) {
                    std::terminate();
                } else if (on_destruction_== on_destruction::do_join) {
                    th_.join();
                }
            }
        }

        template <typename Nullary>
#ifdef BOOST_THREAD_HAS_THREAD_ATTR
        unique_joiner_data(thread::native_handle_attr_type& attr, Nullary f, on_destruction::type on_destruction_do)
            : on_destruction_(on_destruction_do) 
        {
            packaged_task<result_type> tsk(f);
            fut_ = tsk.get_future();
            thread th(attr, boost::move(tsk));
            th_ = boost::move(th);
        }
#else
        unique_joiner_data(Nullary f, on_destruction::type on_destruction_do)
            : on_destruction_(on_destruction_do) 
        {
            packaged_task<result_type> tsk(f);
            fut_ = tsk.get_future();
#if 0
            th_ = boost::move(tsk);
#else
            thread th(boost::move(tsk));
            th_ = boost::move(th);
#endif
        }
#endif

    };
    shared_ptr<unique_joiner_data> data_;
    typedef unique_joiner_data this_impl_type;
    typedef unique_joiner this_type;

    unique_joiner(unique_joiner<ResultType>& other);
    unique_joiner& operator=(unique_joiner<ResultType>& other);
//protected:
public:
    friend class unique_threader;

    template <typename Nullary>
    // requires result_of<Nullary>::type  is convertible to ResultType
#ifdef BOOST_THREAD_HAS_THREAD_ATTR
    unique_joiner(thread::native_handle_attr_type& attr, Nullary f, on_destruction::type on_destruction_do=on_destruction::do_join)
    : data_(new this_impl_type(attr, f,on_destruction_do))
#else
    unique_joiner(Nullary f, on_destruction::type on_destruction_do=on_destruction::do_join)
    : data_(new this_impl_type(f,on_destruction_do))
#endif
    {}


public:
    // move support
#ifdef BOOST_HAS_RVALUE_REFS
    unique_joiner(this_type&& other) {
        data_.swap(other.data_);
    }
    unique_joiner& operator=(unique_joiner&& other) {
        data_=other.data_;
        other.data_.reset(new this_impl_type());
        return *this;
    }

    unique_joiner&& move() {
        return static_cast<unique_joiner&&>(*this);
    }
#else
    unique_joiner(boost::detail::thread_move_t<unique_joiner> x) {
        data_=x->data_;
        x->data_.reset(new this_impl_type());
    }

    unique_joiner& operator=(boost::detail::thread_move_t<unique_joiner> x) {
        this_type tmp_(x);
        swap(tmp_);
        return *this;
    }

    operator boost::detail::thread_move_t<unique_joiner>() {
        return move();
    }

    boost::detail::thread_move_t<unique_joiner> move() {
        return boost::detail::thread_move_t<unique_joiner>(*this);
    }

#endif

    void swap(this_type& x) {
        data_.swap(x.data_);
    }

    bool joinable() {
        return data_->th_.joinable();
    }

    void join() {
        data_->th_.join();
    }

    bool join_until(const system_time& abs_time) {
        return data_->th_.timed_join(abs_time);
    }

    template<typename TimeDuration>
    inline bool join_for(TimeDuration const& rel_time)
    {
        return join_until(get_system_time()+rel_time);
    }
    move_dest_type get()  {
        return data_->fut_.get();
    }
    move_dest_type operator()() { return get(); }

    bool is_ready() const {
        return data_->fut_.is_ready();
    }
    bool has_exception() const {
        return data_->fut_.has_exception();
    }
    bool has_value() const {
        return data_->fut_.has_value();
    }

    void wait() {
        data_->fut_.wait();
    }
    bool wait_until(const system_time& abs_time) {
        return data_->fut_.timed_wait_until(abs_time);
    }

    template<typename TimeDuration>
    inline bool wait_for(TimeDuration const& rel_time)
    {
        return wait_until(get_system_time()+rel_time);
    }

    void detach() {
        data_->th_.detach();
    }

    void interrupt() {
        data_->th_.interrupt();
    }

    bool interruption_requested() const {
        return data_->th_.interruption_requested();
    }

    thread::id get_id() const {
        return data_->th_.get_id();
    }

    unique_future<result_type>& get_future() {
        return data_->fut_;
    }
};

template <typename R>
struct is_movable<unique_joiner<R> > : mpl::true_{};

template <typename R>
struct has_future_if<unique_joiner<R> > : mpl::true_{};

template <typename R>
struct has_thread_if<unique_joiner<R> > : mpl::true_{};

class unique_threader {
#ifdef BOOST_THREAD_HAS_THREAD_ATTR
private:
    thread_specific_ptr<thread::native_handle_attr_type> attr_;
public:
    thread::native_handle_attr_type& attr() {
        if(attr_.get() ==0) {
            attr_.reset(new thread::native_handle_attr_type());
        };
        return *attr_.get();
    }
#endif

public:
    template <typename T>
    struct handle {
        typedef unique_joiner<T> type;
    };
    template <typename F>
    unique_joiner<typename boost::result_of<F()>::type>
    fork(F f) {
        typedef typename boost::result_of<F()>::type result_type;
#ifdef BOOST_THREAD_HAS_THREAD_ATTR
        unique_joiner<result_type> unique_joiner_(attr(), f);
#else
        unique_joiner<result_type> unique_joiner_(f);
#endif
        return boost::move(unique_joiner_);
    }

};

template <>
struct get_future<unique_threader> {
    template <typename T>
    struct future_type {
        typedef unique_future<T> type;
    };
    template <typename T>
    unique_future<T>& operator()(unique_joiner<T>& j) { return j.get_future(); }
};

template <typename ResultType>
class shared_joiner;

template<typename T>
struct act_traits<shared_joiner<T> > : act_traits<shared_future<T> > {};

template <typename ResultType>
class shared_joiner {
public:
    typedef ResultType result_type;
    typedef typename act_traits<shared_joiner<ResultType> >::move_dest_type move_dest_type;

private:
    struct shared_joiner_data {
        shared_future< result_type > fut_;
        thread th_;
        on_destruction::type on_destruction_;

        shared_joiner_data(on_destruction::type on_destruction_do= on_destruction::do_join) 
            : on_destruction_(on_destruction_do) 
        {}
        ~shared_joiner_data() {
            if (th_.joinable()) {
                if (on_destruction_== on_destruction::do_terminate) {
                    std::terminate();
                } else if (on_destruction_== on_destruction::do_join) {
                    th_.join();
                }
            }
        }

        template <typename Nullary>
#ifdef BOOST_THREAD_HAS_THREAD_ATTR
        shared_joiner_data(thread::native_handle_attr_type& attr, Nullary f, on_destruction::type on_destruction_do) {
            : on_destruction(on_destruction_do) 
        {
            packaged_task<result_type> tsk(f);
            fut_ = tsk.get_future();
            thread th(attr, boost::move(tsk));
            th_ = boost::move(th);
        }
#else
        shared_joiner_data(Nullary f, on_destruction::type on_destruction_do)
            : on_destruction_(on_destruction_do) 
        {
            packaged_task<result_type> tsk(f);
            fut_ = tsk.get_future();
#if 0
            th_ = boost::move(tsk);
#else
            thread th(boost::move(tsk));
            th_ = boost::move(th);
#endif
        }
#endif

    };
    shared_ptr<shared_joiner_data> data_;
    typedef shared_joiner_data this_impl_type;
    typedef shared_joiner this_type;

//protected:
public:
    friend class shared_threader;
    template <typename Nullary>
    // requires result_of<Nullary>::type  is convertible to ResultType
#ifdef BOOST_THREAD_HAS_THREAD_ATTR
    shared_joiner(thread::native_handle_attr_type& attr, Nullary f, on_destruction::type on_destruction_do=on_destruction::do_join)
    : data_(new this_impl_type(attr, f,on_destruction_do))
#else
    shared_joiner(Nullary f, on_destruction::type on_destruction_do=on_destruction::do_join)
    : data_(new this_impl_type(f,on_destruction_do))
#endif
    {
    }


public:
    shared_joiner() : data_() {};
    shared_joiner(const shared_joiner& other) : data_(other.data_){
    }
    this_type& operator=(const shared_joiner& other) {
        data_=other.data_;
        return *this;
    }

    // move support
#ifdef BOOST_HAS_RVALUE_REFS
    shared_joiner(this_type&& other): data_(other.data_) {
        other.data_.reset(new this_impl_type());
    }
    shared_joiner& operator=(shared_joiner&& other) {
        data_=other.data_;
        other.data_.reset(new this_impl_type());
        return *this;
    }

    shared_joiner&& move() {
        return static_cast<shared_joiner&&>(*this);
    }
#else
    shared_joiner(boost::detail::thread_move_t<shared_joiner> x) {
        data_=x->data_;
        x->data_.reset(new this_impl_type());
    }

    shared_joiner& operator=(boost::detail::thread_move_t<shared_joiner> x) {
        this_type tmp_(x);
        swap(tmp_);
        return *this;
    }

    operator boost::detail::thread_move_t<shared_joiner>() {
        return move();
    }

    boost::detail::thread_move_t<shared_joiner> move() {
        return boost::detail::thread_move_t<shared_joiner>(*this);
    }

#endif

    void swap(this_type& x) {
        data_.swap(x.data_);
    }

    bool joinable() {
        return data_->th_.joinable();
    }

    void join() {
        data_->th_.join();
    }

    bool join_until(const system_time& abs_time) {
        return data_->th_.timed_join(abs_time);
    }

    template<typename TimeDuration>
    inline bool join_for(TimeDuration const& rel_time)
    {
        return join_until(get_system_time()+rel_time);
    }
    move_dest_type get() const {
        return data_->fut_.get();
    }

    move_dest_type operator()() const { return get(); }

    bool is_ready() const {
        return data_->fut_.is_ready();
    }
    bool has_exception() const {
        return data_->fut_.has_exception();
    }
    bool has_value() const {
        return data_->fut_.has_value();
    }

    void wait() {
        data_->fut_.wait();
    }
    bool wait_until(const system_time& abs_time) {
        return data_->fut_.timed_wait_until(abs_time);
    }

    template<typename TimeDuration>
    inline bool wait_for(TimeDuration const& rel_time)
    {
        return wait_until(get_system_time()+rel_time);
    }

    void detach() {
        data_->th_.detach();
    }

    void interrupt() {
        data_->th_.interrupt();
    }

    bool interruption_requested() const {
        return data_->th_.interruption_requested();
    }

    thread::id get_id() const {
        return data_->th_.get_id();
    }

    shared_future<result_type>& get_future() {
        return data_->fut_;
    }
};

template <typename R>
struct is_movable<shared_joiner<R> > : mpl::true_{};

template <typename R>
struct has_future_if<shared_joiner<R> > : mpl::true_{};

template <typename R>
struct has_thread_if<shared_joiner<R> > : mpl::true_{};


class shared_threader {
#ifdef BOOST_THREAD_HAS_THREAD_ATTR
private:
    thread_specific_ptr<thread::native_handle_attr_type> attr_;
public:
    thread::native_handle_attr_type& attr() {
        if(attr_.get() ==0) {
            attr_.reset(new thread::native_handle_attr_type());
        };
        return *attr_.get();
    }
#endif

public:
    template <typename T>
    struct handle {
        typedef shared_joiner<T> type;
    };
    template <typename F>
    shared_joiner<typename boost::result_of<F()>::type>
    fork(F f) {
        typedef typename boost::result_of<F()>::type result_type;
#ifdef BOOST_THREAD_HAS_THREAD_ATTR
        return shared_joiner<result_type>(attr(), f);
#else
        return shared_joiner<result_type>(f);
#endif
    }



};

template <>
struct get_future<shared_threader> {
    template <typename T>
    struct future_type {
        typedef shared_future<T> type;
    };
    template <typename T>
    shared_future<T>& operator()(shared_joiner<T>& j) { return j.get_future(); }
};

}
#ifdef BOOST_HAS_RVALUE_REFS
    template <typename T>
    inline async::unique_joiner<T>&& move(async::unique_joiner<T>&& t)
    {
        return t;
    }
#else
    template <typename T>
    inline boost::detail::thread_move_t<async::unique_joiner<T> >
    move(boost::async::unique_joiner<T>&  t)
    {
        return boost::detail::thread_move_t<async::unique_joiner<T> >(t);
    }
#endif
#ifdef BOOST_HAS_RVALUE_REFS
    template <typename T>
    inline async::shared_joiner<T>&& move(async::shared_joiner<T>&& t)
    {
        return t;
    }
#else
    template <typename T>
    inline boost::detail::thread_move_t<async::shared_joiner<T> >
    move(boost::async::shared_joiner<T>& t)
    {
        return boost::detail::thread_move_t<async::shared_joiner<T> >(t);
    }
#endif
}


#include <boost/config/abi_suffix.hpp>

#endif


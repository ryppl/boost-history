#ifndef BOOST_ASYNC_FORK_AFTER__HPP
#define BOOST_ASYNC_FORK_AFTER__HPP

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

#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/futures/future.hpp>
#include <boost/async/fork.hpp>
#include <boost/async/algorithm/wait_all.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/shared_ptr.hpp>

#ifdef BOOST_HAS_CHRONO_LIB
#include <boost/chrono/chrono.hpp>
#else
#include <boost/thread/thread_time.hpp>
#endif

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace async {

#define ACT_WRAPPER
#ifdef ACT_WRAPPER
template <typename ACT>
struct act_wrapper;

template <typename ACT>
struct act_traits<act_wrapper<ACT> > : act_traits<ACT>{};

template <typename ACT>
struct act_wrapper {
    typedef typename act_traits<act_wrapper<ACT> >::move_dest_type move_dest_type;

    act_wrapper() :ptr_(new data()) {}
    void wait_initialized() {
        while (!ptr_->inittialized_) {
            unique_lock<mutex> lk(ptr_->mtx_);
            ptr_->cv_.wait(lk);
        }
    }
    void set_initialized() {
        unique_lock<mutex> lk(ptr_->mtx_);
        ptr_->inittialized_=true;
    }
    void set(ACT& other) {
        ptr_->act_=boost::move(other);
        set_initialized();
        ptr_->cv_.notify_all();
    }
    void set(boost::detail::thread_move_t<ACT> other) {
        ptr_->act_=other;
        set_initialized();
        ptr_->cv_.notify_all();
    }

    void wait() {
        wait_initialized();
        ptr_->act_.wait();
    }

    bool wait_until(const system_time& abs_time) {
        wait_initialized();
        return ptr_->act_.wait_until(abs_time);
    }

    template <typename Duration>
    bool wait_for(ACT& act, Duration rel_time) {
        wait_initialized();
        return ptr_->act_.wait_for(rel_time);
    }

    move_dest_type get() {
        wait_initialized();
        return ptr_->act_.get();
    }

    bool is_ready() {
        wait_initialized();
        return ptr_->act_.is_ready();
    }

    bool has_value() {
        wait_initialized();
        return ptr_->act_.has_value();
    }

    bool has_exception() {
        wait_initialized();
        return ptr_->act_.has_exception();
    }

    void detach() {
        wait_initialized();
        ptr_->act_.detach();
    }

    bool joinable() {
        wait_initialized();
        return ptr_->act_.joinable();
    }

    void join() {
        wait_initialized();
        ptr_->act_.join();
    }

    bool join_until(const system_time& abs_time) {
        wait_initialized();
        return ptr_->act_.join_until(abs_time);
    }

    template <typename Duration>
    bool join_for(ACT& act, Duration rel_time) {
        wait_initialized();
        return ptr_->act_.join_for(rel_time);
    }

    void interrupt() {
        wait_initialized();
        ptr_->act_.interrupt();
    }

    bool interruption_requested() {
        wait_initialized();
        return ptr_->act_.interruption_requested();
    }


private:
    struct data {
        data()
        : inittialized_(false)
        {}
        ACT act_;
        bool inittialized_;
        mutex mtx_;
        condition_variable cv_;
    };
    shared_ptr<data> ptr_;
};


template <typename ACT>
struct is_movable<act_wrapper<ACT> > : is_movable<ACT>{};

template <typename ACT>
struct has_future_if<act_wrapper<ACT> > : has_future_if<ACT> {};

template <typename ACT>
struct has_thread_if<act_wrapper<ACT> > : has_thread_if<ACT>{};


namespace result_of {
    template <typename AE,typename F>
    struct fork_after {
        typedef typename boost::result_of<F()>::type result_type;
        typedef typename asynchronous_completion_token<AE, result_type>::type act_type;
        typedef act_wrapper<act_type> type;
    };
}

#else
namespace result_of {
    template <typename AE,typename F>
    struct fork_after {
        typedef void type;
    };
}
#endif
template <typename AE, typename F, typename D>
struct call_f_after {
    typedef void result_type;
#ifndef ACT_WRAPPER
    void operator()(AE& ae, F fn, D& d,
        typename asynchronous_completion_token<AE,typename boost::result_of<F()>::type>::type& h
    ) {
        //d.wait();
        boost::async::wait_all(d);
        typename asynchronous_completion_token<AE,typename boost::result_of<F()>::type>::type tmp = fork(ae, fn);
        h = boost::move(tmp);
    }
#else
    void operator()(AE& ae, F fn, D& d, act_wrapper<
        typename asynchronous_completion_token<AE,typename boost::result_of<F()>::type>::type>& h
    ) {
        //d.wait();
        boost::async::wait_all(d);
        typename asynchronous_completion_token<AE,typename boost::result_of<F()>::type>::type tmp = fork(ae, fn);
        h.set(tmp);
        //h.set(boost::move(tmp));

    }
#endif
};

namespace partial_specialization_workaround {
template< typename AE, typename F, typename D >
struct fork_after {
#ifndef ACT_WRAPPER
    static void
    apply(AE& ae, F fn, D& d,
        typename asynchronous_completion_token<AE,typename boost::result_of<F()>::type>::type& h
    ) {
        call_f_after<AE,F,D> f;
        boost::async::fork(ae, boost::bind(f, boost::ref(ae), fn, boost::ref(d), boost::ref(h)));
    }
#else
    static typename result_of::fork_after<AE,F>::type
    apply(AE& ae, F fn, D& d) {
        act_wrapper<
            typename asynchronous_completion_token<AE,
            typename boost::result_of<F()>::type>::type> h;
        call_f_after<AE,F,D> f;
        boost::async::fork(ae, boost::bind(f, boost::ref(ae), fn, boost::ref(d), boost::ref(h)));
        return h;
    }
#endif
};
}
template< typename AE, typename F, typename D>
#ifndef ACT_WRAPPER
void
fork_after( AE& ae, F fn, D& d,
    typename asynchronous_completion_token<AE,typename boost::result_of<F()>::type>::type& h) {
    return partial_specialization_workaround::fork_after<AE,F,D>::apply(ae,fn, d, h);
}
#else
typename result_of::fork_after<AE,F>::type
fork_after( AE& ae, F fn, D& d) {
    return partial_specialization_workaround::fork_after<AE,F,D>::apply(ae,fn, d);
}
#endif

template< typename AE, typename D, typename F>
act_wrapper< typename asynchronous_completion_token<AE, typename boost::result_of<F()>::type >::type >
after_completion_fork( AE& ae, D& d, F fn) {
    return fork_after(ae, fn, d );
}

template< typename AE, typename D, typename F, typename A1 >
act_wrapper< typename asynchronous_completion_token<AE, typename boost::result_of<F(A1)>::type >::type >
after_completion_fork( AE& ae, D& d, F fn, A1 a1 ) {
    return fork_after(ae, bind( fn, a1 ), d );
}

template< typename AE, typename D, typename F, typename A1, typename A2  >
act_wrapper< typename asynchronous_completion_token<AE, typename boost::result_of<F(A1,A2)>::type >::type >
after_completion_fork( AE& ae, D& d, F fn, A1 a1, A2 a2 ) {
    return fork_after(ae, bind( fn, a1, a2 ), d );
}

template< typename AE, typename D, typename F, typename A1, typename A2, typename A3  >
act_wrapper< typename asynchronous_completion_token<AE, typename boost::result_of<F(A1,A2,A3)>::type >::type >
after_completion_fork( AE& ae, D& d, F fn, A1 a1, A2 a2, A3 a3 ) {
    return fork_after(ae, bind( fn, a1, a2, a3 ), d );
}

}
}


#include <boost/config/abi_suffix.hpp>

#endif


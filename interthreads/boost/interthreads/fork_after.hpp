#ifndef BOOST_INTERTHREADS_FORK_AFTER__HPP
#define BOOST_INTERTHREADS_FORK_AFTER__HPP

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

#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/utility/result_of.hpp>
#include <boost/futures/future.hpp>
#include <boost/interthreads/fork.hpp>
#include <boost/interthreads/algorithm/wait_all.hpp>

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/locks.hpp>
#include <boost/shared_ptr.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {

#define ACT_WRAPPER
#ifdef ACT_WRAPPER

template <typename ACT>
struct act_wrapper {
    act_wrapper() :ptr_(new data()) {}
    void wait_initialized() {
        while (!ptr_->inittialized_) {
            unique_lock<mutex> lk(ptr_->mtx_);
            ptr_->cv_.wait(lk);
        }
    }
    void wait() {
        wait_initialized();
        ptr_->act_.wait();
    }

    typename act_value<ACT>::type get() {
        wait_initialized();
        return ptr_->act_.get();
    }

    void join() {
        wait_initialized();
        ptr_->act_.join();
    }
    void set_initialized(ACT& other) {
        ptr_->act_=boost::move(other);
        ptr_->inittialized_=true;
        ptr_->cv_.notify_all();
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
struct act_value<act_wrapper<ACT> > {
    typedef typename act_value<ACT>::type type;
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
    void operator()(AE& ae, F fn, D& d, typename AE:: template handle<typename boost::result_of<F()>::type>::type& h) {
        //d.wait();
        boost::interthreads::wait_all(d);
        typename AE:: template handle<typename boost::result_of<F()>::type>::type tmp = fork(ae, fn);
        //h = boost::move(tmp);
        h = tmp;
        
    }
#else
    void operator()(AE& ae, F fn, D& d, act_wrapper<typename AE:: template handle<typename boost::result_of<F()>::type>::type>& h) {
        //d.wait();
        boost::interthreads::wait_all(d);
        typename AE:: template handle<typename boost::result_of<F()>::type>::type tmp = fork(ae, fn);
        //h = boost::move(tmp);
        h.set_initialized(tmp);        
    }
#endif
};

namespace partial_specialization_workaround {
template< typename AE, typename F, typename D > 
struct fork_after {
#ifndef ACT_WRAPPER
    static void 
    apply(AE& ae, F fn, D& d, typename AE:: template handle<typename boost::result_of<F()>::type>::type& h ) {
        call_f_after<AE,F,D> f;
        boost::interthreads::fork(ae, boost::bind(f, boost::ref(ae), fn, boost::ref(d), boost::ref(h)));
    }
#else        
    static typename result_of::fork_after<AE,F>::type
    apply(AE& ae, F fn, D& d) {
        act_wrapper<typename AE:: template handle<typename boost::result_of<F()>::type>::type> h;
        call_f_after<AE,F,D> f;
        boost::interthreads::fork(ae, boost::bind(f, boost::ref(ae), fn, boost::ref(d), boost::ref(h)));
        return h;
    }
#endif        
};
}
template< typename AE, typename F, typename D> 
#ifndef ACT_WRAPPER
void 
fork_after( AE& ae, F fn, D& d, typename AE:: template handle<typename boost::result_of<F()>::type>::type& h) {
    return partial_specialization_workaround::fork_after<AE,F,D>::apply(ae,fn, d, h);
}
#else    
typename result_of::fork_after<AE,F>::type 
fork_after( AE& ae, F fn, D& d) {
    return partial_specialization_workaround::fork_after<AE,F,D>::apply(ae,fn, d);
}
#endif    

#if 0
template< typename AE, typename F, typename A1 > 
typename asynchronous_completion_token<AE, typename boost::result_of<F(A1)>::type >::type
fork( AE& ae, F fn, A1 a1 ) {
    return ae.fork( bind( fn, a1 ) );
}

template< typename AE, typename F, typename A1, typename A2 >
typename  asynchronous_completion_token<AE, typename boost::result_of<F(A1,A2)>::type >::type
fork( AE& ae, F fn, A1 a1, A2 a2 ) {
    return ae.fork( bind( fn, a1, a2 ) );
}

template< typename AE, typename F, typename A1, typename A2, typename A3 >
typename  asynchronous_completion_token<AE, typename boost::result_of<F(A1,A2,A3)>::type >::type
fork( AE& ae, F fn, A1 a1, A2 a2, A3 a3 ) {
    return ae.fork( bind( fn, a1, a2, a3 ) );
}
#endif
}
}


#include <boost/config/abi_suffix.hpp>

#endif


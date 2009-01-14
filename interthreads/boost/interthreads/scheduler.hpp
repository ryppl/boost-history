#ifndef BOOST_INTERTHREADS_SCHEDULER__HPP
#define BOOST_INTERTHREADS_SCHEDULER__HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-20009. 
// Distributed under the Boost Software License, Version 1.0. 
// (See accompanying file LICENSE_1_0.txt or 
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the unique_threader/unique_joiner design from of Kevlin Henney (n1883)
//
// See http://www.boost.org/libs/interthreads for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#include <boost/thread/detail/move.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread/thread.hpp>
#include <boost/thread/tss.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/tp/pool.hpp>
#include <boost/futures/future.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/interthreads/fork.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost {
namespace interthreads {

   
template <typename C>
class scheduler {
    tp::pool<C> _pool;
public: 
    explicit scheduler(
        tp::poolsize const& psize
    ) : _pool(psize)
    {};
    template <typename T>
    struct handle {
        typedef tp::task<T> type;
    };
    template <typename F>
    tp::task<typename boost::result_of<F()>::type> 
    fork(F f) {
        return _pool.submit(f);
    }

};

template <typename Channel>
struct get_future<scheduler<Channel> > {
    template <typename T>
    shared_future<T>& operator()(tp::task<T>& act) { return act.get_future(); }
};



template <typename Channel, typename T>
struct asynchronous_completion_token<boost::tp::pool<Channel>,T> {
    typedef boost::tp::task<T> type;
};    


template< typename Channel, typename F > 
struct fork_aux<boost::tp::pool<Channel>,F> {
    typename result_of::fork<boost::tp::pool<Channel>, F>::type  
    fork( boost::tp::pool<Channel>& ae, F fn ) {
        return ae.submit(fn);
    }
};

template <typename C>
struct get_future<tp::pool<C> > {
    template <typename T>
    shared_future<T>& operator()(tp::task<T>& act) { return act.get_future(); }
};

}
}


#include <boost/config/abi_suffix.hpp>

#endif


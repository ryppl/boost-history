#ifndef BOOST_ASYNC_SCHEDULER__HPP
#define BOOST_ASYNC_SCHEDULER__HPP

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
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/tp/pool.hpp>
#include <boost/futures/future.hpp>
#include <boost/utility/result_of.hpp>

#include <boost/async/algorithm.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost
{
#ifdef TASK_POOL
    template<typename Pool, typename T>
    boost::tp::task<Pool, T>& move(boost::tp::task<Pool, T>& t)
#else
    template<typename T>
    boost::tp::task<T>& move(boost::tp::task<T>& t)
#endif
    {
        return t;
    }
}


namespace boost {
namespace async {

#if 0
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
#ifdef TASK_POOL
        typedef typename tp::pool<C>::template handle<T>::type type;
#else
        typedef tp::task<T> type;
#endif
    };
    template <typename F>
    typename handle<typename boost::result_of<F()>::type>::type
    fork(F f) {
        return _pool.submit(f);
    }

};

template <typename Channel>
struct get_future<scheduler<Channel> > {
    template <typename T>
    struct future_type {
        typedef shared_future<T> type;
    };
    template <typename T>
    shared_future<T>& operator()(tp::task<T>& act) { return act.get_future(); }
};


#endif


template <typename Channel, typename T>
struct asynchronous_completion_token<boost::tp::pool<Channel>,T> {
#ifdef TASK_POOL
    typedef typename tp::template pool<Channel>::template handle<T>::type type;
#else
    typedef boost::tp::task<T> type;
#endif
};


namespace partial_specialization_workaround {
    template< typename Channel, typename F >
    struct fork<boost::tp::pool<Channel>,F> {
        static typename result_of::fork<boost::tp::pool<Channel>, F>::type
        apply( boost::tp::pool<Channel>& ae, F fn ) {
            return ae.submit(fn);
        }
    };
    
    
    
}

template <typename C>
struct get_future<tp::pool<C> > {
    template <typename T>
    struct future_type {
        typedef shared_future<T> type;
    };
    template <typename T>
    shared_future<T> operator()(
#ifdef TASK_POOL
        //typename asynchronous_completion_token<tp::pool<C>,T>::type & act
        tp::task<tp::pool<C>, T>& act
#else
        tp::task<T>& act
#endif
    ) { return act.result(); }
};

#ifdef TASK_POOL
template <typename Pool, typename ResultType>
struct act_traits< tp::task<Pool, ResultType> > {
    typedef ResultType move_dest_type;
};

template <typename Pool, typename R>
struct is_movable<tp::task<Pool, R> > : mpl::false_{};

template <typename Pool, typename R>
struct has_future_if<tp::task<Pool, R> > : mpl::true_{};

template <typename Pool, typename R>
struct has_thread_if<tp::task<Pool, R> > : mpl::true_{};
#else
template <typename ResultType>
struct act_traits< tp::task<ResultType> > {
    typedef ResultType move_dest_type;
};

template <typename R>
struct is_movable<tp::task<R> > : mpl::false_{};

template <typename R>
struct has_future_if<tp::task<R> > : mpl::true_{};

template <typename R>
struct has_thread_if<tp::task<R> > : mpl::true_{};
#endif

#ifdef TASK_POOL

    
    
    namespace partial_specialization_workaround {
        template <typename Pool, typename R, typename Duration>
        struct wait_until<tp::task<Pool, R> > {
            static typename result_of::template wait_until<tp::task<Pool, R> >::type apply( tp::task<Pool, R>& act, const system_time& abs_time ) {
                return act.result().timed_wait_until(abs_time);
            }
        };
        template <typename Pool, typename R, typename Duration>
        struct wait_for<tp::task<Pool, R>, Duration> {
            static typename result_of::template wait_for<tp::task<Pool, R>,Duration>::type 
            apply( tp::task<Pool, R>& act, Duration rel_time ) {
                return act.result().timed_wait(rel_time);
            }
        };
        
        template <typename Pool, typename R>
        struct join<tp::task<Pool, R> > {
            static typename result_of::template join<tp::task<Pool, R> >::type apply( tp::task<Pool, R>& act) {
                return act.result().wait();
            }
        };
        template <typename Pool, typename R>
        struct join_until<tp::task<Pool, R> > {
            static typename result_of::template join_until<tp::task<Pool, R> >::type apply( tp::task<Pool, R>& act, const system_time& abs_time ) {
                return act.result().wait_until(abs_time);
            }
        };
        template <typename Pool, typename R, typename Duration>
        struct join_for<tp::task<Pool, R>, Duration> {
            static typename result_of::template join_for<tp::task<Pool, R>,Duration>::type apply( tp::task<Pool, R>& act, Duration rel_time ) {
                return async::join_until(act, get_system_time()+rel_time );
            }
        };
        template< typename Pool, typename R >
        struct interruption_requested<tp::task<Pool, R> > {
            static typename result_of::template interruption_requested<tp::task<Pool, R> >::type apply( tp::task<Pool, R>& act ) {
                return act.interruption_requested();
            }
        };
    }
#else
    namespace partial_specialization_workaround {
        template< typename R >
        struct wait<tp::task<R> > {
            static typename result_of::template wait<tp::task<R> >::type apply( tp::task<R>& act ) {
                return act.result().wait();
            }
        };

        template <typename R>
        struct wait_until<tp::task<R> > {
            static typename result_of::template wait_until<tp::task<R> >::type apply( tp::task<R>& act, const system_time& abs_time ) {
                return act.result().timed_wait_until(abs_time);
            }
        };
        template <typename R, typename Duration>
        struct wait_for<tp::task<R>, Duration> {
            static typename result_of::template wait_for<tp::task<R>,Duration>::type apply( tp::task<R>& act, Duration abs_time ) {
                return async::wait_until(act, get_system_time()+abs_time);
            }
        };
        
        template <typename R> 
        struct get<tp::task<R> > {
            static typename result_of::template get<tp::task<R> >::type apply( tp::task<R>& act ) {
                return act.result().get();
            }
        };

        template <typename R>
        struct join<tp::task<R> > {
            static typename result_of::template join<tp::task<R> >::type apply( tp::task<R>& act) {
                return async::wait(act);
            }
        };
        template <typename R>
        struct join_until<tp::task<R> > {
            static typename result_of::template join_until<tp::task<R> >::type apply( tp::task<R>& act, const system_time& abs_time ) {
                return async::wait_until(act, abs_time);
            }
        };
        template <typename R, typename Duration>
        struct join_for<tp::task<R>, Duration> {
            static typename result_of::template join_for<tp::task<R>,Duration>::type apply( tp::task<R>& act, Duration abs_time ) {
                return async::wait_until(act, get_system_time()+abs_time);
            }
        };
        template< typename R >
        struct interruption_requested<tp::task<R> > {
            static typename result_of::template interruption_requested<tp::task<R> >::type apply( tp::task<R>& act ) {
                return act.interruption_requested();
            }
        };
    }


#endif
}
}

#include <boost/config/abi_suffix.hpp>

#endif


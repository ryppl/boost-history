#ifndef BOOST_ASYNC_FUTURE_TRAITS__HPP
#define BOOST_ASYNC_FUTURE_TRAITS__HPP

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

#include <boost/async/act_traits.hpp>
#include <boost/thread/detail/move.hpp>
#include <boost/futures/future.hpp>
#include <boost/type_traits/is_fundamental.hpp>
#include <boost/type_traits/is_convertible.hpp>
#include <boost/mpl/if.hpp>

#include <boost/config/abi_prefix.hpp>


namespace boost { namespace async {

    template<typename T>
    struct act_traits<unique_future<T> >
    {
#ifdef BOOST_HAS_RVALUE_REFS
        typedef typename boost::mpl::if_<boost::is_fundamental<T>,T,T&&>::type move_dest_type;
#else
        typedef typename boost::mpl::if_<boost::is_convertible<T&,boost::detail::thread_move_t<T> >,boost::detail::thread_move_t<T>,T>::type move_dest_type;
#endif

#if 0
        typedef boost::scoped_ptr<T> storage_type;
#ifdef BOOST_HAS_RVALUE_REFS
        typedef T const& source_reference_type;
        struct dummy;
        typedef typename boost::mpl::if_<boost::is_fundamental<T>,dummy&,T&&>::type rvalue_source_type;
#else
        typedef T& source_reference_type;
        typedef typename boost::mpl::if_<boost::is_convertible<T&,boost::detail::thread_move_t<T> >,boost::detail::thread_move_t<T>,T const&>::type rvalue_source_type;
#endif
        static void init(storage_type& storage,source_reference_type t)
        {
            storage.reset(new T(t));
        }

        static void init(storage_type& storage,rvalue_source_type t)
        {
            storage.reset(new T(static_cast<rvalue_source_type>(t)));
        }

        static void cleanup(storage_type& storage)
        {
            storage.reset();
        }
#endif        
    };

    template<typename T>
    struct act_traits<unique_future<T&> >
    {
        typedef T& move_dest_type;
#if 0
        typedef T* storage_type;
        typedef T& source_reference_type;
        struct rvalue_source_type {};

        static void init(storage_type& storage,T& t)
        {
            storage=&t;
        }

        static void cleanup(storage_type& storage)
        {
            storage=0;
        }
#endif
    };

    template<>
    struct act_traits<unique_future<void> >
    {
        typedef void move_dest_type;

#if 0
        typedef bool storage_type;
        static void init(storage_type& storage)
        {
            storage=true;
        }

        static void cleanup(storage_type& storage)
        {
            storage=false;
        }

#endif
    };

    template<typename T>
    struct act_traits<shared_future<T> >
    {
        typedef T move_dest_type;

#if 0
        typedef boost::scoped_ptr<T> storage_type;
#ifdef BOOST_HAS_RVALUE_REFS
        typedef T const& source_reference_type;
        struct dummy;
        typedef typename boost::mpl::if_<boost::is_fundamental<T>,dummy&,T&&>::type rvalue_source_type;
#else
        typedef T& source_reference_type;
        typedef typename boost::mpl::if_<boost::is_convertible<T&,boost::detail::thread_move_t<T> >,boost::detail::thread_move_t<T>,T const&>::type rvalue_source_type;
#endif
        static void init(storage_type& storage,source_reference_type t)
        {
            storage.reset(new T(t));
        }

        static void init(storage_type& storage,rvalue_source_type t)
        {
            storage.reset(new T(static_cast<rvalue_source_type>(t)));
        }

        static void cleanup(storage_type& storage)
        {
            storage.reset();
        }
#endif
    };

    template<typename T>
    struct act_traits<shared_future<T&> >
    {
        typedef T& move_dest_type;

#if 0
        typedef T* storage_type;
        typedef T& source_reference_type;
        struct rvalue_source_type {};
        static void init(storage_type& storage,T& t)
        {
            storage=&t;
        }

        static void cleanup(storage_type& storage)
        {
            storage=0;
        }
#endif
    };

    template<>
    struct act_traits<shared_future<void> >
    {
        typedef void move_dest_type;

#if 0
        typedef bool storage_type;
        static void init(storage_type& storage)
        {
            storage=true;
        }

        static void cleanup(storage_type& storage)
        {
            storage=false;
        }
#endif

    };

    template <typename R>
    struct is_movable<unique_future<R> > : mpl::true_{};

    template <typename R>
    struct has_future_if<unique_future<R> > : mpl::true_{};

    template <typename R>
    struct has_thread_if<unique_future<R> > : mpl::false_{};


    template <typename R>
    struct is_movable<shared_future<R> > : mpl::true_{};

    template <typename R>
    struct has_future_if<shared_future<R> > : mpl::true_{};

    template <typename R>
    struct has_thread_if<shared_future<R> > : mpl::true_{};

    namespace partial_specialization_workaround {
        template <typename R>
        struct join<unique_future<R> > {
            static typename result_of::template join<unique_future<R> >::type apply( unique_future<R>& act) {
                return act.wait();
            }
        };
        template <typename R>
        struct join<shared_future<R> > {
            static typename result_of::template join<shared_future<R> >::type apply( shared_future<R>& act) {
                return act.wait();
            }
        };
        template <typename R>
        struct join_until<unique_future<R> > {
            static typename result_of::template join_until<unique_future<R> >::type apply( unique_future<R>& act, const system_time& abs_time ) {
                return act.timed_wait_until(abs_time);
            }
        };
        template <typename R>
        struct join_until<shared_future<R> > {
            static typename result_of::template join_until<shared_future<R> >::type apply( shared_future<R>& act, const system_time& abs_time ) {
                return act.timed_wait_until(abs_time);
            }
        };
        template <typename R, typename Duration>
        struct join_for<unique_future<R>, Duration> {
            static typename result_of::template join_for<unique_future<R>,Duration>::type apply( unique_future<R>& act, Duration rel_time ) {
                return act.timed_wait(rel_time);
            }
        };
        template <typename R, typename Duration>
        struct join_for<shared_future<R>, Duration> {
            static typename result_of::template join_for<shared_future<R>,Duration>::type apply( shared_future<R>& act, Duration rel_time ) {
                return act.timed_wait(rel_time);
            }
        };
        template <typename R>
        struct wait_until<unique_future<R> > {
            static typename result_of::template wait_until<unique_future<R> >::type apply( unique_future<R>& act, const system_time& abs_time ) {
                return act.timed_wait_until(abs_time);
            }
        };
        template <typename R>
        struct wait_until<shared_future<R> > {
            static typename result_of::template wait_until<shared_future<R> >::type apply( shared_future<R>& act, const system_time& abs_time ) {
                return act.timed_wait_until(abs_time);
            }
        };
        template <typename R, typename Duration>
        struct wait_for<unique_future<R>, Duration> {
            static typename result_of::template wait_for<unique_future<R>,Duration>::type apply( unique_future<R>& act, Duration rel_time ) {
                return act.timed_wait(rel_time);
            }
        };
        template <typename R, typename Duration>
        struct wait_for<shared_future<R>, Duration> {
            static typename result_of::template wait_for<shared_future<R>,Duration>::type apply( shared_future<R>& act, Duration rel_time ) {
                return act.timed_wait(rel_time);
            }
        };
    }

}
}


#include <boost/config/abi_suffix.hpp>

#endif


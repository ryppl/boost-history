// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams

#ifndef BOOST_SYNCHRO_LOCKABLES_LOCK_UNTIL__HPP
#define BOOST_SYNCHRO_LOCKABLES_LOCK_UNTIL__HPP

#include <algorithm>
#include <iterator>

#include <boost/thread/locks.hpp>
#include <boost/synchro/lockables/detail/lock_until_internal.hpp>
#include <boost/synchro/lockables/detail/is_mutex_type_wrapper.hpp>
#include <boost/synchro/is_lockable.hpp>
#include <boost/synchro/lockers.hpp>
#include <boost/chrono/chrono.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockables {
    
    namespace detail
    {

        template<typename MutexType1,typename MutexType2, class Clock, class Duration >
        void lock_until_impl(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2,is_mutex_type_wrapper<true>)
        {
            lock_until_internal(abs_time,m1,m2);
        }

        template<typename Iterator, class Clock, class Duration >
        void lock_until_impl(const chrono::time_point<Clock, Duration>& abs_time, Iterator begin,Iterator end,is_mutex_type_wrapper<false>);
    }
    
    template<typename MutexType1,typename MutexType2, class Clock, class Duration >
    void lock_until(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2)
    {
        detail::lock_until_impl(abs_time,m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2, class Clock, class Duration >
    void lock_until(const chrono::time_point<Clock, Duration>& abs_time, const MutexType1& m1,MutexType2& m2)
    {
        detail::lock_until_impl(abs_time,m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2, class Clock, class Duration >
    void lock_until(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,const MutexType2& m2)
    {
        detail::lock_until_impl(abs_time,m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2, class Clock, class Duration >
    void lock_until(const chrono::time_point<Clock, Duration>& abs_time, const MutexType1& m1,const MutexType2& m2)
    {
        detail::lock_until_impl(abs_time,m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3, class Clock, class Duration >
    void lock_until(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2,MutexType3& m3)
    {
        detail::lock_until_internal(abs_time,m1,m2,m3);
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3,typename MutexType4, class Clock, class Duration >
    void lock_until(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2,MutexType3& m3,MutexType4& m4)
    {
        detail::lock_until_internal(abs_time,m1,m2,m3,m4);
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3,typename MutexType4,typename MutexType5, class Clock, class Duration >
    void lock_until(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2,MutexType3& m3,MutexType4& m4,MutexType5& m5)
    {
        detail::lock_until_internal(abs_time,m1,m2,m3,m4,m5);
    }
    

    namespace detail {

        template<typename Iterator, class Clock, class Duration >
        void lock_until_impl(const chrono::time_point<Clock, Duration>& abs_time, Iterator begin,Iterator end,is_mutex_type_wrapper<false>)

        {
            if(begin==end) {
                return;
            }
            typedef typename std::iterator_traits<Iterator>::value_type lock_type;
            unique_locker<lock_type> guard(abs_time,*begin);
            lock_until(abs_time,++begin,end);
            guard.release();
        }
    }
    
}}}

#include <boost/config/abi_suffix.hpp>

#endif

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams

#ifndef BOOST_SYNCHRO_LOCKABLES_TRY_LOCK_UNTIL__HPP
#define BOOST_SYNCHRO_LOCKABLES_TRY_LOCK_UNTIL__HPP

#include <algorithm>
#include <iterator>

#include <boost/thread/locks.hpp>
#include <boost/synchro/lockables/detail/try_lock_until_internal.hpp>
#include <boost/synchro/lockables/detail/is_mutex_type_wrapper.hpp>
#include <boost/synchro/lockers.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockables {
    
    namespace detail
    {

        template<typename MutexType1,typename MutexType2, class Clock, class Duration >
        int lock_until_impl(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2,is_mutex_type_wrapper<true>)
        {
            return ((int)detail::lock_until_internal(abs_time,m1,m2))-1;
        }

        template<typename Iterator, class Clock, class Duration >
        Iterator lock_until_impl(const chrono::time_point<Clock, Duration>& abs_time, Iterator begin,Iterator end,is_mutex_type_wrapper<false>);
    }
    
    template<typename MutexType1,typename MutexType2, class Clock, class Duration >
    void lock_until(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2)
    {
        return detail::lock_until_impl(abs_time,m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2, class Clock, class Duration >
    void lock_until(const chrono::time_point<Clock, Duration>& abs_time, const MutexType1& m1,MutexType2& m2)
    {
        return detail::lock_until_impl(abs_time,m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2, class Clock, class Duration >
    void lock_until(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,const MutexType2& m2)
    {
        return detail::lock_until_impl(abs_time,m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2, class Clock, class Duration >
    void lock_until(const chrono::time_point<Clock, Duration>& abs_time, const MutexType1& m1,const MutexType2& m2)
    {
        return detail::lock_until_impl(abs_time,m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3, class Clock, class Duration >
    void lock_until(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2,MutexType3& m3)
    {
        return ((int)detail::lock_until_internal(abs_time,m1,m2,m3))-1;
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3,typename MutexType4, class Clock, class Duration >
    void lock_until(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2,MutexType3& m3,MutexType4& m4)
    {
        return ((int)detail::lock_until_internal(abs_time,m1,m2,m3,m4))-1;
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3,typename MutexType4,typename MutexType5, class Clock, class Duration >
    void lock_until(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2,MutexType3& m3,MutexType4& m4,MutexType5& m5)
    {
        return ((int)detail::lock_until_internal(abs_time,m1,m2,m3,m4,m5))-1;
    }
    

    namespace detail {

        template<typename Iterator, class Clock, class Duration >
        Iterator lock_until_impl(const chrono::time_point<Clock, Duration>& abs_time, Iterator begin,Iterator end,is_mutex_type_wrapper<false>)

        {
            if(begin==end)
            {
                return end;
            }
            typedef typename std::iterator_traits<Iterator>::value_type lock_type;
            unique_lock<lock_type> guard(*begin,abs_time);
            
            if(!guard.owns_lock())
            {
                return begin;
            }
            Iterator const failed=lock_until(abs_time,++begin,end);
            if(failed==end)
            {
                guard.release();
            }
            
            return failed;
        }
    }
    
}}}

#include <boost/config/abi_suffix.hpp>

#endif

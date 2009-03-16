// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams

#ifndef BOOST_SYNCHRO_LOCKABLES_TRY_LOCK_FOR__HPP
#define BOOST_SYNCHRO_LOCKABLES_TRY_LOCK_FOR__HPP

#include <algorithm>
#include <iterator>

#include <boost/thread/locks.hpp>
#include <boost/synchro/lockables/try_lock_until.hpp>
#include <boost/synchro/lockables/detail/is_mutex_type_wrapper.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockables {
    
    template<typename MutexType1,typename MutexType2, class Rep, class Period >
    typename result_of::try_lock_until<MutexType1>::type try_lock_for(const chrono::duration<Rep, Period>& rel_time, MutexType1& m1,MutexType2& m2)
    {
        return detail::try_lock_until_impl(chrono::monotonic_clock::now()+rel_time,m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2, class Rep, class Period >
    typename result_of::try_lock_until<MutexType1>::type try_lock_for(const chrono::duration<Rep, Period>& rel_time, const MutexType1& m1,MutexType2& m2)
    {
        return detail::try_lock_until_impl(chrono::monotonic_clock::now()+rel_time,m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2, class Rep, class Period >
    typename result_of::try_lock_until<MutexType1>::type try_lock_for(const chrono::duration<Rep, Period>& rel_time, MutexType1& m1,const MutexType2& m2)
    {
        return detail::try_lock_until_impl(chrono::monotonic_clock::now()+rel_time,m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2, class Rep, class Period >
    typename result_of::try_lock_until<MutexType1>::type try_lock_for(const chrono::duration<Rep, Period>& rel_time, const MutexType1& m1,const MutexType2& m2)
    {
        return detail::try_lock_until_impl(chrono::monotonic_clock::now()+rel_time,m1,m2,detail::is_mutex_type_wrapper<is_mutex_type<MutexType1>::value>());
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3, class Rep, class Period >
    int try_lock_for(const chrono::duration<Rep, Period>& rel_time, MutexType1& m1,MutexType2& m2,MutexType3& m3)
    {
        return ((int)detail::try_lock_until_internal(chrono::monotonic_clock::now()+rel_time,m1,m2,m3))-1;
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3,typename MutexType4, class Rep, class Period >
    int try_lock_for(const chrono::duration<Rep, Period>& rel_time, MutexType1& m1,MutexType2& m2,MutexType3& m3,MutexType4& m4)
    {
        return ((int)detail::try_lock_until_internal(chrono::monotonic_clock::now()+rel_time,m1,m2,m3,m4))-1;
    }

    template<typename MutexType1,typename MutexType2,typename MutexType3,typename MutexType4,typename MutexType5, class Rep, class Period >
    int try_lock_for(const chrono::duration<Rep, Period>& rel_time, MutexType1& m1,MutexType2& m2,MutexType3& m3,MutexType4& m4,MutexType5& m5)
    {
        return ((int)detail::try_lock_until_internal(chrono::monotonic_clock::now()+rel_time,m1,m2,m3,m4,m5))-1;
    }
        
}}}

#include <boost/config/abi_suffix.hpp>

#endif

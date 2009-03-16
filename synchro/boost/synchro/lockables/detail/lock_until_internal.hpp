// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams

#ifndef BOOST_SYNCHRO_LOCKABLES_LOCK_UNTIL_INTERNAL__HPP
#define BOOST_SYNCHRO_LOCKABLES_LOCK_UNTIL_INTERNAL__HPP

#include <boost/synchro/lockable/lock_until.hpp>
#include <boost/synchro/lockers.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockables { namespace detail {
        
        template<typename MutexType1,typename MutexType2, class Clock, class Duration>
        void lock_until_internal(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2)
        {
            unique_locker<MutexType1> l1(abs_time,m1);
            lockable::lock_until(m2,abs_time);
            l1.release();
        }

        template<typename MutexType1,typename MutexType2,typename MutexType3, class Clock, class Duration>
        void lock_until_internal(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2,MutexType3& m3)
        {
            unique_locker<MutexType1> l1(abs_time,m1);
            unique_locker<MutexType1> l2(abs_time,m2);
            lockable::lock_until(m3,abs_time);
            l1.release();
            l2.release();
        }


        template<typename MutexType1,typename MutexType2,typename MutexType3,
                 typename MutexType4, class Clock, class Duration>
        void lock_until_internal(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2,MutexType3& m3,
                                   MutexType4& m4)
        {
            unique_locker<MutexType1> l1(abs_time,m1);
            unique_locker<MutexType1> l2(abs_time,m2);
            unique_locker<MutexType1> l3(abs_time,m3);
            lockable::lock_until(m4,abs_time);
            l1.release();
            l2.release();
            l3.release();
        }

        template<typename MutexType1,typename MutexType2,typename MutexType3,
                 typename MutexType4,typename MutexType5, class Clock, class Duration>
        void lock_until_internal(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2,MutexType3& m3,
                                   MutexType4& m4,MutexType5& m5)
        {
            unique_locker<MutexType1> l1(abs_time,m1);
            unique_locker<MutexType1> l2(abs_time,m2);
            unique_locker<MutexType1> l3(abs_time,m3);
            unique_locker<MutexType1> l4(abs_time,m4);
            lockable::lock_until(m5,abs_time);
            l1.release();
            l2.release();
            l3.release();
            l4.release();
        }
}}}}

#include <boost/config/abi_suffix.hpp>

#endif

// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2007 Anthony Williams

#ifndef BOOST_SYNCHRO_LOCKABLES_TRY_LOCK_UNTIL_INTERNAL__HPP
#define BOOST_SYNCHRO_LOCKABLES_TRY_LOCK_UNTIL_INTERNAL__HPP
#include <boost/synchro/lockable/try_lock_until.hpp>

#include <boost/config/abi_prefix.hpp>

namespace boost { namespace synchro { namespace lockables { namespace detail {
        
        template<typename MutexType1,typename MutexType2, class Clock, class Duration>
        unsigned try_lock_until_internal(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2)
        {
            unique_locker<MutexType1> l1(m1,abs_time);
            if(!l1)
            {
                return 1;
            }
            if(!lockable::try_lock_until(m2,abs_time))
            {
                return 2;
            }
            l1.release();
            return 0;
        }

        template<typename MutexType1,typename MutexType2,typename MutexType3, class Clock, class Duration>
        unsigned try_lock_until_internal(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2,MutexType3& m3)
        {
            unique_locker<MutexType1> l1(m1,abs_time);
            if(!l1)
            {
                return 1;
            }
            if(unsigned const failed_lock=try_lock_until_internal(abs_time, m2,m3))
            {
                return failed_lock+1;
            }
            l1.release();
            return 0;
        }


        template<typename MutexType1,typename MutexType2,typename MutexType3,
                 typename MutexType4, class Clock, class Duration>
        unsigned try_lock_until_internal(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2,MutexType3& m3,
                                   MutexType4& m4)
        {
            unique_locker<MutexType1> l1(m1,abs_time);
            if(!l1)
            {
                return 1;
            }
            if(unsigned const failed_lock=try_lock_until_internal(abs_time,m2,m3,m4))
            {
                return failed_lock+1;
            }
            l1.release();
            return 0;
        }

        template<typename MutexType1,typename MutexType2,typename MutexType3,
                 typename MutexType4,typename MutexType5, class Clock, class Duration>
        unsigned try_lock_until_internal(const chrono::time_point<Clock, Duration>& abs_time, MutexType1& m1,MutexType2& m2,MutexType3& m3,
                                   MutexType4& m4,MutexType5& m5)
        {
            unique_locker<MutexType1> l1(m1,abs_time);
            if(!l1)
            {
                return 1;
            }
            if(unsigned const failed_lock=try_lock_until_internal(abs_time, m2,m3,m4,m5))
            {
                return failed_lock+1;
            }
            l1.release();
            return 0;
        }

}}}}

#include <boost/config/abi_suffix.hpp>

#endif

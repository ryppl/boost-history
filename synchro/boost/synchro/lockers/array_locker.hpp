//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008-2009. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_ARRAY_LOCKER__HPP
#define BOOST_SYNCHRO_ARRAY_LOCKER__HPP

#include <boost/mpl/bool.hpp>
#include <boost/noncopyable.hpp>
#include <boost/concept_check.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/locker_concepts.hpp>
#include <boost/synchro/lockable/functions.hpp>
#include <boost/synchro/lockables/functions.hpp>
#include <boost/synchro/lockers.hpp>
#include <boost/synchro/thread/locks.hpp>
#include <boost/chrono/chrono.hpp>

namespace boost { namespace synchro {

/*
 *
 * { 
 *   unique_array_locker<shared_lock<mutex>, 3> lock(m1, m2, m3);
 *      // ... some code needing lock on m1, m2, m3
 * } 
 */

    template <typename Lockable, unsigned N>
    class unique_array_locker;

    template <typename Lockable>
    class unique_array_locker<Lockable,2> {
    public:
        typedef Lockable lockable_type;
        BOOST_NON_CONST_COPY_CONSTRUCTOR_DELETE(unique_array_locker) /*< disable copy construction >*/
        BOOST_NON_CONST_COPY_ASSIGNEMENT_DELETE(unique_array_locker) /*< disable copy asignement >*/

        inline unique_array_locker(Lockable& m0, Lockable& m1)  
            : mtx0_(m0)
            , mtx1_(m1)
            , locked_(false)  
        {
            lock();
        }
        inline unique_array_locker(Lockable& m0, Lockable& m1, adopt_lock_t)  
            : mtx0_(m0)
            , mtx1_(m1)
            , locked_(true)  
        {}
        inline unique_array_locker(Lockable& m0, Lockable& m1, defer_lock_t)  
            : mtx0_(m0)
            , mtx1_(m1)
            , locked_(false)  
        {}
        inline unique_array_locker(Lockable& m0, Lockable& m1, try_to_lock_t)  
            : mtx0_(m0)
            , mtx1_(m1)
            , locked_(lockables::try_lock(mtx0_,mtx1_)==-1)
        {}
        ~unique_array_locker() {
            if (locked_) unlock();
        }

        template<class Clock, class Duration >
        unique_array_locker(Lockable& m0, Lockable& m1, chrono::time_point<Clock, Duration> const& target_time)
            : mtx0_(m0)
            , mtx1_(m1)
            , locked_(lockables::try_lock_until(target_time,mtx0_,mtx1_)==-1)
        {}
        template<class Rep, class Period >
        unique_array_locker(Lockable& m0, Lockable& m1, chrono::duration<Rep, Period> const& target_time)
            : mtx0_(m0)
            , mtx1_(m1)
            , locked_(lockables::try_lock_for(target_time,mtx0_,mtx1_)==-1)
        {}
            
        template<class Clock, class Duration >
        unique_array_locker(Lockable& m0, Lockable& m1, chrono::time_point<Clock, Duration> const& target_time, throw_timeout_t)
            : mtx0_(m0)
            , mtx1_(m1)
            , locked_(false)
        {
            lock_until(target_time);
        }
        template<class Rep, class Period >
        unique_array_locker(Lockable& m0, Lockable& m1, chrono::duration<Rep, Period> const& target_time, throw_timeout_t)
            : mtx0_(m0)
            , mtx1_(m1)
            , locked_(false)
        {
            lock_until(target_time);
        }
        
        template<class Clock, class Duration >
        unique_array_locker(chrono::time_point<Clock, Duration> const& target_time, Lockable& m0, Lockable& m1)
            : mtx0_(m0)
            , mtx1_(m1)
            , locked_(false)
        {
            lock_until(target_time);
        }
        template<class Rep, class Period >
        unique_array_locker(chrono::duration<Rep, Period> const& target_time, Lockable& m0, Lockable& m1)
            : mtx0_(m0)
            , mtx1_(m1)
            , locked_(false)
        {
            lock_for(target_time);
        }

        template<class Clock, class Duration >
        unique_array_locker(nothrow_timeout_t, Lockable& m0, Lockable& m1, chrono::time_point<Clock, Duration> const& target_time)
            : mtx0_(m0)
            , mtx1_(m1)
            , locked_(lockables::try_lock_until(target_time,mtx0_,mtx1_)==-1)
        {
        }
        template<class Rep, class Period >
        unique_array_locker(nothrow_timeout_t, Lockable& m0, Lockable& m1, chrono::duration<Rep, Period> const& target_time)
            : mtx0_(m0)
            , mtx1_(m1)
            , locked_(lockables::try_lock_for(target_time,mtx0_,mtx1_)==-1)
        {
        }
        bool is_locking(lockable_type* l) const {
            return locked_&& ((mtx0_==l)|| (mtx1_==l));
        }
        bool owns_lock() const {
            return locked_;
        }
        typedef bool (unique_array_locker::*bool_type)() const; /*< safe bool idiom >*/
        operator bool_type() const { return (owns_lock()?&unique_array_locker::owns_lock:0);  }
        bool operator!() const { return !owns_lock(); } 

        void lock() {
            lockables::lock(mtx0_,mtx1_);
            locked_=true;
        }
        
        void unlock() {
            lockable::unlock(mtx0_);
            lockable::unlock(mtx1_);
            locked_=false;
        }
        bool try_lock()
        {
            locked_=(lockables::try_lock(mtx0_,mtx1_)==-1);
            return locked_;
        }
        template<class Rep, class Period >
        bool try_lock_for(chrono::duration<Rep, Period> const& relative_time)
        {
            locked_=(lockables::try_lock_for(relative_time,mtx0_,mtx1_)==-1);
            return locked_;
        }

        template<class Clock, class Duration >
        bool try_lock_until(chrono::time_point<Clock, Duration> const& absolute_time)
        {
            locked_=(lockables::try_lock_until(absolute_time,mtx0_,mtx1_)==-1);
            return locked_;
        }

        template<class Rep, class Period >
        void lock_for(chrono::duration<Rep, Period> const& relative_time)
        {
            if(!try_lock_for(relative_time)) throw timeout_exception();
        }

        template<class Clock, class Duration >
        void lock_until(chrono::time_point<Clock, Duration> const& absolute_time)
        {
            if(!try_lock_until(absolute_time)) throw timeout_exception();
        }
       
    protected:
        lockable_type& mtx0_;
        lockable_type& mtx1_;
        bool locked_;
    };
    
    template <typename Lockable>
    class unique_array_locker<Lockable,3> {
    public:
        typedef Lockable lockable_type;
        BOOST_NON_CONST_COPY_CONSTRUCTOR_DELETE(unique_array_locker) /*< disable copy construction >*/
        BOOST_NON_CONST_COPY_ASSIGNEMENT_DELETE(unique_array_locker) /*< disable copy asignement >*/

        inline unique_array_locker(Lockable& m0, Lockable& m1, Lockable& m2)  
            : mtx0_(m0)
            , mtx1_(m1)
            , mtx2_(m2)
            , locked_(false)  
        {
            lock();
        }
        inline unique_array_locker(Lockable& m0, Lockable& m1, Lockable& m2, adopt_lock_t)  
            : mtx0_(m0)
            , mtx1_(m1)
            , mtx2_(m2)
            , locked_(true)  
        {}
        inline unique_array_locker(Lockable& m0, Lockable& m1, Lockable& m2, defer_lock_t)  
            : mtx0_(m0)
            , mtx1_(m1)
            , mtx2_(m2)
            , locked_(false)  
        {}
        inline unique_array_locker(Lockable& m0, Lockable& m1, Lockable& m2, try_to_lock_t)  
            : mtx0_(m0)
            , mtx1_(m1)
            , mtx2_(m2)
            , locked_(lockables::try_lock(mtx0_,mtx1_,mtx2_)==-1)
        {}
        ~unique_array_locker() {
            if (locked_) unlock();
        }

        template<class Clock, class Duration >
        unique_array_locker(Lockable& m0, Lockable& m1, Lockable& m2, chrono::time_point<Clock, Duration> const& target_time)
            : mtx0_(m0)
            , mtx1_(m1)
            , mtx2_(m2)
            , locked_(lockables::try_lock_until(target_time,mtx0_,mtx1_,mtx2_)==-1)
        {}
        template<class Rep, class Period >
        unique_array_locker(Lockable& m0, Lockable& m1, Lockable& m2, chrono::duration<Rep, Period> const& target_time)
            : mtx0_(m0)
            , mtx1_(m1)
            , mtx2_(m2)
            , locked_(lockables::try_lock_for(target_time,mtx0_,mtx1_,mtx2_)==-1)
        {}
            
        template<class Clock, class Duration >
        unique_array_locker(Lockable& m0, Lockable& m1, Lockable& m2, chrono::time_point<Clock, Duration> const& target_time, throw_timeout_t)
            : mtx0_(m0)
            , mtx1_(m1)
            , mtx2_(m2)
            , locked_(false)
        {
            lock_until(target_time);
        }
        template<class Rep, class Period >
        unique_array_locker(Lockable& m0, Lockable& m1, Lockable& m2, chrono::duration<Rep, Period> const& target_time, throw_timeout_t)
            : mtx0_(m0)
            , mtx1_(m1)
            , mtx2_(m2)
            , locked_(false)
        {
            lock_until(target_time);
        }
        
        template<class Clock, class Duration >
        unique_array_locker(nothrow_timeout_t, Lockable& m0, Lockable& m1, Lockable& m2, chrono::time_point<Clock, Duration> const& target_time)
            : mtx0_(m0)
            , mtx1_(m1)
            , mtx2_(m2)
            , locked_(lockables::try_lock_until(target_time,mtx0_,mtx1_,mtx2_)==-1)
        {
        }
        template<class Rep, class Period >
        unique_array_locker(nothrow_timeout_t, Lockable& m0, Lockable& m1, Lockable& m2, chrono::duration<Rep, Period> const& target_time)
            : mtx0_(m0)
            , mtx1_(m1)
            , mtx2_(m2)
            , locked_(lockables::try_lock_for(target_time,mtx0_,mtx1_,mtx2_)==-1)
        {
        }

        template<class Clock, class Duration >
        unique_array_locker(chrono::time_point<Clock, Duration> const& target_time, Lockable& m0, Lockable& m1, Lockable& m2)
            : mtx0_(m0)
            , mtx1_(m1)
            , mtx2_(m2)
            , locked_(false)
        {
            lock_until(target_time);
        }
        template<class Rep, class Period >
        unique_array_locker(chrono::duration<Rep, Period> const& target_time, Lockable& m0, Lockable& m1, Lockable& m2)
            : mtx0_(m0)
            , mtx1_(m1)
            , mtx2_(m2)
            , locked_(false)
        {
            lock_for(target_time);
        }

        bool is_locking(lockable_type* l) const {
            return locked_&& ((mtx0_==l)|| (mtx1_==l)|| (mtx2_==l));
        }
        bool owns_lock() const {
            return locked_;
        }
        
        
        typedef bool (unique_array_locker::*bool_type)() const; /*< safe bool idiom >*/
        operator bool_type() const { return (owns_lock()?&unique_array_locker::owns_lock:0);  }
        bool operator!() const { return !owns_lock(); } 
        
        void lock() {
            lockables::lock(mtx0_,mtx1_,mtx2_);
            locked_=true;
        }
        
        void unlock() {
            lockable::unlock(mtx0_);
            lockable::unlock(mtx1_);
            lockable::unlock(mtx2_);
            locked_=false;
        }
        bool try_lock()
        {
            locked_=(lockables::try_lock(mtx0_,mtx1_,mtx2_)==-1);
            return locked_;
        }
        template<class Rep, class Period >
        bool try_lock_for(chrono::duration<Rep, Period> const& relative_time)
        {
            locked_=(lockables::try_lock_for(relative_time,mtx0_,mtx1_,mtx2_)==-1);
            return locked_;
        }

        template<class Clock, class Duration >
        bool try_lock_until(chrono::time_point<Clock, Duration> const& absolute_time)
        {
            locked_=(lockables::try_lock_until(absolute_time,mtx0_,mtx1_,mtx2_)==-1);
            return locked_;
        }

        template<class Rep, class Period >
        void lock_for(chrono::duration<Rep, Period> const& relative_time)
        {
            if(!try_lock_for(relative_time)) throw timeout_exception();
        }

        template<class Clock, class Duration >
        void lock_until(chrono::time_point<Clock, Duration> const& absolute_time)
        {
            if(!try_lock_until(absolute_time)) throw timeout_exception();
        }
       
    protected:
        lockable_type& mtx0_;
        lockable_type& mtx1_;
        lockable_type& mtx2_;
        bool locked_;
    };
    
#if 0    
    template <typename Loker, unsigned N>
    class try_unique_array_locker;

    template <typename Loker, 2>
    class try_unique_array_locker {
    public:
        inline try_array_loker(lockable_type& m0, lockable_type& m1)  
        :  mtx0_(m0)
        ,  mtx1_(m1)
        , locked_(lockable::try_lock(mtx0_,mtx1_)))
        {}
        ~try_unique_array_locker() {
            unlock();
        }
    
    protected:
        void unlock() {
            lockable::unlock(mtx0_);
            lockable::unlock(mtx1_);
        }

        lockable_type mtx0_;
        lockable_type mtx1_;
    };

    template <typename Loker, 3>
    class try_unique_array_locker {
    public:
        inline try_unique_array_locker(lockable_type& m0, lockable_type& m1, lockable_type& m2)  
        :  mtx0_(m0)
        ,  mtx1_(m1)
        ,  mtx2_(m2)
        , locked_(lockable::try_lock(mtx0_,mtx1_,mtx2_))
        {}
        ~try_unique_array_locker() {
            unlock();
        }
    
    protected:
        void unlock() {
            lockable::unlock(mtx0_);
            lockable::unlock(mtx1_);
            lockable::unlock(mtx2_);
        }

        lockable_type mtx0_;
        lockable_type mtx1_;
        lockable_type mtx2_;
    };

    template <typename Loker, 4>
    class try_unique_array_locker {
    public:
        inline try_array_loker(
                lockable_type& m0
            , lockable_type& m1
            , lockable_type& m2
            , lockable_type& m3)  
        :  mtx0_(m0)
        ,  mtx1_(m1)
        ,  mtx2_(m2)
        ,  mtx3_(m2)
        , locked_(lockable::try_lock(mtx0_,mtx1_,mtx2_,mtx3_))
        {}
        ~try_array_loker() {
            unlock();
        }
    
    protected:
        void unlock() {
            lockable::unlock(mtx0_);
            lockable::unlock(mtx1_);
            lockable::unlock(mtx2_);
            lockable::unlock(mtx3_);
        }

        lockable_type mtx0_;
        lockable_type mtx1_;
        lockable_type mtx2_;
        lockable_type mtx3_;
    };
#endif    
}
}

#endif

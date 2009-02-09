//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_CONDITION_LOCKER__HPP
#define BOOST_SYNCHRO_CONDITION_LOCKER__HPP

#include <boost/synchro/lockable_concepts.hpp>
#include <boost/thread/condition.hpp>
#include "boost/synchro/thread/mutex.hpp"

namespace boost { namespace synchro {

/* Write-Priority Read/Write Locks

A write-priority read/write lock provides multiple threads with simultaneous
read-only access to a protected resource, and a single thread with write access
to the resource while excluding reads. When a writer releases a lock, other
waiting writers will get the lock before any waiting reader. Write-priority
read/write locks are usually used to protect resources that are more often
read than written.

A write-priority read/write lock has the rwlock_t data type. It must be
initialized by the rwlock_init routine. The rwlock_lock_read routine locks the
lock for a reader (multiple readers are allowed), the rwlock_unlock_read
routine unlocks it. The rwlock_lock_write routine locks the lock for a writer,
the rwlock_unlock_write routine unlocks it. The proper unlocking routine (for
the reader or for the writer) must be called.

In the following example, the lock owner is not checked. As a result, any
thread can unlock any lock. Routines, such as the pthread_mutex_trylock
subroutine, are missing and error handling is not performed, but cancelations
are properly handled with cleanup handlers whenever required.

*/

struct pririty_write_mutex {
    struct state_data
    {
        int lock_count;
        bool exclusive;
        bool upgrade;
        bool exclusive_waiting_blocked;
        int waiting_writers;
    };



    state_data state;
    boost::mutex state_change;
    boost::condition_variable shared_cond;
    boost::condition_variable exclusive_cond;
    boost::condition_variable upgrade_cond;

    void release_waiters()
    {
        exclusive_cond.notify_one();
        shared_cond.notify_all();
    }
    int lock_count;  /* < 0 .. held by writer             */
                         /* > 0 .. held by lock_count readers */
                         /* = 0 .. held by nobody             */
        int waiting_writers;  /* count of wating writers      */
public:
    shared_mutex()
    {
        state_data state_={0,0,0,0};
        state=state_;
    }

    ~shared_mutex()
    {
    }

    void lock_shared()
    {
        boost::this_thread::disable_interruption do_not_disturb;
        boost::mutex::scoped_lock lock(state_change);

        while(state.exclusive || state.exclusive_waiting_blocked)
        {
            shared_cond.wait(lock);
        }
        ++state.shared_count;
    }

    bool try_lock_shared()
    {
        boost::mutex::scoped_lock lock(state_change);

        if(state.exclusive || state.exclusive_waiting_blocked)
        {
            return false;
        }
        else
        {
            ++state.shared_count;
            return true;
        }
    }

    bool timed_lock_shared(system_time const& timeout)
    {
        boost::this_thread::disable_interruption do_not_disturb;
        boost::mutex::scoped_lock lock(state_change);

        while(state.exclusive || state.exclusive_waiting_blocked)
        {
            if(!shared_cond.timed_wait(lock,timeout))
            {
                return false;
            }
        }
        ++state.shared_count;
        return true;
    }

    template<typename TimeDuration>
    bool timed_lock_shared(TimeDuration const & relative_time)
    {
        return timed_lock_shared(get_system_time()+relative_time);
    }

    void unlock_shared()
    {
        boost::mutex::scoped_lock lock(state_change);
        bool const last_reader=!--state.shared_count;

        if(last_reader)
        {
            if(state.upgrade)
            {
                state.upgrade=false;
                state.exclusive=true;
                upgrade_cond.notify_one();
            }
            else
            {
                state.exclusive_waiting_blocked=false;
            }
            release_waiters();
        }
    }

    void lock()
    {
        boost::this_thread::disable_interruption do_not_disturb;
        boost::mutex::scoped_lock lock(state_change);

        while(state.shared_count || state.exclusive)
        {
            state.exclusive_waiting_blocked=true;
            exclusive_cond.wait(lock);
        }
        state.exclusive=true;
    }

    bool timed_lock(system_time const& timeout)
    {
        boost::this_thread::disable_interruption do_not_disturb;
        boost::mutex::scoped_lock lock(state_change);

        while(state.shared_count || state.exclusive)
        {
            state.exclusive_waiting_blocked=true;
            if(!exclusive_cond.timed_wait(lock,timeout))
            {
                if(state.shared_count || state.exclusive)
                {
                    state.exclusive_waiting_blocked=false;
                    exclusive_cond.notify_one();
                    return false;
                }
                break;
            }
        }
        state.exclusive=true;
        return true;
    }

    template<typename TimeDuration>
    bool timed_lock(TimeDuration const & relative_time)
    {
        return timed_lock(get_system_time()+relative_time);
    }

    bool try_lock()
    {
        boost::mutex::scoped_lock lock(state_change);

        if(state.shared_count || state.exclusive)
        {
            return false;
        }
        else
        {
            state.exclusive=true;
            return true;
        }

    }

    void unlock()
    {
        boost::mutex::scoped_lock lock(state_change);
        state.exclusive=false;
        state.exclusive_waiting_blocked=false;
        release_waiters();
    }

    void lock_upgrade()
    {
        boost::this_thread::disable_interruption do_not_disturb;
        boost::mutex::scoped_lock lock(state_change);
        while(state.exclusive || state.exclusive_waiting_blocked || state.upgrade)
        {
            shared_cond.wait(lock);
        }
        ++state.shared_count;
        state.upgrade=true;
    }

    bool timed_lock_upgrade(system_time const& timeout)
    {
        boost::this_thread::disable_interruption do_not_disturb;
        boost::mutex::scoped_lock lock(state_change);
        while(state.exclusive || state.exclusive_waiting_blocked || state.upgrade)
        {
            if(!shared_cond.timed_wait(lock,timeout))
            {
                if(state.exclusive || state.exclusive_waiting_blocked || state.upgrade)
                {
                    return false;
                }
                break;
            }
        }
        ++state.shared_count;
        state.upgrade=true;
        return true;
    }

    template<typename TimeDuration>
    bool timed_lock_upgrade(TimeDuration const & relative_time)
    {
        return timed_lock(get_system_time()+relative_time);
    }

    bool try_lock_upgrade()
    {
        boost::mutex::scoped_lock lock(state_change);
        if(state.exclusive || state.exclusive_waiting_blocked || state.upgrade)
        {
            return false;
        }
        else
        {
            ++state.shared_count;
            state.upgrade=true;
            return true;
        }
    }

    void unlock_upgrade()
    {
        boost::mutex::scoped_lock lock(state_change);
        state.upgrade=false;
        bool const last_reader=!--state.shared_count;

        if(last_reader)
        {
            state.exclusive_waiting_blocked=false;
            release_waiters();
        }
    }

    void unlock_upgrade_and_lock()
    {
        boost::this_thread::disable_interruption do_not_disturb;
        boost::mutex::scoped_lock lock(state_change);
        --state.shared_count;
        while(state.shared_count)
        {
            upgrade_cond.wait(lock);
        }
        state.upgrade=false;
        state.exclusive=true;
    }

    void unlock_and_lock_upgrade()
    {
        boost::mutex::scoped_lock lock(state_change);
        state.exclusive=false;
        state.upgrade=true;
        ++state.shared_count;
        state.exclusive_waiting_blocked=false;
        release_waiters();
    }

    void unlock_and_lock_shared()
    {
        boost::mutex::scoped_lock lock(state_change);
        state.exclusive=false;
        ++state.shared_count;
        state.exclusive_waiting_blocked=false;
        release_waiters();
    }

    void unlock_upgrade_and_lock_shared()
    {
        boost::mutex::scoped_lock lock(state_change);
        state.upgrade=false;
        state.exclusive_waiting_blocked=false;
        release_waiters();
    }
};
void rwlock_init(rwlock_t *rwl)
{
}

void waiting_reader_cleanup(void *arg)
{
        rwlock_t *rwl;

        rwl = (rwlock_t *)arg;
        pthread_mutex_unlock(&rwl->lock);
}

void rwlock_lock_read(rwlock_t *rwl)
{
        pthread_mutex_lock(&rwl->lock);
        pthread_cleanup_push(waiting_reader_cleanup, rwl);
        while ((rwl->lock_count < 0) && (rwl->waiting_writers))
                pthread_cond_wait(&rwl->rcond, &rwl->lock);
        rwl->lock_count++;
        /*
         *  Note that the pthread_cleanup_pop subroutine will
         *  execute the waiting_reader_cleanup routine
         */
        pthread_cleanup_pop(1);
}

void rwlock_unlock_read(rwlock_t *rwl)
{
        pthread_mutex_lock(&rwl->lock);
        rwl->lock_count--;
        if (!rwl->lock_count)
                pthread_cond_signal(&rwl->wcond);
        pthread_mutex_unlock(&rwl->lock);
}

void waiting_writer_cleanup(void *arg)
{
        rwlock_t *rwl;

        rwl = (rwlock_t *)arg;
        rwl->waiting_writers--;
        if ((!rwl->waiting_writers) && (rwl->lock_count >= 0))
                /*
                         * This only happens if we have been canceled
                         */
                        pthread_cond_broadcast(&rwl->wcond);
                        pthread_mutex_unlock(&rwl->lock);
}

void rwlock_lock_write(rwlock_t *rwl)
{
        pthread_mutex_lock(&rwl->lock);
        rwl->waiting_writers++;
        pthread_cleanup_push(waiting_writer_cleanup, rwl);
        while (rwl->lock_count)
                pthread_cond_wait(&rwl->wcond, &rwl->lock);
        rwl->lock_count = -1;
        /*
         *  Note that the pthread_cleanup_pop subroutine will
         *  execute the waiting_writer_cleanup routine
         */
        pthread_cleanup_pop(1);
}

void rwlock_unlock_write(rwlock_t *rwl)
{
        pthread_mutex_lock(&rwl->lock);
        l->lock_count = 0;
        if (!rwl->wating_writers)
                pthread_cond_broadcast(&rwl->rcond);
        else
                pthread_cond_signal(&rwl->wcond);
        pthread_mutex_unlock(&rwl->lock);
}
};

/*
 * Readers are counted only. When the count reaches zero, a waiting writer may
 * take the lock. Only one writer can hold the lock. When the lock is released
 * by a writer, another writer is awakened, if there is one. Otherwise, all
 * waiting readers are awakened.
 *
 * The locking routines are cancelable, because they call the
 * pthread_cond_wait subroutine. Cleanup handlers are therefore registered
 * before calling the subroutine.
 */
}
}
#endif

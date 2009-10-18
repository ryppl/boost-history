//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Justin E. Gottchlich 2009.
// (C) Copyright Vicente J. Botet Escriba 2009.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/stm for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHO_TSS_HPP
#define BOOST_SYNCHO_TSS_HPP

//-----------------------------------------------------------------------------
#include "pthread.h"
//-----------------------------------------------------------------------------
#include <boost/synchro/lockers/lock_guard.hpp>
#include <boost/synchro/pthread/mutex.hpp>
//-----------------------------------------------------------------------------

namespace boost { namespace synchro {

    template <typename T>
    class implicit_thread_specific_ptr
    {
    private:
        implicit_thread_specific_ptr(implicit_thread_specific_ptr&);
        implicit_thread_specific_ptr& operator=(implicit_thread_specific_ptr&);

        typedef pthread_mutex_t mutex_type;
        // Key that uniquely identifies the 'logically global' object that
        // 'physically' resides locally in thread-specific storage.
        mutable pthread_key_t key_;

        // "First time in" flag
        mutable bool once_;


        // Avoid race conditions during initialization.
        mutable mutex_type keylock_;

        // A static cleanup hook method that deletes dynamically allocated memory.
        static void cleanup_hook (void *ptr) {
            // This cast invokes the destructor (if one exists).
            delete (T*) ptr;
        }

    public:
        implicit_thread_specific_ptr()
            : key_(0) , once_ (false)
        {
            int const res=pthread_mutex_init(&keylock_,0);
            assert(res==0);
            //if(res) throw thread_resource_error();
        }
        ~implicit_thread_specific_ptr() {
            int const res=pthread_mutex_destroy(&keylock_);
            assert(res==0);
        }

        T* get() const {
            T *tss_data = 0;
            // Use the Double-Checked Locking Optimization pattern to avoid excessive locking.
            if (!once_) {
                // Use Scoped Locking idiom to ensure <keylock_>
                // is acquired to serialize critical section.
                synchro::lock_guard<mutex_type> guard (keylock_);
                if (!once_) {
                    pthread_key_create (&key_, &cleanup_hook);
                    // Must come last so that other threads don't use the key until it's created.
                    once_ = true;
                }
                // <Guard> destructor releases the lock.
            }
            // Get data from thread-specific storage. Note that no locks are required,
            // because this thread's own copy of the thread-specific object will be accessed.
            tss_data = static_cast<T*>(pthread_getspecific (key_));
            // Check if it's the first time in for this thread.
            if (tss_data == 0) {
                // Allocate memory dynamically off the heap,
                tss_data = new T;
                // Store pointer in thread-specific storage.
                pthread_setspecific (key_, (void *)tss_data);
            }
            return tss_data;
        }
        T* operator->() const
        {
            return get();
        }
        T& operator*() const
        {
            return *get();
        }
    };
}}
#endif // BOOST_SYNCHO_TAGS_HPP


//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2008.
// (C) Copyright Vicente J. Botet Escriba 2008-2009.
// Distributed under the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/synchro for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SYNCHRO_SEMAPHORE_HPP
#define BOOST_SYNCHRO_SEMAPHORE_HPP

#include <boost/synchro/thread_synchronization_family.hpp>
#include <boost/synchro/lockable_traits.hpp>
#include <boost/synchro/detail/deleted_functions.hpp>
#include <boost/convert_to/chrono_time_point_to_posix_time_ptime.hpp>
#include <boost/convert_to/chrono_duration_to_posix_time_duration.hpp>

//!\file
//!Describes a semaphore class for inter-process synchronization

namespace boost {

namespace synchro {

//!Wraps a semaphore that can be placed in shared memory and can be
//!shared between processes. Allows timed lock tries
template <typename ScopeTag=multi_threaded_tag>
class basic_semaphore
{
    typedef synchronization_family<ScopeTag> Sync;
    typedef typename Sync::mutex_type lockable_type;
    typedef typename Sync::condition_type condition_variable;
    typedef typename unique_lock_type<lockable_type>::type scoped_lock;
    typedef basic_semaphore this_type;

public:
    BOOST_COPY_CONSTRUCTOR_DELETE(basic_semaphore) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(basic_semaphore) /*< disable copy asignement >*/

    //!Creates a semaphore with the given initial count.
    //!exception if there is an error.*/
    inline basic_semaphore(int initialCount);

    //!Destroys the semaphore.
    //!Does not throw
    inline ~basic_semaphore();

    //!Increments the semaphore count. If there are processes/threads blocked waiting
    //!for the semaphore, then one of these processes will return successfully from
    //!its wait function. If there is an error an exception exception is thrown.
    inline void post();

    //!Decrements the semaphore. If the semaphore value is not greater than zero,
    //!then the calling process/thread blocks until it can decrement the counter.
    //!If there is an error an exception exception is thrown.
    inline void wait();

    //!Decrements the semaphore if the semaphore's value is greater than zero
    //!and returns true. If the value is not greater than zero returns false.
    //!If there is an error an exception exception is thrown.
    inline bool try_wait();

    //!Decrements the semaphore if the semaphore's value is greater
    //!than zero and returns true. Otherwise, waits for the semaphore
    //!to the posted or the timeout expires. If the timeout expires, the
    //!function returns false. If the semaphore is posted the function
    //!returns true. If there is an error throws sem_exception
    template<typename Clock, typename Duration>
    inline bool try_wait_until(chrono::time_point<Clock, Duration> const& abs_time);
    
    template<typename Rep, typename Period>
    inline bool try_wait_for(const chrono::duration<Rep, Period> &rel_time) {
        return try_wait_until(Rep::now()+rel_time);
    }
    
    template<typename Clock, typename Duration>
    inline void wait_until(chrono::time_point<Clock, Duration> const& abs_time) {
        if (!try_wait_until(abs_time)) throw timeout_exception();
    }
    
    template<typename Rep, typename Period>
    inline void wait_for(const chrono::duration<Rep, Period> &rel_time) {
        if (!try_wait_until(rel_time)) throw timeout_exception();
    }        

    /// @cond
protected:
    lockable_type       m_mut;
    condition_variable  m_cond;
    int                 m_count;
    /// @endcond
};

template <typename ScopeTag>
inline basic_semaphore<ScopeTag>::~basic_semaphore()
{}

template <typename ScopeTag>
inline basic_semaphore<ScopeTag>::basic_semaphore(int initialCount)
   :  m_mut(), m_cond(), m_count(initialCount)
{}

template <typename ScopeTag>
inline void basic_semaphore<ScopeTag>::post()
{
   scoped_lock lock(m_mut);
   if(m_count == 0){
      m_cond.notify_one();
   }
   ++m_count;
}

template <typename ScopeTag>
inline void basic_semaphore<ScopeTag>::wait()
{
   scoped_lock lock(m_mut);
   while(m_count == 0){
      m_cond.wait(lock);
   }
   --m_count;
}

template <typename ScopeTag>
inline bool basic_semaphore<ScopeTag>::try_wait()
{
   scoped_lock lock(m_mut);
   if(m_count == 0){
      return false;
   }
   --m_count;
   return true;
}

   
template<class ScopeTag> 
template<class Clock, class Duration>
inline bool basic_semaphore<ScopeTag>::try_wait_until(const chrono::time_point<Clock, Duration> &abs_time)
{
   scoped_lock lock(m_mut);
   while(m_count == 0){
      if(!m_cond.timed_wait(lock, boost::convert_to<posix_time::ptime>(abs_time)))
         return m_count != 0;
   }
   --m_count;
   return true;
}

typedef basic_semaphore<> semaphore;

template <typename ScopeTag=multi_threaded_tag>
class basic_binary_semaphore: public basic_semaphore<ScopeTag>
{
public:
    BOOST_COPY_CONSTRUCTOR_DELETE(basic_binary_semaphore) /*< disable copy construction >*/
    BOOST_COPY_ASSIGNEMENT_DELETE(basic_binary_semaphore) /*< disable copy asignement >*/

    //!Creates a semaphore with the given initial count.
    //!exception if there is an error.*/
    inline explicit basic_binary_semaphore(int initialCount)
                        :basic_semaphore<ScopeTag>(initialCount>0?1:0) {};
                            
    inline void post()
    {
        typename basic_semaphore<ScopeTag>::scoped_lock lock(this->m_mut);
        if(this->m_count == 0){
            this->m_cond.notify_one();
            ++(this->m_count);
        }
    }
};
typedef basic_binary_semaphore<> binary_semaphore;


}
}

#endif   //BOOST_SYNCHRO_SEMAPHORE_HPP

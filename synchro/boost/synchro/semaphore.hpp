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

//!\file
//!Describes a semaphore class for inter-process synchronization

namespace boost {

namespace synchro {

//!Wraps a semaphore that can be placed in shared memory and can be
//!shared between processes. Allows timed lock tries
template <typename Sync=thread_synchronization_family>
class basic_semaphore
{
    typedef typename Sync::mutex_type lockable_type;
    typedef typename Sync::condition_type condition_variable;
    typedef typename scoped_lock_type<lockable_type>::type scoped_lock;
    typedef basic_semaphore this_type;

    /// @cond
    //Non-copyable
    basic_semaphore(const this_type &);
    this_type& operator=(const this_type &);
    /// @endcond
public:

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
    inline bool wait_until(const system_time &abs_time);
    template<typename TimeDuration>
    inline bool wait_for(const TimeDuration &rel_time);

    /// @cond
private:
    lockable_type       m_mut;
    condition_variable  m_cond;
    int                 m_count;
    /// @endcond
};

template <typename Sync>
inline basic_semaphore<Sync>::~basic_semaphore()
{}

template <typename Sync>
inline basic_semaphore<Sync>::basic_semaphore(int initialCount)
   :  m_mut(), m_cond(), m_count(initialCount)
{}

template <typename Sync>
inline void basic_semaphore<Sync>::post()
{
   scoped_lock lock(m_mut);
   if(m_count == 0){
      m_cond.notify_one();
   }
   ++m_count;
}

template <typename Sync>
inline void basic_semaphore<Sync>::wait()
{
   scoped_lock lock(m_mut);
   while(m_count == 0){
      m_cond.wait(lock);
   }
   --m_count;
}

template <typename Sync>
inline bool basic_semaphore<Sync>::try_wait()
{
   scoped_lock lock(m_mut);
   if(m_count == 0){
      return false;
   }
   --m_count;
   return true;
}

template <typename Sync>
inline bool basic_semaphore<Sync>::wait_until(const system_time &abs_time)
{
   scoped_lock lock(m_mut);
   while(m_count == 0){
      if(!m_cond.timed_wait(lock, abs_time))
         return m_count != 0;
   }
   --m_count;
   return true;
}

typedef basic_semaphore<> semaphore;

}
}

#endif   //BOOST_SYNCHRO_SEMAPHORE_HPP

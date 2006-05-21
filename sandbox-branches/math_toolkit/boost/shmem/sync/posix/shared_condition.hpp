//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

namespace boost {

namespace shmem {

inline shared_condition::shared_condition()
{
   int res;
   pthread_condattr_t cond_attr;
   res = pthread_condattr_init(&cond_attr);
   if(res != 0){
      throw process_resource_exception();
   }
   res = pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
   if(res != 0){
      pthread_condattr_destroy(&cond_attr);
      throw process_resource_exception();
   }
   res = pthread_cond_init(&m_condition, &cond_attr);
   pthread_condattr_destroy(&cond_attr);
   if(res != 0){
      throw process_resource_exception();
   }
}

inline shared_condition::~shared_condition()
{
    int res = 0;
    res = pthread_cond_destroy(&m_condition);
    assert(res == 0);
}

inline void shared_condition::notify_one()
{
    int res = 0;
    res = pthread_cond_signal(&m_condition);
    assert(res == 0);
}

inline void shared_condition::notify_all()
{
    int res = 0;
    res = pthread_cond_broadcast(&m_condition);
    assert(res == 0);
}

template <class SharedMutex>
inline void shared_condition::do_wait(SharedMutex &mut)
{
   pthread_mutex_t* pmutex = &mut.m_mut;
   int res = 0;
   res = pthread_cond_wait(&m_condition, pmutex);
   assert(res == 0);
}

template<class SharedMutex>
inline bool shared_condition::do_timed_wait(const xtime& xt, SharedMutex &mut)
{
   timespec ts;
   xtime_to_timespec(xt, ts);

   int res = 0;
   res = pthread_cond_timedwait(&m_condition, &mut.m_mut, &ts);
   assert(res == 0 || res == ETIMEDOUT);

   return res != ETIMEDOUT;
}

}  //namespace shmem

}  // namespace boost

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////
//
// Parts of the pthread code come from Boost Threads code:
//
//////////////////////////////////////////////////////////////////////////////
//
// Copyright (C) 2001-2003
// William E. Kempf
//
// Permission to use, copy, modify, distribute and sell this software
// and its documentation for any purpose is hereby granted without fee,
// provided that the above copyright notice appear in all copies and
// that both that copyright notice and this permission notice appear
// in supporting documentation.  William E. Kempf makes no representations
// about the suitability of this software for any purpose.
// It is provided "as is" without express or implied warranty.
//////////////////////////////////////////////////////////////////////////////

namespace boost {

namespace shmem {

inline static void do_lock_func(volatile long &m_s)
{
   using namespace boost::detail;
   do{
      long prev_s = BOOST_INTERLOCKED_COMPARE_EXCHANGE((long*)&m_s, 0, 1);   

      if (m_s == 0 && prev_s == 1){
            break;
      }
      // relinquish current timeslice
      sched_yield();
   }while (true);
}

inline static bool do_trylock_func(volatile long &m_s)
{
   using namespace boost::detail;
   long prev_s = BOOST_INTERLOCKED_COMPARE_EXCHANGE((long*)&m_s, 0, 1);   

   if (m_s == 0 && prev_s == 1){
      return true;
   }
   return false;
}

inline static bool do_timedlock_func(volatile long &m_s, const xtime& xt)
{
   //Obtain current count and target time
   unsigned long sleeptm  = (unsigned long)(xt.sec*1000+xt.nsec/(1000*1000));
   xtime xnow;
   xtime_get(&xnow, TIME_UTC);
   unsigned long nowtime  = (unsigned long)(xnow.sec*1000+xnow.nsec/(1000*1000));
   using namespace boost::detail;

   if(nowtime >= sleeptm) return false;

   do{
      long prev_s = BOOST_INTERLOCKED_COMPARE_EXCHANGE((long*)&m_s, 0, 1);   

      if (m_s == 0 && prev_s == 1){
         break;
      }
      xtime_get(&xnow, TIME_UTC);
      nowtime    = (unsigned long)(xnow.sec*1000+xnow.nsec/(1000*1000));

      if(nowtime >= sleeptm){
         return false;
      }
      // relinquish current time slice
      sched_yield();
   }while (true);

   return true;
}

inline static void do_unlock_func(volatile long &m_s)
{
   using namespace boost::detail;
   BOOST_INTERLOCKED_COMPARE_EXCHANGE((long*)&m_s, 1, 0);
}

inline shared_mutex::shared_mutex() 
   : m_s(1) 
{}

inline shared_mutex::~shared_mutex() 
{}

inline void shared_mutex::do_lock(void)
{
   do_lock_func(m_s);
}

inline void shared_mutex::do_unlock(void)
{
   do_unlock_func(m_s);
}

inline shared_try_mutex::shared_try_mutex() 
   : m_s(1) 
{}

inline shared_try_mutex::~shared_try_mutex() 
{}

inline void shared_try_mutex::do_lock(void)
{
   do_lock_func(m_s);
}

inline bool shared_try_mutex::do_trylock(void)
{
   return do_trylock_func(m_s);
}

inline void shared_try_mutex::do_unlock(void)
{
   do_unlock_func(m_s);
}

//---

inline shared_timed_mutex::shared_timed_mutex() 
   : m_s(1) 
{}

inline shared_timed_mutex::~shared_timed_mutex() 
{}

inline void shared_timed_mutex::do_lock(void)
{
   do_lock_func(m_s);
}

inline bool shared_timed_mutex::do_trylock(void)
{
   return do_trylock_func(m_s);
}

inline bool shared_timed_mutex::do_timedlock(const xtime& xt)
{
   return do_timedlock_func(m_s, xt);
}

inline void shared_timed_mutex::do_unlock(void)
{
   do_unlock_func(m_s);
}

}  //namespace shmem {

}  //namespace boost {


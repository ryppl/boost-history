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
   m_command      = SLEEP;
   m_num_waiters  = 0;
}

inline shared_condition::~shared_condition()
{
}


inline void shared_condition::notify_one()
{
   notify(NOTIFY_ONE);
}

inline void shared_condition::notify_all()
{
   notify(NOTIFY_ALL);
}

inline void shared_condition::notify(long command)
{
   //This mutex guarantees that no other thread can enter to the 
   //do_timed_wait method logic, so that thread count will be
   //constant until the function writes a NOTIFY_ALL command.
   //It also guarantees that no other notification can be signaled 
   //on this condition before this one ends
   using namespace boost::detail;
   m_enter_mut.do_lock();

   //Return if there are no waiters
   if(!exchange_and_add(&m_num_waiters, 0)) { 
      m_enter_mut.do_unlock();
      return;
   }

   //Notify that all threads should execute wait logic
   BOOST_INTERLOCKED_COMPARE_EXCHANGE((long*)&m_command, command, SLEEP);
   //The enter mutex will rest locked until the last waiting thread unlocks it
}

template<class SharedMutex>
inline void shared_condition::do_wait(SharedMutex &mut)
{
   do_timed_wait(false, xtime(), mut);
}

template<class SharedMutex>
inline bool shared_condition::do_timed_wait(const xtime& xt, SharedMutex &mut)
{
   return do_timed_wait(true, xt, mut);
}

template<class SharedMutex>
inline bool shared_condition::do_timed_wait(bool tout_enabled,
                                     const xtime& abstime, 
                                     SharedMutex &mut)
{
   unsigned long sleeptm;
   xtime xnow;
   unsigned long nowtime;
   using namespace boost::detail;
   if(tout_enabled){
      //Obtain current count and target time (msec)
      sleeptm  = (unsigned long)abstime.sec*1000+abstime.nsec/(1000*1000);
      xtime_get(&xnow, TIME_UTC);
      nowtime = (unsigned long)(xnow.sec*1000+xnow.nsec/(1000*1000));

      if(nowtime >= sleeptm) return false;
   }

   //The enter mutex guarantees that while executing a notification, 
   //no other thread can execute the do_timed_wait method. 
   {
      //---------------------------------------------------------------
      boost::shmem::scoped_lock<shared_try_mutex> lock(m_enter_mut);
      //---------------------------------------------------------------
      //We increment the waiting thread count protected so that it will be
      //always constant when another thread enters the notification logic.
      //The increment marks this thread as "waiting on condition"
      BOOST_INTERLOCKED_INCREMENT((long*)&m_num_waiters);

      //We unlock the external mutex atomically with the increment
      mut.do_unlock();
   }

   //By default, we suppose that no timeout has happened
   bool timed_out  = false, unlock_enter_mut= false;
   
   //Loop until a notification indicates that the thread should 
   //exit or timeout occurs
   while(1){
      //The thread sleeps/spins until a condition commands a notification
      //Notification occurred, we will lock the checking mutex so that
      while(exchange_and_add(&m_command, 0) == SLEEP){
         sched_yield();

         //Check for timeout
         if(tout_enabled){
            xtime_get(&xnow, TIME_UTC);
            nowtime = (unsigned long)(xnow.sec*1000+xnow.nsec/(1000*1000));

            if(nowtime >= sleeptm){
               //If we can lock the mutex it means that no notification
               //is being executed in this condition variable
               timed_out = m_enter_mut.do_trylock();

               //If locking fails, indicates that another thread is executing
               //notification, so we play the notification game
               if(!timed_out){
                  //There is an ongoing notification, we will try again later
                  continue;
               }
               //No notification in execution, since enter mutex is locked. 
               //We will execute time-out logic, so we will decrement count, 
               //release the enter mutex and return false.
               break;
            }
         }
      }

      //If a timeout occurred, the mutex will not execute checking logic
      if(tout_enabled && timed_out){
         //Decrement wait count
         BOOST_INTERLOCKED_DECREMENT((long*)&m_num_waiters);
         unlock_enter_mut = true;
         break;
      }
      else{
         //Notification occurred, we will lock the checking mutex so that
         //if a notify_one notification occurs, only one thread can exit
        //---------------------------------------------------------------
         boost::shmem::scoped_lock<shared_mutex> lock(m_check_mut);
         //---------------------------------------------------------------
         long result = BOOST_INTERLOCKED_COMPARE_EXCHANGE((long*)&m_command, SLEEP, NOTIFY_ONE);
         if(result == SLEEP){
            //Other thread has been notified and since it was a NOTIFY one
            //command, this thread must sleep again
            continue;
         }
         else if(result == NOTIFY_ONE){
            //If it was a NOTIFY_ONE command, only this thread should  
            //exit. This thread has atomically marked command as sleep before
            //so no other thread will exit.
            //Decrement wait count.
            unlock_enter_mut = true;
            BOOST_INTERLOCKED_DECREMENT((long*)&m_num_waiters);
            break;
         }
         else{
            //If it is a NOTIFY_ALL command, all threads should return 
            //from do_timed_wait function. Decrement wait count. 
            unlock_enter_mut = BOOST_INTERLOCKED_DECREMENT((long*)&m_num_waiters) == 0;
            //Check if this is the last thread of notify_all waiters
            //Only the last thread will release the mutex
            if(unlock_enter_mut){
               BOOST_INTERLOCKED_COMPARE_EXCHANGE((long*)&m_command, SLEEP, NOTIFY_ALL);
            }
            break;
         }
      }
   }

   //Unlock the enter mutex if it is a single notification, if this is 
   //the last notified thread in a notify_all or a timeout has occurred
   if(unlock_enter_mut){
      m_enter_mut.do_unlock();
   }

   //Lock external again before returning from the method
   mut.do_lock();
   return !timed_out;
}

}  //namespace shmem

}  // namespace boost


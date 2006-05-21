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

inline named_semaphore::~named_semaphore()
{  
   close();
}

inline named_semaphore::named_semaphore() 
   : mp_sem(0)
{ }

inline bool named_semaphore::create(const char *name, int initialCount)
{
   if(mp_sem)
      return false;
   mp_sem = detail::create_semaphore(initialCount, name);
   if(detail::get_last_error() == detail::error_already_exists){
      detail::close_handle(mp_sem);
      return false;
   }
   return true;
}

inline bool named_semaphore::open_or_create(const char *name, int initialCount)
{
   if(mp_sem)
      return false;
   mp_sem = detail::create_semaphore(initialCount, name);

   if(!mp_sem){
      return false;
   }
   return true;
}

inline bool named_semaphore::open(const char *name)
{
   if(mp_sem)
      return false;

   mp_sem = detail::open_semaphore(name);
   return mp_sem != 0;
}

inline void named_semaphore::close()
{
   if(mp_sem){
      bool success = detail::close_handle(mp_sem) == 1;
      assert(success == true);
      mp_sem = 0;
   }
}

inline void named_semaphore::post()
{
   if(detail::release_semaphore(mp_sem, 1, 0) != 1){
      throw sem_exception();
   }
}

inline void named_semaphore::wait()
{
   if(detail::wait_for_single_object(mp_sem, detail::infinite_time) 
         != detail::wait_object_0){
      throw sem_exception();
   }
}

inline bool named_semaphore::try_wait()
{
   return detail::wait_for_single_object(mp_sem, 0) 
            == detail::wait_object_0;
}

inline bool named_semaphore::timed_wait(const xtime &xt)
{
   for (;;)
   {
      int milliseconds;
      to_duration(xt, milliseconds);

      unsigned long res = detail::wait_for_single_object(mp_sem, milliseconds);

      if (res == detail::wait_timeout)
      {
         xtime cur;
         xtime_get(&cur, TIME_UTC);
         if (xt > cur)
               continue;
      }
      return res == detail::wait_object_0;
   }
}

}  //namespace shmem {

}  //namespace boost {


//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////


/*!\file
   Describes a named semaphore class for inter-process synchronization
*/

namespace boost {

namespace shmem {

namespace detail {

   struct sem_construct_func_t
   {
      enum CreationType {  open_only, open_or_create, create_only  };

      sem_construct_func_t(sem_t *&sem)
         :  mp_sem(sem){}

      bool operator()(const shared_memory::segment_info_t * info, bool created) const
      {   
         mp_sem = reinterpret_cast<sem_t *>(info->get_user_ptr());
         switch(m_create){
            case open_only:   
               mp_sem = reinterpret_cast<sem_t *>(info->get_user_ptr());
            break;

            case open_or_create:
               if(created){
                  if(sem_init(mp_sem, 1, m_initialCount) == 0){
                     return true;
                  }
                  else{
                     mp_sem = 0;
                     return false;
                  }
               }
            break;

            case create_only:
               return true;
            break;
         
            default:
               return false;
            break;
         }
         return true;
      }

      int            m_initialCount;
      CreationType   m_create;
      sem_t *       &mp_sem; 
   };

   struct sem_destroy_func_t
   {
      sem_destroy_func_t(sem_t *&sem)
         :  mp_sem(sem){}

      void operator()(const shared_memory::segment_info_t * info, bool last) const
         {  if(last) sem_destroy(mp_sem); }

      sem_t *       &mp_sem; 
   };
}

inline named_semaphore::~named_semaphore()
{  
   close();
}

inline named_semaphore::named_semaphore() 
   : mp_sem((sem_t*)-1)
{ }

inline bool named_semaphore::create(const char *name, int initialCount)
{
   if(mp_sem != (sem_t*)-1 || initialCount > SEM_VALUE_MAX)
      return false;

   detail::sem_construct_func_t func(mp_sem);
   func.m_initialCount  = initialCount;
   func.m_create        = detail::sem_construct_func_t::create_only;

   if(!m_shmem.create_with_func(name, sizeof(sem_t), func)){
      return false;
   }

   return true;
}

inline bool named_semaphore::open_or_create(const char *name, int initialCount)
{
   if(mp_sem != (sem_t*)-1 || initialCount > SEM_VALUE_MAX)
      return false;

   detail::sem_construct_func_t func(mp_sem);
   func.m_initialCount  = initialCount;
   func.m_create        = detail::sem_construct_func_t::open_or_create;

   if(!m_shmem.open_or_create_with_func(name, sizeof(sem_t), func)){
      return false;
   }

   return true;
}

inline bool named_semaphore::open(const char *name)
{
   if(mp_sem != (sem_t*)-1)
      return false;

   detail::sem_construct_func_t func(mp_sem);
   func.m_create  = detail::sem_construct_func_t::open_only;

   if(!m_shmem.open_with_func(name, func)){
      return false;
   }

   return true;
}

inline void named_semaphore::close()
{
   if(mp_sem != (sem_t*)-1){
      m_shmem.close_with_func(detail::sem_destroy_func_t(mp_sem));
      mp_sem = (sem_t*)-1;
   }
}

inline void named_semaphore::post()
{
   if(sem_post(mp_sem) != 0){
      throw sem_exception();
   }
}

inline void named_semaphore::wait()
{
   if(sem_wait(mp_sem) != 0){
      throw sem_exception();
   }
}

inline bool named_semaphore::try_wait()
{
   int res = sem_trywait(mp_sem);
   assert(res == 0 || errno == EAGAIN);
   return res == 0;
}

inline bool named_semaphore::timed_wait(const xtime &xt)
{
   for (;;){
      timespec tspec;
      xtime_to_timespec(xt, tspec);

      int res = sem_timedwait(mp_sem, &tspec);
      assert(res == 0 || errno == ETIMEDOUT);

      if (errno == ETIMEDOUT)
         return false;
   }
   return true;
}

}  //namespace shmem {

}  //namespace boost {

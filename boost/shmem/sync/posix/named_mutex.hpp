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

   struct mut_construct_func_t
   {
      enum CreationType {  open_only, open_or_create, create_only  };

      mut_construct_func_t(sem_t *&mut)
         :  mp_mut(mut){}

      bool operator()(const shared_memory::segment_info_t * info, bool created) const
      {   
         mp_mut = reinterpret_cast<sem_t *>(info->get_user_ptr());
         switch(m_create){
            case open_only:   
               mp_mut = reinterpret_cast<sem_t *>(info->get_user_ptr());
            break;

            case open_or_create:
               if(created){
                 if(sem_init(mp_sem, 1, 1) == 0){
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

      CreationType   m_create;
      sem_t *       &mp_mut; 
   };

   struct mut_destroy_func_t
   {
      mut_destroy_func_t(sem_t *&mut)
         :  mp_mut(mut){}

      void operator()(const shared_memory::segment_info_t * info, bool last) const
      {  
         if(last){
            int res = sem_destroy(mp_mut);
            assert(res  == 0);
         }
      }
      sem_t *       &mp_mut; 
   };
}

inline named_mutex::~named_mutex()
{  
   close();
}

inline named_mutex::named_mutex() 
   : mp_mut((sem_t*)0)
{ }

inline bool named_mutex::create(const char *name)
{
   if(mp_mut != (sem_t*)0)
      return false;

   detail::mut_construct_func_t func(mp_mut);
   func.m_create        = detail::mut_construct_func_t::create_only;

   if(!m_shmem.create_with_func(name, sizeof(sem_t), func)){
      return false;
   }

   return true;
}

inline bool named_mutex::open_or_create(const char *name)
{
   if(mp_mut != (sem_t*)0)
      return false;

   detail::mut_construct_func_t func(mp_mut);
   func.m_create        = detail::mut_construct_func_t::open_or_create;

   if(!m_shmem.open_or_create_with_func(name, sizeof(sem_t), func)){
      return false;
   }

   return true;
}

inline bool named_mutex::open(const char *name)
{
   if(mp_mut != (sem_t*)0){
      return false;
   }

   detail::mut_construct_func_t func(mp_mut);
   func.m_create  = detail::mut_construct_func_t::open_only;

   if(!m_shmem.open_with_func(name, func)){
      return false;
   }

   return true;
}

inline void named_mutex::close()
{
   if(mp_mut != (sem_t*)0){
      m_shmem.close_with_func(detail::mut_destroy_func_t(mp_mut));
      mp_mut = (sem_t*)0;
   }
}

inline void named_mutex::lock()
{
   if(sem_wait(mp_mut) != 0){
      throw lock_exception();
   }
}

inline void named_mutex::unlock()
{
   if(sem_post(mp_mut) != 0){
      throw lock_exception();
   }
}

inline bool named_mutex::try_lock()
{
   int res = sem_trywait(mp_mut);
   assert(res == 0 || errno == EAGAIN);
   if(!(res == 0 || errno == EAGAIN)){
      throw lock_exception();
   }
   return res == 0;
}

inline bool named_mutex::timed_lock(const xtime &xt)
{
   for (;;){
      timespec tspec;
      xtime_to_timespec(xt, tspec);

      int res = sem_timedwait((mp_mut, &tspec);
      assert(res == 0 || errno == ETIMEDOUT);

      if(!(res == 0 || errno == ETIMEDOUT)){
         throw lock_exception();
      }

      if (errno == ETIMEDOUT)
         return false;
   }
   return true;
}

}  //namespace shmem {

}  //namespace boost {

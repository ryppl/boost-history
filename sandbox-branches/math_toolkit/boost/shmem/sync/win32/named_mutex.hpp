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

inline named_mutex::~named_mutex()
{  
   this->close();
}

inline named_mutex::named_mutex() 
   : mp_mut(0)
{ }

inline bool named_mutex::create(const char *name)
{
   if(mp_mut)
      return false;
   mp_mut = detail::create_mutex(name);

   if(!mp_mut){
      return false;
   }
   else if(detail::get_last_error() == detail::error_already_exists){
      detail::close_handle(mp_mut);
      return false;
   }
   return true;
}

inline bool named_mutex::open_or_create(const char *name)
{
   if(mp_mut)
      return false;
   mp_mut = detail::create_mutex(name);

   if(!mp_mut){
      return false;
   }
   return true;
}

inline bool named_mutex::open(const char *name)
{
   if(mp_mut)
      return false;

   mp_mut = detail::create_mutex(name);
   return mp_mut != 0;
}

inline void named_mutex::close()
{
   if(mp_mut){
      bool success = detail::close_handle(mp_mut) == 1;
      assert(success == true);
      mp_mut = 0;
   }
}

inline void named_mutex::unlock()
{
   if(detail::release_mutex(mp_mut) != 1){
      throw lock_exception();
   }
}

inline void named_mutex::lock()
{
   if(detail::wait_for_single_object
      (mp_mut, detail::infinite_time) != detail::wait_object_0){
      throw lock_exception();
   }
}

inline bool named_mutex::try_lock()
{
   return detail::wait_for_single_object(mp_mut, 0) == detail::wait_object_0;
}

inline bool named_mutex::timed_lock(const xtime &xt)
{
   for (;;)
   {
      int milliseconds;
      to_duration(xt, milliseconds);

      unsigned long res = 
         detail::wait_for_single_object(mp_mut, detail::infinite_time);

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


//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHARED_MEMORY_HPP
#define BOOST_SHARED_MEMORY_HPP

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <boost/type_traits/type_with_alignment.hpp>
#include <boost/shmem/detail/utilities.hpp>
#include <boost/noncopyable.hpp>
#include <boost/detail/workaround.hpp>
#include <boost/shmem/shmem_fwd.hpp>

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
#  include <boost/shmem/sync/win32/win32_sync_primitives.hpp>
#else

#  ifdef BOOST_HAS_UNISTD_H
#    include <fcntl.h>        //O_CREAT, O_*... 
#    include <sys/mman.h>     //mmap
#    include <unistd.h>       //ftruncate, close
#    include <semaphore.h>  //sem_t* family, SEM_VALUE_MAX
#    include <string>         //std::string
#    include <sys/stat.h>     //mode_t, S_IRWXG, S_IRWXO, S_IRWXU,
#  else
#    error Unknown platform
#  endif

#endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

#include <boost/shmem/offset_ptr.hpp>

/*!\file
   Describes shared_memory class
*/

namespace boost {

namespace shmem {

/*!A class that wraps basic shared memory management*/
class shared_memory : private boost::noncopyable
{
 public:

   class segment_info_t
   {
      friend class shared_memory;
    public:
      /*!Returns pointer to the shared memory fragment
         the user can overwrite*/
      void *      get_user_ptr() const;
      /*!Returns the size of the shared memory fragment
         the user can overwrite*/
      std::size_t get_user_size()const;
    private:
      std::size_t       ref_count;
      std::size_t       real_size;
   };

   /*!Initializes members. Does not throw*/
   shared_memory();

   /*!Calls close. Does not throw*/
  ~shared_memory();

   /*!Creates a shared memory segment with name "name", with size "size".
      User can specify the mapping address. Never throws.*/
   bool create(const char *name,          size_t size, 
               const void *addr = 0);

   /*!Creates a shared memory segment with name "name", with size "size".
      User can also specify the mapping address in "addr". Never throws.
      It also executes the functor "func" atomically if the segment is created.
      Functor has must have the following signature:

      bool operator()(const segment_info_t * info, bool created) const
      
      "info" is an initialized segment info structure, and "created" 
      must be "true". If the functor returns "false", an error is supposed
      and segment will be closed. The functor must not throw.*/
   template <class Func>
   bool create_with_func(const char *name, size_t size, Func func,
                         const void *addr = 0);

   /*!Opens previously created shared memory segment with name "name", 
      and size "size". If the memory segment was not previously created, 
      the function return false. User can specify the mapping address. 
      Never throws.*/
   bool open(const char *name, const void *addr = 0);

   /*!Opens previously created shared memory segment with name "name", 
      and size "size". If the memory segment was not previously created, 
      the function returns "false". User can specify the mapping address. 
      Never throws.
      It also executes the functor "func" atomically if the segment is opened.
      Functor has must have the following signature:

      bool operator()(const segment_info_t * info, bool created) const
      
      "info" is an initialized segment info structure, and "created" 
      must be "false". If the functor returns "false", an error is supposed
      and segment will be closed. The functor must not throw.*/
   template <class Func>
   bool open_with_func(const char *name, Func func,
                       const void *addr = 0);

   /*!Creates a shared memory segment with name "name", and size "size" if
      the shared memory was not previously created. If it was previously 
      created it tries to open it. User can specify the mapping address.
      Never throws.*/
   bool open_or_create(const char *name, size_t size, const void *addr = 0);

   /*!Creates a shared memory segment with name "name", and size "size" if
      the shared memory was not previously created. If it was previously 
      created it tries to open it. User can specify the mapping address.
      Never throws.
      It also executes the functor "func" atomically if the segment is 
      created or opened. Functor has must have the following signature:

      bool operator()(const segment_info_t * info, bool created) const
      
      "info" is an initialized segment info structure, and "created" 
      will be "true" if the segment was created or "false" if the segment was
      opened. If the functor returns "false", an error is supposed
      and segment will be closed. The functor must not throw.*/
   template <class Func>
   bool open_or_create_with_func(const char *name, size_t size, 
                                 Func func,        const void *addr = 0);

   /*!Returns the size of the shared memory segment. Never throws.*/
   size_t   get_size()  const;

   /*!Returns shared memory segment's base address for this process. 
      Never throws.*/
   void*    get_base()  const;

   /*!Unmaps shared memory from process' address space. Never throws.*/
   void close();

   /*!Unmaps shared memory from process' address space . Never throws.
      It also executes the functor "func" atomically.
      Functor has must have the following signature:

      void operator()(const segment_info_t * info, bool last) const
      
      "info" is an initialized segment info structure, and "last" 
      indicates if this unmapping is the last unmapping so that
      there will be no no other processes attached to the segment.
      The functor must not throw.*/
   template <class Func>
   void close_with_func(Func func);

   #if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   typedef void * OS_handle_t;
   #else    //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   typedef int    OS_handle_t;
   #endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   OS_handle_t get_shmem_OS_handle(){  return m_shmHnd;  }

   void swap(shared_memory &other);

 private:

   friend void swap(shared_memory &x, shared_memory &y);

   enum { Alignment  = boost::alignment_of<boost::detail::max_align>::value  };

 public:
   enum { segment_info_size = detail::ct_rounded_size<sizeof(segment_info_t), Alignment>::value  };

 private:
   /*!No-op creation functor for xxx_with_func() functions*/
   struct null_construct_func_t
   {
      bool operator()(const segment_info_t * info, bool created) const
         {   return true;   }
   };

   /*!No-op creation functor for close_with_func() functions*/
   struct null_destroy_func_t
   {
      void operator()(const segment_info_t * info, bool last) const
         {  }
   };

   template <class Func>
   bool priv_create(const char *name, size_t size, 
                    bool createonly,  const void *addr, Func func);

   template <class Func>
   bool priv_open(const char *name, const void *addr, Func f);

   template <class Func>
   void priv_close_with_func(Func func);

   void priv_close_handles();

   class GlobalNamedScopedMutex
   {
    public:
      GlobalNamedScopedMutex();
      bool acquire();
     ~GlobalNamedScopedMutex();

    private:
      #if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
      void    *m_mut;
      #else
      sem_t   *m_sem;
      #endif
      bool     m_acquired;
   };

   OS_handle_t       m_shmHnd;
   segment_info_t  * mp_info;  
   #if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   #else
   std::string       m_name;
   #endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
};

inline bool shared_memory::create(const char *name,          size_t size, 
            const void *addr)
{  
   return priv_create(name, size, true, addr, null_construct_func_t()); 
}

template <class Func>
inline bool shared_memory::create_with_func(const char *name, size_t size, Func func,
                        const void *addr)
{  
   return priv_create(name, size, true, addr, func); 
}

inline bool shared_memory::open(const char *name, const void *addr)
{  
   return priv_open(name, addr, null_construct_func_t()); 
}

template <class Func>
bool shared_memory::open_with_func(const char *name, Func func,
                     const void *addr)
{  
   return priv_open(name, addr, func); 
}

inline bool shared_memory::open_or_create(const char *name, size_t size, const void *addr)
{  
   return priv_create(name, size, false, addr, null_construct_func_t()); 
}

template <class Func>
inline bool shared_memory::open_or_create_with_func(const char *name, size_t size, 
                              Func func,        const void *addr)
{  
   return priv_create(name, size, false, addr, func); 
}

inline void shared_memory::close()
{  
   this->priv_close_with_func(null_destroy_func_t());  
}

template <class Func>
inline void shared_memory::close_with_func(Func func)
{  
   this->priv_close_with_func(func);  
}

inline void *shared_memory::segment_info_t::get_user_ptr() const
{  
   return detail::char_ptr_cast(this)+segment_info_size; 
}

inline std::size_t shared_memory::segment_info_t::get_user_size() const
{
   return this->real_size - segment_info_size; 
}

inline shared_memory::~shared_memory() 
{
   this->close();
}

inline size_t   shared_memory::get_size()  const  
{  
   return mp_info->get_user_size(); 
}

inline void*    shared_memory::get_base()  const  
{  
   return mp_info->get_user_ptr(); 
}

inline void shared_memory::swap(shared_memory &other)
{
   detail::swap(this->m_shmHnd, other.m_shmHnd);
   detail::swap(this->mp_info, other.mp_info);
   detail::swap(this->mp_info, other.mp_info);
   #if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   #else
   detail::swap(this->m_name,    other.m_name);
   #endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
}

inline void swap(shared_memory &x, shared_memory &y)
{
   x.swap(y);
}

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

inline shared_memory::GlobalNamedScopedMutex::GlobalNamedScopedMutex()
   : m_mut(0), m_acquired(false){}
   
inline bool shared_memory::GlobalNamedScopedMutex::acquire()
{  
   m_mut = create_mutex("/boost_shmem_shm_global_mutex");  
   if(m_mut == 0)
      return false;
   m_acquired = wait_for_single_object(m_mut, infinite_time) == wait_object_0; 
   return m_acquired;  
}

inline shared_memory::GlobalNamedScopedMutex::~GlobalNamedScopedMutex()
{
   if(m_acquired){
      release_mutex(m_mut);
   }

   if(m_mut){
      close_handle(m_mut);
   }
}

inline shared_memory::shared_memory() 
   : m_shmHnd(0), mp_info(0)
{}

template <class Func>
inline bool shared_memory::priv_create(const char *name,   size_t size, 
                                       bool createonly,    const void *addr,   Func func)
{
   if(m_shmHnd)   return false;

   unsigned long flProtect = page_readwrite;
   bool created   = true;

   GlobalNamedScopedMutex mut;
   if(!mut.acquire()){
      return false;
   }

   size += segment_info_size;

   //Create new shared memory
   m_shmHnd = create_file_mapping( invalid_handle, // handle to file to map
                                    flProtect, // protection for mapping object
                                    0,          // high-order 32 bits of object size
                                    (unsigned long)size,// low-order 32 bits of object size
                                    name );     // name of file-mapping object
                                 
   //Check if was already created
   if( m_shmHnd != 0  &&  get_last_error() == error_already_exists ) { 
      if(createonly){
         this->priv_close_handles();
         return false;
      }
      else{
         created = false;
         //Map into process address space
         mp_info = reinterpret_cast<segment_info_t*> (
                      map_view_of_file_ex( m_shmHnd, // file-mapping object to map into address space
                                 file_map_all_access,
                                 0,    // high-order 32 bits of file offset
                                 0,    // low-order 32 bits of file offset
                                 segment_info_size,    // number of bytes to map
                                 (void*)addr)// suggested starting address for mapped view
                  );
         if(!mp_info){
            this->priv_close_handles();
            return false;
         }

         //Get the real size of the segment
         size   = reinterpret_cast<segment_info_t *>(mp_info)->real_size;

         //Unmap the memory to remap it
         unmap_view_of_file(mp_info);
      }
   }
   else if(!m_shmHnd){
      this->priv_close_handles();
      return false;
   }
   else{
      created = true;
   }

   //Map into process address space
   mp_info = reinterpret_cast<segment_info_t*> (
            map_view_of_file_ex( m_shmHnd, // file-mapping object to map into address space
                              file_map_all_access,
                              0,    // high-order 32 bits of file offset
                              0,    // low-order 32 bits of file offset
                              size, // number of bytes to map
                              (void*)(addr))// suggested starting address for mapped view
            );
   if(!mp_info){
      this->priv_close_handles();
      return false;
   }

   //Initialize data if segment was created
   if(created){
      mp_info->real_size = size;
      mp_info->ref_count      = 0;
   }

   if(!func(const_cast<const segment_info_t *>(mp_info), created)){
      this->priv_close_handles();
      return false;
   }
   ++mp_info->ref_count;
   return true;
}

template <class Func>
inline bool shared_memory::priv_open(const char *name,   const void *addr,   Func func)
{
   if(m_shmHnd)   return false;

   unsigned long dwDesiredAccess = file_map_write;

   GlobalNamedScopedMutex mut;
   if(!mut.acquire()){
      return false;
   }

   //Open existing shared memory
   m_shmHnd = open_file_mapping(dwDesiredAccess, name );

   if(!m_shmHnd){
      this->priv_close_handles();
      return false;
   }

   //Map into process address space
   mp_info = reinterpret_cast<segment_info_t*> (
            map_view_of_file_ex( m_shmHnd, // file-mapping object to map into address space
                             file_map_all_access,
                             0,    // high-order 32 bits of file offset
                             0,    // low-order 32 bits of file offset
                             segment_info_size,    // number of bytes to map
                             (void*)addr)// suggested starting address for mapped view
            );

   if(!mp_info){
      this->priv_close_handles();
      return false;
   }

   std::size_t size = mp_info->real_size;

   //Unmap the memory to remap it
   unmap_view_of_file(mp_info);
   
   //Map into process address space
   mp_info = reinterpret_cast<segment_info_t*> (
            map_view_of_file_ex( m_shmHnd, // file-mapping object to map into address space
                             file_map_all_access,
                             0,    // high-order 32 bits of file offset
                             0,    // low-order 32 bits of file offset
                             size,    // number of bytes to map
                             (void*)addr)// suggested starting address for mapped view
             );
   if(!mp_info){
      this->priv_close_handles();
      return false;
   }

   if(!func(const_cast<const segment_info_t *>(mp_info), false)){
      this->priv_close_handles();
      return false;
   }
   ++mp_info->ref_count;
   return true;
}

template <class Func>
inline void shared_memory::priv_close_with_func(Func func)
{
   //Check if it was previously closed
   if(mp_info && m_shmHnd){
      GlobalNamedScopedMutex mut;
      mut.acquire();
      func (const_cast<const segment_info_t *>(mp_info), mp_info->ref_count == 1);
      --mp_info->ref_count;
      this->priv_close_handles();
   }
}

inline void shared_memory::priv_close_handles()
{
   if(mp_info){
      unmap_view_of_file(mp_info);
      mp_info = 0;
   }

   if(m_shmHnd){
      close_handle(m_shmHnd);
      m_shmHnd = 0;
   }
}

#else    //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

inline shared_memory::GlobalNamedScopedMutex::GlobalNamedScopedMutex()
   : m_sem((sem_t*)-1), m_acquired(false){}
   
inline bool shared_memory::GlobalNamedScopedMutex::acquire()
{  
   mode_t mode = S_IRWXG | S_IRWXO | S_IRWXU;
   m_sem = sem_open("/boost_shmem_shm_global_mutex", O_CREAT, mode, 1);
   if(m_sem == ((sem_t*)-1))
      return false;
   m_acquired = sem_wait(m_sem) == 0; 
   return m_acquired;  
}

inline shared_memory::GlobalNamedScopedMutex::~GlobalNamedScopedMutex()
{
   if(m_acquired){
      sem_post(m_sem);
   }

   if(m_sem != ((sem_t*)-1)){
      sem_close(m_sem);
   }
}

inline shared_memory::shared_memory() 
   : m_shmHnd(-1), mp_info(static_cast<segment_info_t*>(MAP_FAILED))
{}

template <class Func>
inline bool shared_memory::priv_create(const char *name, size_t size,
                                       bool createonly,  const void *addr, Func func)

{
   if(m_shmHnd != -1)   return false;
   bool created = false;

   //Create new shared memory
   int oflag   = O_CREAT | O_EXCL;
   int prot    = PROT_READ;

   oflag |= O_RDWR;
   prot  |= PROT_WRITE;

   GlobalNamedScopedMutex mut;
   if(!mut.acquire()){
      return false;
   }

   size += segment_info_size;

   m_shmHnd = shm_open( name,    // memory name
                        oflag,   // read/write access
                        S_IRWXO | S_IRWXG | S_IRWXU);   // permissions
   if(m_shmHnd == -1){
      created = false;
      //Try to open the shared memory
      if(!createonly){
         oflag   = O_CREAT | O_RDWR;
         m_shmHnd = shm_open( name,    // memory name
                              oflag,   // read/write access
                              S_IRWXO | S_IRWXG | S_IRWXU);   // permissions
         
      }
      //If we can open the shared memory return error
      if(m_shmHnd == -1){
         this->priv_close_handles();
         return false;
      }

      //Map the segment_info_t structure to obtain the real size of the segment
      mp_info = reinterpret_cast<segment_info_t*> (
                mmap((void*)addr, segment_info_size, prot, MAP_SHARED, m_shmHnd, 0)
                );
      if(mp_info == MAP_FAILED){
         this->priv_close_handles();
         //Don't unlink since we have just failed to open
         return false;
      }

      //Obtain real size
      size = mp_info->real_size;

      //Unmap the memory to remap it
      munmap(mp_info, segment_info_size);
   }
   else{
      created = true;
      // Set the memory object's size
      if( ftruncate( m_shmHnd, size ) == -1 ) {
         this->priv_close_handles();
         shm_unlink(name);
         return false;
      }
   }

   m_name = name;
 
   mp_info = reinterpret_cast<segment_info_t*> (
               mmap((void*)addr, size, prot, MAP_SHARED, m_shmHnd, 0)
             );
   if(mp_info == MAP_FAILED){
      this->priv_close_handles();
      if(created){
         shm_unlink(name);
      }
      return false;
   }

   //Initialize data if segment was created
   if(created){
      mp_info->real_size = size;
      mp_info->ref_count      = 0;
   }

   //Execute atomic function
   if(!func(const_cast<const segment_info_t *>(mp_info), created)){
      this->priv_close_handles();
      if(created){
         shm_unlink(name);
      }
      return false;
   }
   ++mp_info->ref_count;
   return true;
}

template <class Func>
inline bool shared_memory::priv_open(const char *name, const void *addr, Func func)
{
   if(m_shmHnd != -1)   return false;

   //Create new shared memory
   int oflag   = 0;
   int prot    = PROT_READ;

   oflag |= O_RDWR;
   prot  |= PROT_WRITE;

   GlobalNamedScopedMutex mut;
   if(!mut.acquire()){
      return false;
   }

   m_shmHnd = shm_open( name,    // memory name
                        oflag,   // read/write access
                        S_IRWXO | S_IRWXG | S_IRWXU);   // permissions
   if(m_shmHnd == -1){
      this->priv_close_handles();
      return false;
   }

   m_name = name;

   mp_info = reinterpret_cast<segment_info_t*> (
               mmap((void*)addr, segment_info_size, prot, MAP_SHARED, m_shmHnd, 0)
             );

   if(mp_info == MAP_FAILED){
      this->priv_close_handles();
      return false;
   }

   //Obtain real size
   std::size_t size = mp_info->real_size;

   //Unmap the memory to remap it
   munmap(mp_info, segment_info_size);

   mp_info = reinterpret_cast<segment_info_t*> (
               mmap((void*)addr, size, prot, MAP_SHARED, m_shmHnd, 0)
             );

   if(mp_info == MAP_FAILED){
      this->priv_close_handles();
      return false;
   }

   if(!func(const_cast<const segment_info_t *>(mp_info), false)){
      this->priv_close_handles();
      return false;
   }
   ++mp_info->ref_count;
   return true;
}

template<class Func>
inline void shared_memory::priv_close_with_func(Func func)
{
   //Check if it was previously closed
   if((mp_info != MAP_FAILED) && (m_shmHnd != -1)){
      GlobalNamedScopedMutex mut;
      mut.acquire();
      bool last = mp_info->ref_count == 1;
      func (const_cast<const segment_info_t *>(mp_info), last);
      --mp_info->ref_count;
      this->priv_close_handles();
      //If last link, unlink the shared memory so that it is destroyed
      if(last){
         shm_unlink(m_name.c_str());
      }
      //Empty string memory
      std::string tmp;
      m_name.swap(tmp);
   }
}

inline void shared_memory::priv_close_handles()
{
   if(mp_info != MAP_FAILED){
      munmap(mp_info, mp_info->get_user_size());
      mp_info = static_cast<segment_info_t*>(MAP_FAILED);
   }

   if(m_shmHnd != -1){
      ::close(m_shmHnd);
      m_shmHnd = -1;
   }
}

#endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif   //BOOST_SHARED_MEMORY_HPP

//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2006. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_MANAGED_OPEN_OR_CREATE_IMPL
#define BOOST_INTERPROCESS_MANAGED_OPEN_OR_CREATE_IMPL

#include <boost/interprocess/detail/os_thread_functions.hpp>
#include <boost/interprocess/detail/os_file_functions.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/detail/utilities.hpp>
#include <boost/interprocess/detail/type_traits.hpp>
#include <boost/interprocess/detail/atomic.hpp>
#include <boost/interprocess/creation_tags.hpp>
#include <boost/interprocess/detail/mpl.hpp>
#include <boost/cstdint.hpp>

namespace boost {
namespace interprocess {
namespace detail {

template<class DeviceAbstraction, bool FileBased = true>
class managed_open_or_create_impl
{
   //Non-copyable
   managed_open_or_create_impl();
   managed_open_or_create_impl(const managed_open_or_create_impl &);
   managed_open_or_create_impl &operator=(const managed_open_or_create_impl &);

   enum
   {  
      UninitializedSegment,  
      InitializingSegment,  
      InitializedSegment,
      CorruptedSegment
   };

   public:

   enum
   {
      ManagedOpenOrCreateUserOffset = 
         detail::ct_rounded_size
            < sizeof(boost::uint32_t)
            , detail::alignment_of<detail::max_align>::value>::value
   };

   managed_open_or_create_impl(create_only_t, 
                 const char *name,
                 std::size_t size,
                 mode_t mode = read_write,
                 const void *addr = 0)
   {
      m_name = name;
      priv_open_or_create
         ( detail::DoCreate
         , size
         , mode
         , addr
         , null_mapped_region_function());
   }

   managed_open_or_create_impl(open_only_t, 
                 const char *name,
                 mode_t mode = read_write,
                 const void *addr = 0)
   {
      m_name = name;
      priv_open_or_create
         ( detail::DoOpen
         , 0
         , mode
         , addr
         , null_mapped_region_function());
   }


   managed_open_or_create_impl(open_or_create_t, 
                 const char *name,
                 std::size_t size,
                 mode_t mode = read_write,
                 const void *addr = 0)
   {
      m_name = name;
      priv_open_or_create
         ( detail::DoCreateOrOpen
         , size
         , mode
         , addr
         , null_mapped_region_function());
   }

   template <class ConstructFunc>
   managed_open_or_create_impl(create_only_t, 
                 const char *name,
                 std::size_t size,
                 mode_t mode,
                 const void *addr,
                 const ConstructFunc &construct_func)
   {
      m_name = name;
      priv_open_or_create
         (detail::DoCreate
         , size
         , mode
         , addr
         , construct_func);
   }

   template <class ConstructFunc>
   managed_open_or_create_impl(open_only_t, 
                 const char *name,
                 mode_t mode,
                 const void *addr,
                 const ConstructFunc &construct_func)
   {
      m_name = name;
      priv_open_or_create
         ( detail::DoOpen
         , 0
         , mode
         , addr
         , construct_func);
   }

   template <class ConstructFunc>
   managed_open_or_create_impl(open_or_create_t, 
                 const char *name,
                 std::size_t size,
                 mode_t mode,
                 const void *addr,
                 const ConstructFunc &construct_func)
   {
      m_name = name;
      priv_open_or_create
         ( detail::DoCreateOrOpen
         , size
         , mode
         , addr
         , construct_func);
   }

   #ifndef BOOST_INTERPROCESS_RVALUE_REFERENCE
   managed_open_or_create_impl
      (detail::moved_object<managed_open_or_create_impl> &moved)
   {  this->swap(moved.get());   }
   #else
   managed_open_or_create_impl
      (managed_open_or_create_impl &&moved)
   {  this->swap(moved);   }
   #endif

   #ifndef BOOST_INTERPROCESS_RVALUE_REFERENCE
   managed_open_or_create_impl &operator=
      (detail::moved_object<managed_open_or_create_impl> &moved)
   {  
      managed_open_or_create_impl tmp(moved);
      this->swap(tmp);
      return *this;  
   }
   #else
   managed_open_or_create_impl &operator=
      (managed_open_or_create_impl &&moved)
   {  
      managed_open_or_create_impl tmp(move(moved));
      this->swap(tmp);
      return *this;  
   }
   #endif

   ~managed_open_or_create_impl()
   {}

   std::size_t get_size()  const
   {  return m_mapped_region.get_size() - ManagedOpenOrCreateUserOffset; }

   void *get_address()  const
   {  return (char*)m_mapped_region.get_address() + ManagedOpenOrCreateUserOffset;  }

   void swap(managed_open_or_create_impl &other)
   {
      this->m_name.swap(other.m_name);
      this->m_mapped_region.swap(other.m_mapped_region);
   }

   const char *get_name() const
   {  return m_name.c_str();  }

   bool flush()
   {  return m_mapped_region.flush();  }

   private:

   //These are templatized to allow explicit instantiations
   template<bool dummy>
   static void write_whole_device(DeviceAbstraction &, std::size_t, detail::false_)
   {} //Empty

   template<bool dummy>
   static void write_whole_device(DeviceAbstraction &dev, std::size_t size, detail::true_)
   {
      file_handle_t hnd = detail::file_handle_from_mapping_handle(dev.get_mapping_handle());

      if(size <= ManagedOpenOrCreateUserOffset){
         throw interprocess_exception(error_info(system_error_code()));
      }

      size -= ManagedOpenOrCreateUserOffset;

      if(!detail::set_file_pointer(hnd, ManagedOpenOrCreateUserOffset, file_begin)){
         throw interprocess_exception(error_info(system_error_code()));
      }

      //We will write zeros in the file
      for(std::size_t remaining = size, write_size = 0
         ;remaining > 0
         ;remaining -= write_size){
         const std::size_t DataSize = 512;
         static char data [DataSize];
         write_size = DataSize < remaining ? DataSize : remaining;
         if(!detail::write_file(hnd, data, write_size)){
            error_info err = system_error_code();
            throw interprocess_exception(err);
         }
      }
   }

   //These are templatized to allow explicit instantiations
   template<bool dummy>
   static void truncate_device(DeviceAbstraction &, std::size_t, detail::false_)
   {} //Empty

   template<bool dummy>
   static void truncate_device(DeviceAbstraction &dev, std::size_t size, detail::true_)
   {  dev.truncate(size);  }

   //These are templatized to allow explicit instantiations
   template<bool dummy>
   static void create_device(DeviceAbstraction &dev, const char *name, std::size_t size, detail::false_)
   {
      DeviceAbstraction tmp(create_only, name, read_write, size);
      tmp.swap(dev);
   }

   template<bool dummy>
   static void create_device(DeviceAbstraction &dev, const char *name, std::size_t, detail::true_)
   {
      DeviceAbstraction tmp(create_only, name, read_write);
      tmp.swap(dev);
   }

   template <class ConstructFunc> inline 
   void priv_open_or_create
      (detail::create_enum_t type, std::size_t size,
       mode_t mode, const void *addr,
       ConstructFunc construct_func)
   {
      typedef detail::bool_<FileBased> file_like_t;
      (void)mode;
      error_info err;
      bool created = false;
      DeviceAbstraction dev;

      if(type != detail::DoOpen && size < ManagedOpenOrCreateUserOffset){
         throw interprocess_exception(error_info(size_error));
      }

      if(type == detail::DoOpen){
         DeviceAbstraction tmp(open_only, m_name.c_str(), read_write);
         tmp.swap(dev);
         created = false;
      }
      else if(type == detail::DoCreate){
         create_device<FileBased>(dev, m_name.c_str(), size, file_like_t());
         created = true;
      }
      else if(type == detail::DoCreateOrOpen){
         //This loop is very ugly, but brute force is sometimes better
         //than diplomacy. If someone knows how to open or create a
         //file and know if we have really created it or just open it
         //drop me a e-mail!
         bool completed = false;
         while(!completed){
            try{
               create_device<FileBased>(dev, m_name.c_str(), size, file_like_t());
               created     = true;
               completed   = true;
            }
            catch(interprocess_exception &ex){
               if(ex.get_error_code() != already_exists_error){
                  throw;
               }
               else{
                  try{
                     DeviceAbstraction tmp(open_only, m_name.c_str(), read_write);
                     dev.swap(tmp);
                     created     = false;
                     completed   = true;
                  }
                  catch(interprocess_exception &ex){
                     if(ex.get_error_code() != not_found_error){
                        throw;
                     }
                  }
               }
            }
            detail::thread_yield();
         }
      }

      if(created){
         try{
            //If this throws, we are lost
            truncate_device<FileBased>(dev, size, file_like_t());

            //If the following throws, we will truncate the file to 1
            mapped_region        region(dev, read_write, 0, 0, addr);
            boost::uint32_t *patomic_word = static_cast<boost::uint32_t*>(region.get_address());
            boost::uint32_t previous = detail::atomic_cas32(patomic_word, InitializingSegment, UninitializedSegment);

            if(previous == UninitializedSegment){
               try{
                  write_whole_device<FileBased>(dev, size, file_like_t());
                  construct_func((char*)region.get_address() + ManagedOpenOrCreateUserOffset, size - ManagedOpenOrCreateUserOffset, true);
                  //All ok, just move resources to the external mapped region
                  m_mapped_region.swap(region);
               }
               catch(...){
                  detail::atomic_write32(patomic_word, CorruptedSegment);
                  throw;
               }
               detail::atomic_write32(patomic_word, InitializedSegment);
            }
            else if(previous == InitializingSegment || previous == InitializedSegment){
               throw interprocess_exception(error_info(already_exists_error));
            }
            else{
               throw interprocess_exception(error_info(corrupted_error));
            }
         }
         catch(...){
            try{
               truncate_device<FileBased>(dev, 1u, file_like_t());
            }
            catch(...){
            }
            throw;
         }
      }
      else{
         if(FileBased){
            offset_t filesize = 0;
            while(filesize == 0){
               if(!detail::get_file_size(detail::file_handle_from_mapping_handle(dev.get_mapping_handle()), filesize)){
                  throw interprocess_exception(error_info(system_error_code()));
               }
               detail::thread_yield();
            }
            if(filesize == 1){
               throw interprocess_exception(error_info(corrupted_error));
            }
         }

         mapped_region  region(dev, read_write, 0, 0, addr);

         boost::uint32_t *patomic_word = static_cast<boost::uint32_t*>(region.get_address());
         boost::uint32_t value = detail::atomic_read32(patomic_word);

         while(value == InitializingSegment || value == UninitializedSegment){
            detail::thread_yield();
            value = detail::atomic_read32(patomic_word);
         }

         if(value != InitializedSegment)
            throw interprocess_exception(error_info(corrupted_error));

         construct_func((char*)region.get_address() + ManagedOpenOrCreateUserOffset, region.get_size(), false);
         //All ok, just move resources to the external mapped region
         m_mapped_region.swap(region);
      }
   }

   private:
   mapped_region     m_mapped_region;
   std::string       m_name;
};

template<class DeviceAbstraction>
inline void swap(managed_open_or_create_impl<DeviceAbstraction> &x
                ,managed_open_or_create_impl<DeviceAbstraction> &y)
{  x.swap(y);  }

}  //namespace detail {
}  //namespace interprocess {
}  //namespace boost {

#endif   //#ifndef BOOST_INTERPROCESS_MANAGED_OPEN_OR_CREATE_IMPL

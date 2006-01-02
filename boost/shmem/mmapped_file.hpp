//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_MMAPPED_FILE_HPP
#define BOOST_SHMEM_MMAPPED_FILE_HPP

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/shmem_fwd.hpp>
#include <boost/noncopyable.hpp>

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
#  include <boost/shmem/sync/win32/win32_sync_primitives.hpp>
#else

#  ifdef BOOST_HAS_UNISTD_H
#    include <fcntl.h>
#    include <sys/mman.h>     //mmap, off64_t
#    include <unistd.h>
#    include <sys/stat.h>
#  else
#    error Unknown platform
#  endif

#endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

/*!\file
   Describes mmapped_file class
*/

namespace boost {

namespace shmem {

/*!A class that wraps basic file-mapping management*/
class mmapped_file : private boost::noncopyable
{
   #if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   typedef long long          fileoff_t;
   typedef void *             OS_handle_t;
   #else    //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   typedef off64_t            fileoff_t;
   #endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

 public:
   typedef enum { 
                  ro_mode = 0x01, rw_mode = 0x02, access_mode_mask = 0xff 
                }    accessmode_t;

   /*!Initializes members. Does not throw*/
   mmapped_file();

   /*!Calls close. Does not throw*/
   ~mmapped_file();

   /*!Opens a file mapping of file "filename", starting in offset 
      "file_offset", and the mapping's size will be "size". The mapping 
      can be opened for read-only "ro_mode" or read-write "rw_mode. 
      Never throws.*/
   bool open(const char *filename,  fileoff_t file_offset, 
             std::size_t size,      accessmode_t mode,
             void *address = 0);

   /*!Closes a previously opened file mapping. Never throws.*/
   void close();

   /*!Returns if file mapping is open. Never throws.*/
   bool     is_open()  const;

   /*!Returns the size of the file mapping. Never throws.*/
   std::size_t get_size() const;

   /*!Returns the base address of the file mapping. Never throws.*/
   void*    get_base() const;

   /*!Returns the file offset of the file mapping from the beginning of the file.
      Never throws.*/
   fileoff_t  get_file_offset() const;

   /*!Flushes to the disk a byte range within the mapped file. 
      Never throws*/
   bool     flush(std::size_t mapping_offset = 0, std::size_t numbytes = 0);

   friend void swap(mmapped_file &x, mmapped_file &y);

 private:
   #if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   OS_handle_t       m_fileMappingHnd;
   OS_handle_t       m_fileHnd;
   fileoff_t         m_extra_offset;
   #else    //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   int               m_fileHnd;
   #endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   void*             m_base;
   std::size_t       m_size;
   fileoff_t         m_file_offset;
};

inline void swap(mmapped_file &x, mmapped_file &y)
{
   detail::swap_function(x.m_base, y.m_base);
   detail::swap_function(x.m_size, y.m_size);
   detail::swap_function(x.m_file_offset, y.m_file_offset);
   #if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
   detail::swap_function(x.m_fileMappingHnd,    y.m_fileMappingHnd);
   detail::swap_function(x.m_fileHnd,    y.m_fileHnd);
   detail::swap_function(x.m_extra_offset,    y.m_extra_offset);
   #else
   detail::swap_function(x.m_fileHnd,    y.m_fileHnd);
   #endif   //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)
}

inline mmapped_file::~mmapped_file() 
{ 
   close();
}

inline std::size_t mmapped_file::get_size()  const  
{  
   return m_size; 
}

inline mmapped_file::fileoff_t  mmapped_file::get_file_offset()  const  
{  
   return m_file_offset;
}

inline void*    mmapped_file::get_base()  const  
{  
   return m_base; 
}

#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

inline mmapped_file::mmapped_file() 
   : m_fileMappingHnd(0), m_fileHnd(invalid_handle), m_base(0)
{}

inline bool mmapped_file::open(const char *filename, fileoff_t file_offset, 
                               std::size_t size,     accessmode_t mode,
                               void *address)
{
   //Check if mapping is active
   if(this->is_open() || size == 0){
      return false;
   }

   unsigned long granularity = 0;
   unsigned long file_access  = 0;
   unsigned long file_creation_flags  = 0;
   unsigned long file_map_access = 0;
   unsigned long map_access = 0;
   unsigned long foffset_low;
   unsigned long foffset_high;

   //Set accesses
   if (mode == rw_mode){
      file_access       |= generic_read | generic_write;
      file_map_access   |= page_readwrite;
      map_access        |= file_map_write;
   }
   else if (mode == ro_mode){
      file_access       |= generic_read;
      file_map_access   |= page_readonly;
      map_access        |= file_map_read;
   }

   file_creation_flags = open_existing;

   //Open file using windows API since we need the handle
   this->m_fileHnd = create_file(filename, 
                                 file_access, 
                                 file_creation_flags);
   //Check for error
   if(this->m_fileHnd == invalid_handle){
      this->close();
      return false;
   }

   //Create file mapping
   this->m_fileMappingHnd = create_file_mapping(this->m_fileHnd, 
                                               file_map_access,
                                               0, 
                                               0, 
                                               0);
   //Check if all is correct
   if(!this->m_fileMappingHnd){
      this->close();
      return false;
   }

   //We can't map any file_offset so we have to obtain system's 
   //memory granularity
   system_info_t info;
   get_system_info(&info);
   granularity = info.dwAllocationGranularity;

   //Now we calculate valid offsets
   foffset_low  = (unsigned long)(file_offset / granularity) * granularity;
   foffset_high = (unsigned long)(((file_offset / granularity) * granularity) >> 32);

   //We calculate the difference between demanded and valid file_offset
   m_extra_offset = (file_offset - (file_offset / granularity) * granularity);

   //Store user values in memory
   m_file_offset = file_offset;
   m_size   = size;

   //Map file with new offsets and size
   m_base = map_view_of_file_ex(this->m_fileMappingHnd, 
                                map_access, 
                                foffset_high,
                                foffset_low, 
                                static_cast<std::size_t>(m_extra_offset + m_size), 
                                address);
   //Check error
   if (!m_base){
      this->close();
      return false;
   }
   //Calculate new base for the user
   m_base = reinterpret_cast<char*>(m_base) + m_extra_offset;
   return true;
}

inline bool mmapped_file::flush(std::size_t mapping_offset, std::size_t numbytes)
{
   //Check some errors
   if(m_base == 0)
      return false;

   if(mapping_offset >= m_size || (mapping_offset+numbytes)> m_size){
      return false;
   }

   if(numbytes == 0){
      numbytes = m_size-mapping_offset;
   }
   //Flush it all
   return 1 == flush_view_of_file(reinterpret_cast<char*>(m_base)+mapping_offset, 
                                  static_cast<std::size_t>(numbytes));
}

inline bool mmapped_file::is_open() const
{
   return m_fileHnd != invalid_handle;
}

inline void mmapped_file::close()
{
   if(m_base){
      this->flush();
      unmap_view_of_file(reinterpret_cast<char*>(m_base) - m_extra_offset);
      m_base = 0;
   }

   if(m_fileMappingHnd){
      close_handle(m_fileMappingHnd);
      m_fileMappingHnd = 0;
   }

   if(m_fileHnd){
      close_handle(m_fileHnd);
      m_fileHnd = invalid_handle;
   }
}

#else    //#if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

inline mmapped_file::mmapped_file() 
   : m_fileHnd(-1), m_base(MAP_FAILED)
{}

inline bool mmapped_file::open(const char *filename, fileoff_t file_offset, 
                               std::size_t size,     accessmode_t mode,
                               void *address)
{
   if(this->is_open() || size == 0){
      return false;
   }

   if(m_fileHnd != -1)   return false;

   //Create new mapping
   int oflag   = O_CREAT;
   int prot    = PROT_READ;

   if(mode == ro_mode){
      oflag |= O_RDONLY;
   }
   else if(mode == rw_mode){
      oflag |= O_RDWR;
      prot  |= PROT_WRITE;
   }
   else{
      return false;
   }

   //Open file and get handle
   m_fileHnd = ::open( filename,    // filename
                    oflag,   // read/write access
                    S_IRWXO | S_IRWXG | S_IRWXU);   // permissions
   if(m_fileHnd == -1){
      this->close();
      return false;
   }

   //Map it to the address space
   m_base   = mmap64(address, size, prot, MAP_SHARED, m_fileHnd, file_offset);
   m_file_offset = file_offset;
   m_size   = size;

   //Check if mapping was successful
   if(m_base == MAP_FAILED){
      this->close();
      return false;
   }

   return true;
}

inline bool mmapped_file::is_open() const
{
   return m_fileHnd != -1;
}

inline bool mmapped_file::flush(std::size_t mapping_offset, std::size_t numbytes)
{
   //Check for some errors
   if(!this->is_open())
      return false;

   if(mapping_offset >= m_size || (mapping_offset+numbytes)> m_size){
      return false;
   }

   if(numbytes == 0){
      numbytes = m_size - mapping_offset;
   }
   //Flush it all
   return msync(reinterpret_cast<char*>(m_base)+mapping_offset, 
                numbytes, MS_SYNC) == 0;
}

inline void mmapped_file::close()
{
   if(m_base != MAP_FAILED){
      this->flush();
      munmap(m_base, m_size);
      m_base = MAP_FAILED;
   }

   if(m_fileHnd != -1){
      ::close(m_fileHnd);
      m_fileHnd = -1;
   }
}

#endif   //##if (defined BOOST_WINDOWS) && !(defined BOOST_DISABLE_WIN32)

}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif   //BOOST_SHMEM_MMAPPED_FILE_HPP

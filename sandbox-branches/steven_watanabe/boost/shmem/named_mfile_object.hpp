//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_NAMED_MFILE_OBJECT_HPP
#define BOOST_SHMEM_NAMED_MFILE_OBJECT_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/detail/named_object_impl.hpp>
#include <boost/shmem/mmapped_file.hpp>
#include <string>
#include <fstream>

/*!\file
   Describes a named shared memory object allocation user class. 
*/

namespace boost {

namespace shmem {


/*!A basic shared memory named object creation class. Initializes the 
   shared memory segment. Inherits all basic functionality from 
   basic_named_object_impl<CharType, AllocationAlgorithm, IndexType>*/
template
      <
         class CharType, 
         class AllocationAlgorithm, 
         template<class IndexConfig> class IndexType
      >
class basic_named_mfile_object 
   : public detail::basic_named_object_impl <CharType, AllocationAlgorithm, IndexType>
{
 private:

   typedef detail::basic_named_object_impl 
      <CharType, AllocationAlgorithm, IndexType>             base_t;

 public: //functions

   typedef enum { 
                  ro_mode = mmapped_file::ro_mode, 
                  rw_mode = mmapped_file::rw_mode,
                }    accessmode_t;

   /*!Constructor. Allocates basic resources. Never throws.*/
   basic_named_mfile_object(){}

   /*!Destructor. Calls close. Never throws.*/
   ~basic_named_mfile_object()
      {  this->close();  }

   bool create(const char *name, size_t size)
   {
      //Create file with given size
      std::ofstream file(name, std::ios::binary | std::ios::trunc);
      file.seekp(static_cast<std::streamoff>(size-1));
      file.write("", 1);
      file.close();

      //Create mapped file
      if(!m_mfile.open(name, 0, size, (mmapped_file::accessmode_t)rw_mode)){
         return false;
      }

      //Create Shmem machinery
      if(!base_t::create_impl(m_mfile.get_base(), size)){
         close();
         return false;
      }

      m_filename = name;
      return true;    
   }

   bool open(const char *name, accessmode_t mode = rw_mode)
   {
      //Open file and get size
      std::ifstream file(name, std::ios::binary);
      std::size_t size = file.seekg(0, std::ios::end).tellg();
      file.close();

      //Create mapped file
      if(!m_mfile.open(name, 0, size, (mmapped_file::accessmode_t)mode)){
         return false;
      }

      //Open Shmem machinery
      if(!base_t::open_impl(m_mfile.get_base(), size)){
         close();
         return false;
      }

      m_filename = name;
      return true;    
   }

   /*!Flushes cached data to file. Never throws*/
   bool flush()
      {  return m_mfile.flush();  }

   /*!Frees resources. Never throws.*/
   void close()
   {  
      base_t::close_impl();
      m_mfile.close();
   }

   /*!Tries to resize mapped file so that we have room for 
      more objects. 
      WARNING: The memory mapping can change. To be able to use
      this function, all pointers constructed in this buffer
      must be offset pointers. Otherwise, the result is undefined.
      Returns true if the growth has been successful, so you will
      have some extra bytes to allocate new objects. If returns 
      false, the heap allocation has failed.*/
   bool grow(std::size_t extra_bytes)
   {  
      //If memory is reallocated, data will
      //be automatically copied
      std::size_t old_size = m_mfile.get_size();
      std::size_t new_size = old_size + extra_bytes;
      m_mfile.close();
      //Increase file size
      {
         std::ofstream file(m_filename.c_str(), 
                            std::ios::binary |std::ios::in | std::ios::out);
         if(!file){
            return false;
         }
         if(!file.seekp(static_cast<std::streamoff>(new_size - 1))){
            return false;
         }
         if(!file.write("", 1)){
            return false;
         }
      }

      if(!m_mfile.open(m_filename.c_str(), 0, new_size, 
                       (mmapped_file::accessmode_t)rw_mode)){
         return false;
      }

      //Grow always works
      base_t::close_impl();
      base_t::open_impl(m_mfile.get_base(), new_size);
      base_t::grow(extra_bytes);
      return true;
   }

 private:
   mmapped_file   m_mfile;
   std::string    m_filename;
};

}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif   //BOOST_SHMEM_NAMED_MFILE_OBJECT_HPP


//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_NAMED_SHARED_OBJECT_HPP
#define BOOST_SHMEM_NAMED_SHARED_OBJECT_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/detail/named_object_impl.hpp>
#include <boost/shmem/shared_memory.hpp>

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
class basic_named_shared_object 
   : public detail::basic_named_object_impl <CharType, AllocationAlgorithm, IndexType>
{
   ////////////////////////////////////////////////////////////////////////
   //
   //               Some internal helper structs/functors
   //
   ////////////////////////////////////////////////////////////////////////
   /*!This class defines an operator() that creates a shared memory
      of the requested size. The rest of the parameters are
      passed in the constructor. The class a template parameter
      to be used with create_from_file/create_from_istream functions
      of basic_named_object classes*/
   class segment_creator
   {
      public:
      segment_creator(shared_memory &shmem,
                      const char *mem_name, 
                      const void *addr)
      : m_shmem(shmem), m_mem_name(mem_name), m_addr(addr){}

      void *operator()(std::size_t size)
      {
         if(!m_shmem.create(m_mem_name, size, m_addr))
            return 0;
         return m_shmem.get_base(); 
      }      
      private:
      shared_memory &m_shmem;
      const char *m_mem_name;
      const void *m_addr;
   };

   /*!Functor to execute atomically when opening or creating a shared memory
      segment.*/
   struct create_open_func
   {
      enum type_t {   DoCreate, DoOpen, DoCreateOrOpen  };

      create_open_func(basic_named_shared_object * const frontend, type_t type)
         : m_frontend(frontend), m_type(type){}

      bool operator()(const shared_memory::segment_info_t * info, bool created) const
      {  
         if(((m_type == DoOpen)   &&  created) || 
            ((m_type == DoCreate) && !created))
            return false;

         if(created)
            return m_frontend->create_impl(info->get_user_ptr(), info->get_user_size());
         else
            return m_frontend->open_impl  (info->get_user_ptr(), info->get_user_size());
      }
      basic_named_shared_object *m_frontend;
      type_t                     m_type;
   };

   /*!Functor to execute atomically when closing a shared memory segment.*/
   struct close_func
   {
      close_func(basic_named_shared_object * const frontend)
         : m_frontend(frontend){}

      void operator()(const shared_memory::segment_info_t * info, bool last) const
      {  
         if(last) m_frontend->destroy_impl();
         else     m_frontend->close_impl();
      }
      basic_named_shared_object * m_frontend;
   };

   typedef detail::basic_named_object_impl 
      <CharType, AllocationAlgorithm, IndexType>                   base_t;

   //Friend declarations
   friend struct basic_named_shared_object::create_open_func;
   friend struct basic_named_shared_object::close_func;

 public: //functions

   /*!Constructor. Allocates basic resources. Never throws.*/
   basic_named_shared_object(){}

   /*!Destructor. Calls close. Never throws.*/
   ~basic_named_shared_object()
      {  this->close();  }

   /*!Creates shared memory and creates and places the segment manager. 
      This can throw.*/
   bool     create      (const char *name,      size_t size, 
                         const void *addr = 0)
   {
      create_open_func func(this, create_open_func::DoCreate);
      //Create shared memory
      return m_shmem.create_with_func(name, size, func, addr);    
   }

   /*!Creates shared memory and creates and places the segment manager if
      segment was not created. If segment was created it connects to the
      segment.
      This can throw.*/
   bool  open_or_create (const char *name,      size_t size, 
                         const void *addr = 0)
   {
      create_open_func func (this, create_open_func::DoCreateOrOpen);

      //Create shared memory
      return m_shmem.open_or_create_with_func(name, size, func, addr);
   } 

   /*!Connects to a created shared memory and it's the segment manager.
      Never throws.*/
   bool     open        (const char* name,   const void *addr = 0)
   {
      create_open_func func(this, create_open_func::DoOpen);
      //Create shared memory
      return m_shmem.open_with_func(name, func, addr);
   }

   /*!Frees resources. Never throws.*/
   void close()
      {  m_shmem.close_with_func(close_func(this));   }

   /*!Creates shared memory from file. Never throws.*/
   template<class CharT> 
   bool create_from_file (const CharT *filename, const char *mem_name, 
                          const void *addr = 0)
   {
      segment_creator mem_creator(m_shmem, mem_name, addr);
      return base_t::create_from_file(filename, mem_creator);
   }

   /*!Creates shared memory from an istream. Never throws.*/
   bool create_from_istream (std::istream &instream, std::size_t size,
                             const char *mem_name, const void *addr = 0)
   {
      segment_creator mem_creator(m_shmem, mem_name, addr);
      return base_t::create_from_istream(instream, size, mem_creator);
   }

 private:
   shared_memory  m_shmem;
};

}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif   //BOOST_SHMEM_NAMED_SHARED_OBJECT_HPP


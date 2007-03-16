//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_MULTI_NAMED_SHARED_OBJECT_HPP
#define BOOST_SHMEM_MULTI_NAMED_SHARED_OBJECT_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/detail/named_object_impl.hpp>
#include <boost/detail/no_exceptions_support.hpp>
//#include <boost/shmem/detail/multi_segment_services.hpp>
#include <boost/shmem/detail/utilities.hpp>
#include <boost/shmem/shared_memory.hpp>
#include <list>
#include <new>
#include <boost/shmem/containers/string.hpp>
#include <boost/shmem/streams/vectorstream.hpp>
#include <memory>

/*!\file
   Describes a named shared memory object allocation user class. 
*/

namespace boost {

namespace shmem {

/*!A basic shared memory named object creation class. Initializes the 
   shared memory segment. Inherits all basic functionality from 
   basic_named_object_impl<CharType, MemoryAlgorithm, IndexType>*/
template
      <
         class CharType, 
         class MemoryAlgorithm, 
         template<class IndexConfig> class IndexType
      >
class basic_multi_named_shared_object 
   :  public detail::basic_named_object_impl
         <CharType, MemoryAlgorithm, IndexType>
{

   typedef basic_multi_named_shared_object
               <CharType, MemoryAlgorithm, IndexType>    self_t;
   typedef typename self_t::void_pointer                 void_pointer;
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

   class group_services
   {
      public:
      typedef std::pair<void *, std::size_t>             result_type;
      typedef basic_multi_named_shared_object            frontend_t;
      typedef typename 
         basic_multi_named_shared_object::void_pointer   void_pointer;
      group_services(frontend_t *const frontend)
         :  mp_frontend(frontend), m_group(0), m_min_segment_size(0){}

      virtual std::pair<void *, std::size_t> create_new_segment(std::size_t alloc_size)
      {  
         //We should allocate an extra byte so that the
         //[base_addr + alloc_size] byte belongs to this segment
         alloc_size += 1;

         //If requested size is less than minimum, update that
         alloc_size = (m_min_segment_size > alloc_size) ? 
                       m_min_segment_size : alloc_size;
         if(mp_frontend->priv_new_segment(create_open_func::DoCreate,
                                          alloc_size, 0)){
            shmem_list_t::value_type &p_shm = *mp_frontend->m_shmem_list.rbegin();
            return result_type(p_shm->get_base(), p_shm->get_size()-1);
         }
         return result_type(0, 0);  
      }

      virtual ~group_services(){}

      void set_group(std::size_t group)
         {  m_group = group;  }

      std::size_t get_group() const
         {  return m_group;  }

      void set_min_segment_size(std::size_t min_segment_size)
         {  m_min_segment_size = min_segment_size;  }

      std::size_t get_min_segment_size() const
         {  return m_min_segment_size;  }

      private:

      frontend_t * const   mp_frontend;
      std::size_t          m_group;
      std::size_t          m_min_segment_size;
   };

   /*!Functor to execute atomically when opening or creating a shared memory
      segment.*/
   struct create_open_func
   {
      enum type_t {  DoCreate, DoOpen, DoOpenOrCreate  };
      typedef typename 
         basic_multi_named_shared_object::void_pointer   void_pointer;

      create_open_func(self_t * const    frontend,
                       type_t type, std::size_t segment_number)
         : mp_frontend(frontend), m_type(type), m_segment_number(segment_number){}

      bool operator()(const shared_memory::segment_info_t *info, bool created) const
      {  
         if(((m_type == DoOpen)   &&  created) || 
            ((m_type == DoCreate) && !created))
            return false;
         void *addr        = info->get_user_ptr();
         std::size_t size  = info->get_user_size();
         std::size_t group = mp_frontend->m_group_services.get_group();
         bool mapped       = false;
         bool impl_done    = false;

         //Associate this newly created segment as the
         //segment id = 0 of this group
         if((mapped = void_pointer::insert_mapping(group, m_segment_number, addr, size))){
            //Check if this is the master segment
            if(!m_segment_number){
               //Create or open the Shmem machinery
               if(impl_done = created ? 
                  mp_frontend->create_impl(addr, size) : mp_frontend->open_impl(addr, size)){
                  return true;
               }
            }
            else{
               return true;
            }
         }

         //This is the cleanup part
         //---------------
         if(impl_done){
            mp_frontend->close_impl();
         }
         if(mapped){
            bool ret = void_pointer::erase_mapping(group, 0); 
            assert(ret);(void)ret;
         }
         return false;
      }
      self_t * const    mp_frontend;
      type_t            m_type;
      std::size_t       m_segment_number;
   };

   /*!Functor to execute atomically when closing a shared memory segment.*/
   struct close_func
   {
      typedef typename 
         basic_multi_named_shared_object::void_pointer   void_pointer;

      close_func(self_t * const frontend)
         : mp_frontend(frontend){}

      void operator()(const shared_memory::segment_info_t * info, bool last) const
      {  
         if(last) mp_frontend->destroy_impl();
         else     mp_frontend->close_impl();
      }
      self_t * const    mp_frontend;
   };

   typedef detail::basic_named_object_impl 
      <CharType, MemoryAlgorithm, IndexType>                   base_t;

   //Friend declarations
   friend struct basic_multi_named_shared_object::create_open_func;
   friend struct basic_multi_named_shared_object::close_func;
   friend class basic_multi_named_shared_object::group_services;

   typedef std::list<shared_memory*> shmem_list_t;

   basic_multi_named_shared_object *get_this_pointer()
      {  return this;   }

 public:

   /*!Constructor. Allocates basic resources. Never throws.*/
   basic_multi_named_shared_object()
   :  m_group_services(get_this_pointer())
      {}

   /*!Destructor. Calls close. Never throws.*/
   ~basic_multi_named_shared_object()
      {  this->close();  }

   /*!Creates shared memory and creates there the segment manager. 
      This can throw.*/
   bool     create      (const char *name,      size_t size, 
                         const void *addr = 0)
   {
      return priv_open_or_create(create_open_func::DoCreate,name, size, addr);  
   }

   /*!Creates shared memory and creates and places the segment manager if segment
      was not created. If segment was created it connects to the segment. 
      This can throw.*/
   bool  open_or_create (const char *name,      size_t size, 
                         const void *addr = 0)
   {
      return priv_open_or_create(create_open_func::DoOpenOrCreate,name, size, addr);  
   }

   /*!Connects to a created shared memory and it's the segment manager.
      Never throws.*/
   bool     open        (const char* name,   const void *addr = 0)
   {
      return priv_open_or_create(create_open_func::DoOpen,name, 0, addr);  
   }

   bool  priv_open_or_create(typename create_open_func::type_t type, 
                             const char *name,
                             size_t size,
                             const void *addr = 0)
   {
      if(!m_shmem_list.empty())
         return false;
      std::size_t group = 0;
      BOOST_TRY{
         m_root_name = name;
         //Insert multi segment services and get a group identifier
         group = void_pointer::new_group(static_cast<void*>(&m_group_services));
         m_group_services.set_group(group);
         m_group_services.set_min_segment_size(size);

         if(group){
            if(this->priv_new_segment(type, size, addr)){
               return true;
            }
         }
      }
      BOOST_CATCH(const std::bad_alloc&){
      }
      BOOST_CATCH_END
      if(group){
         void_pointer::delete_group(group);
      }
      return false;      
   }

   bool  priv_new_segment(typename create_open_func::type_t type,
                          size_t size,
                          const void *addr)
   {
      BOOST_TRY{
         bool success = false;
         //This can throw.
         std::auto_ptr<shared_memory> shm (new shared_memory());
         //This can throw.
         m_shmem_list.push_back(shm.get());
         //Anti-exception rollback
         detail::value_eraser<shmem_list_t> 
            value_eraser(m_shmem_list, --m_shmem_list.end());
         //Get the pointer of the newly created shared_memory
         shared_memory *shm_ptr = *m_shmem_list.rbegin();
         //Get the number of groups of this multi_segment group
         std::size_t segment_id  = m_shmem_list.size()-1;
         //Format the name of the shared memory: append segment number. 
         boost::shmem::basic_ovectorstream<boost::shmem::string> formatter;
         formatter.seekp(std::ios::beg);
         //Pre-reserve string size
         std::size_t str_size = m_root_name.length()+10;
         if(formatter.vector().size() < str_size){
            //This can throw.
            formatter.resize(str_size);
         }
         //Format segment's name
         formatter << m_root_name
                   << static_cast<unsigned int>(segment_id) << std::ends;
         //This functor will be executed when constructing
         create_open_func func(this, type, segment_id);
         const char *name = formatter.vector().c_str();

         switch(type){
            case create_open_func::DoCreate:
               success = shm_ptr->create_with_func(name, size, func, addr);
            break;

            case create_open_func::DoOpen:
               success = shm_ptr->open_with_func(name, func, addr);
            break;

            case create_open_func::DoOpenOrCreate:
               success = shm_ptr->open_or_create_with_func(name, size, func, addr);
            break;

            default:
               return false;
            break;
         }
         //If all is ok, abort rollback actions
         if(success){
            shm.release();
            value_eraser.release();
            return true;
         }
      }
      BOOST_CATCH(const std::bad_alloc&){
      }
      BOOST_CATCH_END
      return false;
   }

   /*!Frees resources. Never throws.*/
   void close()
   {  
      if(!m_shmem_list.empty()){
         bool ret;
         //Obtain group identifier
         std::size_t group = m_group_services.get_group();
         //Erase main segment and its resources
         shmem_list_t::iterator  itbeg = m_shmem_list.begin(),
                                 itend = m_shmem_list.end(),
                                 it    = itbeg;
         (*itbeg)->close_with_func(close_func(this));
         //Delete group. All mappings are erased too.
         ret = void_pointer::delete_group(group);
         assert(ret);
         //Now destroy all shared_memory segments so they 
         //are unmapped from the process
         for(it = itbeg; it != itend; ++it) delete *it;
         m_shmem_list.clear();
      }
   }

   /*!Creates shared memory from file. Never throws.*/
   template<class CharT> 
   bool create_from_file (const CharT *filename, const char *mem_name, 
                          const void *addr = 0)
   {
      return false;
   }

   /*!Creates shared memory from an istream. Never throws.*/
   bool create_from_istream (std::istream &instream, std::size_t size,
                             const char *mem_name, const void *addr = 0)
   {
      return false;
   }

 private:
   shmem_list_t   m_shmem_list;
   group_services m_group_services;
   std::string    m_root_name;
};

}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif   //BOOST_SHMEM_MULTI_NAMED_SHARED_OBJECT_HPP


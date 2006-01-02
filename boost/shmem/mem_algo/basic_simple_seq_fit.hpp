//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_BASIC_SIMPLE_SEQ_FIT_HPP
#define BOOST_SHMEM_BASIC_SIMPLE_SEQ_FIT_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/shmem_fwd.hpp>
#include <boost/get_pointer.hpp>
#include <boost/noncopyable.hpp>
#include <boost/shmem/offset_ptr.hpp>
#include <boost/shmem/sync/shared_mutex.hpp>
#include <boost/shmem/exceptions.hpp>
#include <boost/shmem/detail/utilities.hpp>
#include <boost/shmem/detail/multi_segment_services.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <boost/type_traits/type_with_alignment.hpp>

#include <assert.h>
#include <new>

/*!\file
   Describes sequential fit algorithm used to allocate objects in shared memory.
   This class is intended as a base class for single segment and multi-segment
   implementations.
*/

namespace boost {

namespace shmem {

/*!This class implements the simple sequential fit algorithm with a simply
   linked list of free buffers.
   This class is intended as a base class for single segment and multi-segment
   implementations.*/
template<class MutexFamily, class VoidPointer>
class basic_simple_seq_fit : private boost::noncopyable
{
   public:
   /*!Shared mutex family used for the rest of the shmem framework*/
   typedef MutexFamily        mutex_family;
   /*!Pointer type to be used with the rest of the shmem framework*/
   typedef VoidPointer        void_pointer;

   private:
   struct block_ctrl;
   typedef typename detail::
      pointer_to_other<void_pointer, block_ctrl>::type block_ctrl_ptr;

   /*!Block control structure*/
   struct block_ctrl
   {
      /*!Offset pointer to the next block.*/
      block_ctrl_ptr m_next;
      /*!This block's memory size (including block_ctrl 
         header) in BasicSize units*/
      std::size_t    m_size;
   };

   /*!Shared mutex to protect memory allocate/deallocate*/
   typedef typename MutexFamily::mutex_t        mutex;

   /*!This struct includes needed data and derives from
      mutex to allow EBO when using null mutex*/
   struct header_t : public mutex
   {
      /*!Pointer to the first free block*/
      block_ctrl_ptr mp_free;
      /*!Allocated bytes for internal checking*/
      std::size_t    m_allocated;
      /*!The size of the memory segment*/
      std::size_t    m_size;
   }  m_header;

   public:
   /*!Constructor. "size" is the total size of the fixed size memory segment, 
      "extra_hdr_bytes" indicates the extra bytes beginning in the sizeof(basic_simple_seq_fit)
      offset that the allocator should not use at all.*/
   basic_simple_seq_fit           (size_t size, size_t extra_hdr_bytes);
   /*!Destructor.*/
   ~basic_simple_seq_fit();
   /*!Obtains the minimum size needed by the algorithm*/
   static size_t get_min_size (size_t extra_hdr_bytes);

   //Functions for single segment management

   /*!Allocates bytes, returns 0 if there is not more memory*/
   void* allocate             (size_t nbytes);
   /*!Deallocates previously allocated bytes*/
   void   deallocate          (void *addr);
   /*!Returns the size of the memory segment*/
   std::size_t get_size()  const;
   /*!Increases managed memory in extra_size bytes more*/
   void grow(std::size_t extra_size);

   //Functions for multi-segment management

   /*!Allocates bytes, if there is no more memory, it executes functor
      f(std::size_t) to allocate a new segment to manage. The functor returns 
      std::pair<void*, std::size_t> indicating the base address and size of 
      the new segment. If the new segment can't be allocated, allocate
      it will return 0.*/
   void* multi_allocate(size_t nbytes);

   private:
   /*!Real allocation algorithm*/
   void* priv_allocate(size_t nbytes);
   /*!Real deallocation algorithm*/
   void priv_deallocate(void *addr);
   /*!Makes a new memory portion available for allocation*/
   void priv_add_segment(void *addr, std::size_t size);

   enum { Alignment  = boost::alignment_of<boost::detail::max_align>::value  };
   enum { BasicSize  = detail::ct_rounded_size<sizeof(block_ctrl), Alignment>::value  };
   enum { MinAlloc   = 2*BasicSize  };
};

template<class MutexFamily, class VoidPointer>
inline basic_simple_seq_fit<MutexFamily, VoidPointer>::
   basic_simple_seq_fit(std::size_t size, std::size_t extra_hdr_bytes)
{
   //Initialize sizes and counters
   m_header.m_allocated = 0;
   m_header.m_size      = size;

   //Initialize pointers
   std::size_t block1_off  = detail::get_rounded_size(sizeof(*this)+extra_hdr_bytes, BasicSize);
   m_header.mp_free        = reinterpret_cast<block_ctrl*>
                              (detail::char_ptr_cast(this) + block1_off);
   m_header.mp_free->m_size  = (size - block1_off)/BasicSize;
   m_header.mp_free->m_next  = m_header.mp_free; 
}

template<class MutexFamily, class VoidPointer>
inline basic_simple_seq_fit<MutexFamily, VoidPointer>::~basic_simple_seq_fit()
{
   //There is a memory leak!
   //assert(m_header.m_allocated == 0);
   //assert(m_header.mp_free->m_next == m_header.mp_free);
}

template<class MutexFamily, class VoidPointer>
inline void basic_simple_seq_fit<MutexFamily, VoidPointer>::grow(std::size_t extra_size)
{  
   //Initialize pointers
   std::size_t block1_off  = detail::char_ptr_cast(m_header.mp_free.get()) - 
                             detail::char_ptr_cast(this);

   //Old highest address block's end offset from block1_off
   std::size_t old_end = ((m_header.m_size - block1_off)/BasicSize)*BasicSize + block1_off;

   //Update managed buffer's size
   m_header.m_size += extra_size;

   //We need at least MinAlloc blocks to create a new block
   if((m_header.m_size - old_end) < MinAlloc){
      return;
   }

   //We'll create a new free block with extra_size bytes
   block_ctrl *new_block = reinterpret_cast<block_ctrl*>
                              (detail::char_ptr_cast(this) + old_end);

   new_block->m_next = 0;
   new_block->m_size = (m_header.m_size - old_end)/BasicSize;
   m_header.m_allocated += new_block->m_size*BasicSize;
   this->priv_deallocate(detail::char_ptr_cast(new_block)+BasicSize);
}

template<class MutexFamily, class VoidPointer>
inline void basic_simple_seq_fit<MutexFamily, VoidPointer>::priv_add_segment(void *addr, std::size_t size)
{  
   //Check size
   assert(!(size < MinAlloc));
   if(size < MinAlloc)
      return;
   //Construct big block using all new segment
   block_ctrl *new_block   = static_cast<block_ctrl *>(addr);
   new_block->m_size       = size/BasicSize;
   new_block->m_next       = 0;
   //Simulate this block was previously allocated
   m_header.m_allocated   += new_block->m_size*BasicSize; 
   //Return block and insert it in the free block list
   this->priv_deallocate(detail::char_ptr_cast(new_block)+BasicSize);
}

template<class MutexFamily, class VoidPointer>
inline std::size_t basic_simple_seq_fit<MutexFamily, VoidPointer>::get_size()  const
   {  return m_header.m_size;  }

template<class MutexFamily, class VoidPointer>
inline std::size_t basic_simple_seq_fit<MutexFamily, VoidPointer>::
   get_min_size (std::size_t extra_hdr_bytes)
{
   return detail::get_rounded_size(sizeof(basic_simple_seq_fit)+extra_hdr_bytes , BasicSize) + 
          detail::get_rounded_size(sizeof(block_ctrl) , BasicSize);
}

template<class MutexFamily, class VoidPointer>
inline void* basic_simple_seq_fit<MutexFamily, VoidPointer>::allocate(std::size_t nbytes)
{
   //-----------------------
   boost::shmem::scoped_lock<mutex> guard(m_header);
   //-----------------------
   return priv_allocate(nbytes);
}

template<class MutexFamily, class VoidPointer>
inline void* basic_simple_seq_fit<MutexFamily, VoidPointer>::
   multi_allocate(std::size_t nbytes)
{
   //-----------------------
   boost::shmem::scoped_lock<mutex> guard(m_header);
   //-----------------------
   //Multisegment pointer. Let's try first the normal allocation
   //since it's faster.
   void *addr = priv_allocate(nbytes);
   if(!addr){
      //If this fails we will try the allocation through the segment
      //creator.
      std::size_t group, id;
      //Obtain the segment group of this segment
      void_pointer::get_group_and_id(this, group, id);
      if(group == 0){
         //Ooops, group 0 is not valid.
         return 0;
      }
      //Now obtain the polymorphic functor that creates
      //new segments and try to allocate again.
      boost::shmem::multi_segment_services *p_services = 
         static_cast<boost::shmem::multi_segment_services*>
                     (void_pointer::find_group_data(group));
      assert(p_services);
      std::pair<void *, std::size_t> ret = 
         p_services->create_new_segment(MinAlloc > nbytes ? MinAlloc : nbytes);
      if(ret.first){
         priv_add_segment(ret.first, ret.second);
         addr = priv_allocate(nbytes);
      }
   }
   return addr;
}

template<class MutexFamily, class VoidPointer>
void* basic_simple_seq_fit<MutexFamily, VoidPointer>::
   priv_allocate(std::size_t nbytes)
{
   using boost::get_pointer;

   if(!nbytes) return 0;

   //Number of units to request (including block_ctrl header)
   std::size_t nunits = (nbytes+BasicSize-1)/BasicSize + 1;

   //Get first two free memory blocks
   block_ctrl *prev  = get_pointer(m_header.mp_free);
   block_ctrl *block = get_pointer(prev->m_next);
   bool found = false;

   //Check available blocks for requested memory size
   block_ctrl * header_free = get_pointer(m_header.mp_free);
   do{
      if (block->m_size > nunits){
         //This block is bigger than needed, split it in 
         //two blocks, the first's size will be (block->m_size-units)
         //the second's size (units)
         block->m_size -= nunits;
         block        = reinterpret_cast<block_ctrl*>
                        (detail::char_ptr_cast(block) + BasicSize*block->m_size);
         block->m_size  = nunits;
         found = true;
      }
      else if (block->m_size == nunits && 
               header_free != block){
         //This block has exactly the right size, get it!
         //We check next != block, since we want to 
         //conserve always the lower block free, makes life easier.
         prev->m_next = block->m_next;
         found = true;
      }
      if(found){
         //We need block_ctrl for deallocation stuff, so
         //return memory user can overwrite
         m_header.m_allocated += BasicSize*nunits;
         //Check alignment
         assert((detail::char_ptr_cast(block) - detail::char_ptr_cast(this))
                % BasicSize == 0 );
         return detail::char_ptr_cast(block)+BasicSize;
      }
      //Bad luck, let's check next block
      prev  = block;
      block = get_pointer(block->m_next);
   }
   while(prev != header_free);
   
   return 0;
}

template<class MutexFamily, class VoidPointer>
void basic_simple_seq_fit<MutexFamily, VoidPointer>::deallocate(void* addr)
{
   if(!addr)   return;
   //-----------------------
   boost::shmem::scoped_lock<mutex> guard(m_header);
   //-----------------------
   return this->priv_deallocate(addr);
}

template<class MutexFamily, class VoidPointer>
void basic_simple_seq_fit<MutexFamily, VoidPointer>::priv_deallocate(void* addr)
{
   using boost::get_pointer;
   if(!addr)   return;

   //Let's get free block list. List is always sorted
   //by memory address to allow block merging.
   //Pointer next always points to the first 
   //(lower address) block
   block_ctrl_ptr pos   = m_header.mp_free;
   block_ctrl_ptr block = reinterpret_cast<block_ctrl*>
                           (detail::char_ptr_cast(addr) - BasicSize);

   //Check if alignment and block size are right
   if(!is_multisegment_ptr<void_pointer>::value){
      assert((detail::char_ptr_cast(addr) - detail::char_ptr_cast(this))
               % BasicSize == 0 );
   }
   assert(m_header.m_allocated >= BasicSize*block->m_size);
  

   //Update used memory count
   m_header.m_allocated -= BasicSize*block->m_size;   

   //Let's find the previous block of the block to deallocate
   //Remember pos >= get_pointer(pos->m_next) indicates that pos is the last block
   block_ctrl_ptr pos_next = pos->m_next;
   while(block > pos && !(block < pos_next || pos  >= pos_next)){
       pos = pos_next, pos_next = pos->m_next;
   }
 
   //Try to combine with upper block
   if ((detail::char_ptr_cast(get_pointer(block)) + BasicSize*block->m_size) == 
        detail::char_ptr_cast(get_pointer(pos_next))){
      block->m_size += pos_next->m_size;
      block->m_next  = pos_next->m_next;
   }
   else{
      block->m_next = pos->m_next;
   }
   //Try to combine with lower block
   if ((detail::char_ptr_cast(get_pointer(pos)) + BasicSize*pos->m_size) == 
        detail::char_ptr_cast(get_pointer(block))){
      pos->m_size += block->m_size;
      pos->m_next = block->m_next;
   }
   else{
      pos->m_next = block;
   }
}

}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif   //#ifndef BOOST_SHMEM_BASIC_SIMPLE_SEQ_FIT_HPP


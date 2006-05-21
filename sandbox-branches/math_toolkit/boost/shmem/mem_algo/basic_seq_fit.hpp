//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztañaga 2005. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/shmem for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_SHMEM_BASIC_SEQ_FIT_HPP
#define BOOST_SHMEM_BASIC_SEQ_FIT_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/shmem/detail/workaround.hpp>
#include <boost/shmem/detail/config_begin.hpp>
#include <boost/shmem/shmem_fwd.hpp>
#include <boost/shmem/offset_ptr.hpp>
#include <boost/shmem/sync/shared_mutex.hpp>
#include <boost/shmem/detail/utilities.hpp>
#include <boost/type_traits/alignment_of.hpp>
#include <boost/type_traits/type_with_alignment.hpp>
#include <boost/shmem/detail/multi_segment_services.hpp>
#include <boost/shmem/exceptions.hpp>
#include <boost/noncopyable.hpp>
#include <boost/static_assert.hpp>
#include <boost/type_traits/is_void.hpp>

#include <assert.h>
#include <new>

/*!\file
   Describes sequential fit algorithm used to allocate objects in shared memory.
   This class is intended as a base class for single segment and multi-segment
   implementations.
*/

namespace boost {

namespace shmem {

/*!This class implements the simple sequential fit algorithm with a simple
   linked list of free buffers.
   This class is intended as a base class for single segment and multi-segment
   implementations.*/
template<class MutexFamily, class VoidPointer>
class basic_seq_fit : private boost::noncopyable
{
   public:
   /*!Shared mutex to protect memory allocate/deallocate*/
   typedef MutexFamily                          mutex_family;
   typedef VoidPointer                          void_pointer;

   private:
   struct block_ctrl;
   typedef typename detail::
      pointer_to_other<void_pointer, block_ctrl>::type block_ctrl_ptr;
   /*!Block control structure*/
   struct block_ctrl
   {
      /*!Offset pointer to the previous block. This pointer
         can't be null, since it's a diff pointer.*/
      block_ctrl_ptr   m_prev;
      /*!Offset pointer to the next block.*/
      block_ctrl_ptr   m_next;
      /*!This block's memory size (including block_ctrl 
         header) in BasicSize units*/
      std::size_t m_size;
      /*!State of this memory portion free or occupied*/
      bool        m_free;
   };

   typedef typename MutexFamily::mutex_t  mutex;

   /*!This struct includes needed data and derives from
      mutex to allow EBO when using null mutex*/
   struct header_t : public mutex
   {
      typedef typename detail::pointer_to_other
         <void_pointer, char>::type char_ptr;
      /*!Pointer to the first free block*/
      char_ptr    mp_first;
      /*!Size of the memory used by the allocator*/
      std::size_t m_size;
      std::size_t m_extra_hdr_bytes;
   } m_header;

   public:
   /*!Constructor. "size" is the total size of the fixed size memory segment, 
      "extra_hdr_bytes" indicates the extra bytes beginning in the sizeof(basic_seq_fit)
      offset that the allocator should not use at all.*/
   basic_seq_fit           (std::size_t size, std::size_t res_size);
   /*!Obtains the minimum size needed by the algorithm*/
   static std::size_t get_min_size (std::size_t res_size);
  /*!Allocates bytes, returns 0 if there is not more memory*/
   void* allocate            (std::size_t nbytes);
   /*!Deallocates previously allocated bytes*/
   void   deallocate          (void *adr);
   /*!Returns the size of the memory segment*/
   std::size_t get_size()  const;
   /*!Increases managed memory in extra_size bytes more*/
   void grow(std::size_t extra_size);
  
   private:
   /*!Real allocation algorithm*/
   void* priv_allocate(std::size_t nbytes);
   void merge(block_ctrl *prev, block_ctrl *current, block_ctrl *next);
   void split(block_ctrl *addr, std::size_t nbytes);

   enum { Alignment  = boost::alignment_of<boost::detail::max_align>::value  };
   enum { BasicSize = detail::ct_rounded_size<sizeof(block_ctrl), Alignment>::value  };
   enum { MinAlloc   = 2*BasicSize  };
};

template<class MutexFamily, class VoidPointer>
inline void basic_seq_fit<MutexFamily, VoidPointer>::grow(std::size_t extra_size)
{  
   std::size_t block1_off  = detail::get_rounded_size(sizeof(*this)+m_header.m_extra_hdr_bytes, BasicSize);

   //Old highest address block's end offset from block1_off
   std::size_t old_end = ((m_header.m_size - block1_off)/BasicSize)*BasicSize + block1_off;

   //Update managed buffer's size
   m_header.m_size += extra_size;

   //We need at least MinAlloc blocks to create a new block
   if((m_header.m_size - old_end)< MinAlloc){
      return;
   }

   //We'll create a new free block with extra_size bytes
   block_ctrl *new_block = reinterpret_cast<block_ctrl*>
                              (detail::char_ptr_cast(this) + old_end);

   new_block->m_next = 0;
   new_block->m_size = (m_header.m_size - old_end)/BasicSize;
   new_block->m_free = true;

   block_ctrl* current   = reinterpret_cast<block_ctrl*>(&m_header.mp_first[0]);
   block_ctrl* null      = reinterpret_cast<block_ctrl*>(&m_header.mp_first[0]);

   while(detail::get_pointer(current->m_next) != null){
      current = current->m_next.get();
   }
   this->merge(current, new_block, 0);
}

template<class MutexFamily, class VoidPointer>
inline std::size_t basic_seq_fit<MutexFamily, VoidPointer>::get_size()  const
   {  return m_header.m_size;  }

template<class MutexFamily, class VoidPointer>
inline basic_seq_fit<MutexFamily, VoidPointer>::
   basic_seq_fit(std::size_t size, std::size_t extra_hdr_bytes)
{
   m_header.m_extra_hdr_bytes = extra_hdr_bytes;
   //Initialize pointers
   std::size_t b1off    = detail::get_rounded_size(sizeof(*this)+extra_hdr_bytes, BasicSize);
   m_header.mp_first    = detail::char_ptr_cast(this) + b1off;
   std::size_t nbytes   = size - b1off; 
   m_header.m_size      = nbytes;
   block_ctrl *null     = reinterpret_cast<block_ctrl*>(&m_header.mp_first[0]);
   block_ctrl *first    = reinterpret_cast<block_ctrl*>(&m_header.mp_first[0]);

   first->m_prev = null;
   first->m_next = null;
   first->m_free = true;
   first->m_size = (nbytes - BasicSize)/BasicSize;
}

template<class MutexFamily, class VoidPointer>
inline std::size_t basic_seq_fit<MutexFamily, VoidPointer>::get_min_size (std::size_t extra_hdr_bytes)
{
   return detail::get_rounded_size(
               detail::get_rounded_size(sizeof(basic_seq_fit)+extra_hdr_bytes, Alignment), 
               BasicSize) 
          + 
          MinAlloc;
}

template<class MutexFamily, class VoidPointer>
inline void* basic_seq_fit<MutexFamily, VoidPointer>::allocate(std::size_t nbytes)
{
   if(nbytes==0){
      return(0);
   }
   //-----------------------
   boost::shmem::scoped_lock<mutex> guard(m_header);
   //-----------------------
   return this->priv_allocate(nbytes);
}

template<class MutexFamily, class VoidPointer>
void* basic_seq_fit<MutexFamily, VoidPointer>::priv_allocate(std::size_t nbytes)
{
   nbytes               = detail::get_rounded_size(nbytes, BasicSize);
   std::size_t nblocks  = nbytes/BasicSize;

   //Traverse the linked list, starting with first element
   block_ctrl *current  = reinterpret_cast<block_ctrl*>(&m_header.mp_first[0]);
   block_ctrl *null     = reinterpret_cast<block_ctrl*>(&m_header.mp_first[0]);
   block_ctrl *next     = detail::get_pointer(current->m_next);
      
   for(;next != null;
        current = next, next = detail::get_pointer(current->m_next)){
      if(current->m_size >= nblocks && current->m_free == true){
         this->split(current, nblocks);
         assert((detail::char_ptr_cast(current) - detail::char_ptr_cast(this))
                % BasicSize == 0 );
         return current+1;
      }
   }

   //handle the last block ( which has current->m_next=null )
   if(current->m_size > nblocks && current->m_free == true){
      this->split(current, nblocks);
      assert((detail::char_ptr_cast(current) - detail::char_ptr_cast(this))
               % BasicSize == 0 );
      return current+1;
   }

   return 0;
}

template<class MutexFamily, class VoidPointer>
void basic_seq_fit<MutexFamily, VoidPointer>::deallocate(void* addr)
{
   //-----------------------
   boost::shmem::scoped_lock<mutex> guard(m_header);
   //-----------------------
   if(addr==0){
      return;
   }

   //perform sanity check to make sure address is kosher
   assert((detail::char_ptr_cast(addr) - detail::char_ptr_cast(this))
            % BasicSize == 0 );
   if( (addr >= &m_header.mp_first[m_header.m_size]) || (addr < &m_header.mp_first[0]) ){
      return;
   }

   //translate void* addr to index in m_header.mp_first[]
   block_ctrl* free     = reinterpret_cast<block_ctrl*>(addr)-1;
   block_ctrl_ptr end   = reinterpret_cast<block_ctrl*>(&m_header.mp_first[m_header.m_size]);

   //yet more sanity checks
   if(free->m_free  != false    ||                 //region if free
      detail::get_pointer(free->m_prev) >= free || //previous element not previous
      free->m_next  >= end      ||                 //next is beyond the end
      free->m_size  >= m_header.m_size/BasicSize ||//size region greater than whole
      free->m_size  == 0        ){                 //no size at all
      return;
   }
   this->merge(detail::get_pointer(free->m_prev), 
               free, 
               detail::get_pointer(free->m_next));
}

/*
breaks [free] region into [alloc][free] pair, if possible
*/
template<class MutexFamily, class VoidPointer>
void basic_seq_fit<MutexFamily, VoidPointer>::split
   (block_ctrl *addr, std::size_t nblocks)
{
   /*
   want payload to have enough room for
   nblocks     = size of request
   1           = header for new region
   1           = minimum payload for new region
   */
   if(addr->m_size >= nblocks + 2)
   {
      block_ctrl_ptr oldnext  = addr->m_next;
      block_ctrl_ptr oldprev  = addr->m_prev;
      std::size_t oldsize          = addr->m_size;
      block_ctrl_ptr newaddr  = addr + nblocks + 1;
      addr->m_next              = newaddr;
      addr->m_prev              = oldprev;
      addr->m_free              = false;
      addr->m_size              = nblocks;
      newaddr->m_next           = oldnext;
      newaddr->m_prev           = addr;
      newaddr->m_free           = true;
      newaddr->m_size           = oldsize-nblocks-1;
   }
   else{
      addr->m_free        = false;
   }
}

/*
4 cases ( F=free O=occupied )
FOF -> [F]
OOF -> O[F]
FOO -> [F]O
OOO -> OFO
*/
template<class MutexFamily, class VoidPointer>
void basic_seq_fit<MutexFamily, VoidPointer>::merge
                        (block_ctrl *prev, 
                         block_ctrl *current, 
                         block_ctrl *next)
{
   /*
   first handle special cases of region at end(s)
   prev=null low end
   next=null high end
   prev=null and next=null only 1 list element
   */
   block_ctrl *null = reinterpret_cast<block_ctrl*>(&m_header.mp_first[0]);
   block_ctrl_ptr temp;
   
   if(prev == null){
      if(next == null){
         current->m_free  = true;
      }
      else if(next->m_free == false){
         current->m_free  = true;
      }
      else if(next->m_free == true){
         current->m_free  = true;
         current->m_size  = current->m_size + next->m_size + 1;
         current->m_next  = next->m_next;
         temp           = next->m_next;
         temp->m_prev     = current;
      }
   }
   else if(next == null){
      if(prev->m_free == false){
         current->m_free = true;
      }
      else{
         prev->m_size  = prev->m_size + current->m_size + 1;
         prev->m_next  = current->m_next;
      }
   }
   /* now we handle 4 cases */
   else if(prev->m_free == false){
      if(next->m_free == false){
         current->m_free  = true;
      }
      else{
         current->m_free  = true;
         current->m_size  = current->m_size + next->m_size + 1;
         current->m_next  = next->m_next;
         temp           = next->m_next;

         if(detail::get_pointer(temp) != null){ 
            temp->m_prev  = current; 
         }
      }
   }
   else if(prev->m_free == true){
      if(next->m_free == false){
         prev->m_size  = prev->m_size + current->m_size + 1;
         prev->m_next  = current->m_next;
         next->m_prev  = prev;
      }
      else{
         prev->m_size  = prev->m_size + current->m_size + 2 + next->m_size;
         prev->m_next  = next->m_next;
         temp        = next->m_next;

         if(detail::get_pointer(temp) != null){ 
            temp->m_prev  = prev; 
         }
      }
   }
}

}  //namespace shmem {

}  //namespace boost {

#include <boost/shmem/detail/config_end.hpp>

#endif   //#ifndef BOOST_SHMEM_BASIC_SEQ_FIT_HPP

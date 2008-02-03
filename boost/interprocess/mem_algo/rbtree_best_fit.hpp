//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Ion Gaztanaga 2005-2008. Distributed under the Boost
// Software License, Version 1.0. (See accompanying file
// LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
// See http://www.boost.org/libs/interprocess for documentation.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef BOOST_INTERPROCESS_MEM_ALGO_RBTREE_BEST_FIT_HPP
#define BOOST_INTERPROCESS_MEM_ALGO_RBTREE_BEST_FIT_HPP

#if (defined _MSC_VER) && (_MSC_VER >= 1200)
#  pragma once
#endif

#include <boost/interprocess/detail/config_begin.hpp>
#include <boost/interprocess/detail/workaround.hpp>

#include <boost/interprocess/interprocess_fwd.hpp>
#include <boost/interprocess/mem_algo/detail/mem_algo_common.hpp>
#include <boost/interprocess/allocators/allocation_type.hpp>
#include <boost/interprocess/offset_ptr.hpp>
#include <boost/interprocess/sync/interprocess_mutex.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <boost/interprocess/detail/utilities.hpp>
#include <boost/interprocess/detail/min_max.hpp>
#include <boost/interprocess/detail/math_functions.hpp>
#include <boost/interprocess/detail/type_traits.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/assert.hpp>
#include <boost/static_assert.hpp>
#include <algorithm>
#include <utility>
#include <climits>
#include <cstring>
#include <iterator>

#include <cassert>
#include <new>

//#define BOOST_INTERPROCESS_RBTREE_BEST_FIT_USE_SPLAY

#ifndef BOOST_INTERPROCESS_RBTREE_BEST_FIT_USE_SPLAY
#include <boost/intrusive/set.hpp>
#else
//#include <boost/intrusive/splay_set.hpp>
//#include <boost/intrusive/avl_set.hpp>
#include <boost/intrusive/sg_set.hpp>
#endif

//!\file
//!Describes a best-fit algorithm based in an intrusive red-black tree used to allocate
//!objects in shared memory. This class is intended as a base class for single segment
//!and multi-segment implementations.

namespace boost {
namespace interprocess {

//!This class implements an algorithm that stores the free nodes in a red-black tree
//!to have logarithmic search/insert times.
template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
class rbtree_best_fit
{
   /// @cond
   //Non-copyable
   rbtree_best_fit();
   rbtree_best_fit(const rbtree_best_fit &);
   rbtree_best_fit &operator=(const rbtree_best_fit &);
   /// @endcond

   public:
   //!Shared interprocess_mutex family used for the rest of the Interprocess framework
   typedef MutexFamily        mutex_family;
   //!Pointer type to be used with the rest of the Interprocess framework
   typedef VoidPointer        void_pointer;
   typedef detail::basic_multiallocation_iterator
      <void_pointer> multiallocation_iterator;
   typedef detail::basic_multiallocation_chain
      <void_pointer> multiallocation_chain;

   /// @cond

   private:
   struct block_ctrl;
   typedef typename detail::
      pointer_to_other<void_pointer, block_ctrl>::type   block_ctrl_ptr;
   typedef typename detail::
      pointer_to_other<void_pointer, char>::type         char_ptr;

#ifndef BOOST_INTERPROCESS_RBTREE_BEST_FIT_USE_SPLAY
   typedef typename bi::make_set_base_hook
#else
//   typedef typename bi::make_splay_set_base_hook
//   typedef typename bi::make_avl_set_base_hook
   typedef typename bi::make_sg_set_base_hook
#endif
      < bi::void_pointer<VoidPointer>
      , bi::optimize_size<true>
      , bi::link_mode<bi::normal_link> >::type           TreeHook;

   typedef detail::multi_allocation_next<void_pointer>   multi_allocation_next_t;
   typedef typename multi_allocation_next_t::
      multi_allocation_next_ptr                          multi_allocation_next_ptr;

   struct SizeHolder
   {
      //!This block's memory size (including block_ctrl 
      //!header) in Alignment units
      std::size_t m_prev_size :  sizeof(std::size_t)*CHAR_BIT;
      std::size_t m_size      :  sizeof(std::size_t)*CHAR_BIT - 2;
      std::size_t m_prev_allocated :  1;
      std::size_t m_allocated :  1;
   };

   //!Block control structure
   struct block_ctrl
      :  public SizeHolder, public TreeHook
   {
      block_ctrl()
      {  this->m_size = 0; this->m_allocated = 0, this->m_prev_allocated = 0;  }

      friend bool operator<(const block_ctrl &a, const block_ctrl &b)
      {  return a.m_size < b.m_size;  }
      friend bool operator==(const block_ctrl &a, const block_ctrl &b)
      {  return a.m_size == b.m_size;  }
   };

   struct size_block_ctrl_compare
   {
      bool operator()(std::size_t size, const block_ctrl &block) const
      {  return size < block.m_size;  }

      bool operator()(const block_ctrl &block, std::size_t size) const
      {  return block.m_size < size;  }      
   };

   //!Shared interprocess_mutex to protect memory allocate/deallocate
   typedef typename MutexFamily::mutex_type                       interprocess_mutex;
#ifndef BOOST_INTERPROCESS_RBTREE_BEST_FIT_USE_SPLAY
   typedef typename bi::make_multiset
#else
   //typedef typename bi::make_splay_multiset
   //typedef typename bi::make_avl_multiset
   typedef typename bi::make_sg_multiset
#endif
      <block_ctrl, bi::base_hook<TreeHook> >::type                Imultiset;

   typedef typename Imultiset::iterator                           imultiset_iterator;

   //!This struct includes needed data and derives from
   //!interprocess_mutex to allow EBO when using null interprocess_mutex
   struct header_t : public interprocess_mutex
   {
      Imultiset            m_imultiset;

      //!The extra size required by the segment
      std::size_t       m_extra_hdr_bytes;
      //!Allocated bytes for internal checking
      std::size_t       m_allocated;
      //!The size of the memory segment
      std::size_t       m_size;
   }  m_header;

   friend class detail::basic_multiallocation_iterator<void_pointer>;
   friend class detail::memory_algorithm_common<rbtree_best_fit>;
   
   typedef detail::memory_algorithm_common<rbtree_best_fit> algo_impl_t;

   public:
   /// @endcond

   //!Constructor. "size" is the total size of the managed memory segment, 
   //!"extra_hdr_bytes" indicates the extra bytes beginning in the sizeof(rbtree_best_fit)
   //!offset that the allocator should not use at all.
   rbtree_best_fit           (std::size_t size, std::size_t extra_hdr_bytes);

   //!Destructor.
   ~rbtree_best_fit();

   //!Obtains the minimum size needed by the algorithm
   static std::size_t get_min_size (std::size_t extra_hdr_bytes);

   //Functions for single segment management

   //!Allocates bytes, returns 0 if there is not more memory
   void* allocate             (std::size_t nbytes);

   /// @cond

   //Experimental. Dont' use

   //!Multiple element allocation, same size
   multiallocation_iterator allocate_many(std::size_t elem_bytes, std::size_t num_elements);

   //!Multiple element allocation, different size
   multiallocation_iterator allocate_many(const std::size_t *elem_sizes, std::size_t n_elements, std::size_t sizeof_element);

   //!Multiple element allocation, different size
   void deallocate_many(multiallocation_iterator it);

   /// @endcond

   //!Deallocates previously allocated bytes
   void   deallocate          (void *addr);

   //!Returns the size of the memory segment
   std::size_t get_size()  const;

   //!Returns the number of free bytes of the segment
   std::size_t get_free_memory()  const;

   //!Initializes to zero all the memory that's not in use.
   //!This function is normally used for security reasons.
   void zero_free_memory();

   //!Increases managed memory in
   //!extra_size bytes more
   void grow(std::size_t extra_size);

   //!Decreases managed memory as much as possible
   void shrink_to_fit();

   //!Returns true if all allocated memory has been deallocated
   bool all_memory_deallocated();

   //!Makes an internal sanity check
   //!and returns true if success
   bool check_sanity();

   template<class T>
   std::pair<T *, bool>
      allocation_command  (allocation_type command,   std::size_t limit_size,
                           std::size_t preferred_size,std::size_t &received_size, 
                           T *reuse_ptr = 0);

   std::pair<void *, bool>
     raw_allocation_command  (allocation_type command,   std::size_t limit_object,
                              std::size_t preferred_object,std::size_t &received_object, 
                              void *reuse_ptr = 0, std::size_t sizeof_object = 1);

   //!Returns the size of the buffer previously allocated pointed by ptr
   std::size_t size(const void *ptr) const;

   //!Allocates aligned bytes, returns 0 if there is not more memory.
   //!Alignment must be power of 2
   void* allocate_aligned     (std::size_t nbytes, std::size_t alignment);

   /// @cond
   private:
   static std::size_t priv_first_block_offset(const void *this_ptr, std::size_t extra_hdr_bytes);

   std::pair<void*, bool>
      priv_allocation_command(allocation_type command,   std::size_t limit_size,
                        std::size_t preferred_size,std::size_t &received_size, 
                        void *reuse_ptr, std::size_t sizeof_object);


   //!Real allocation algorithm with min allocation option
   std::pair<void *, bool> priv_allocate(allocation_type command
                                        ,std::size_t limit_size
                                        ,std::size_t preferred_size
                                        ,std::size_t &received_size
                                        ,void *reuse_ptr = 0
                                        ,std::size_t backwards_multiple = 1);

   //!Obtains the block control structure of the user buffer
   static block_ctrl *priv_get_block(const void *ptr);

   //!Obtains the pointer returned to the user from the block control
   static void *priv_get_user_buffer(const block_ctrl *block);

   //!Returns the number of total units that a user buffer
   //!of "userbytes" bytes really occupies (including header)
   static std::size_t priv_get_total_units(std::size_t userbytes);

   //!Real expand function implementation
   bool priv_expand(void *ptr
                   ,const std::size_t min_size, const std::size_t preferred_size
                   ,std::size_t &received_size);

   //!Real expand to both sides implementation
   void* priv_expand_both_sides(allocation_type command
                               ,std::size_t min_size
                               ,std::size_t preferred_size
                               ,std::size_t &received_size
                               ,void *reuse_ptr
                               ,bool only_preferred_backwards
                               ,std::size_t backwards_multiple);

   //!Get poitner of the previous block (previous block must be free)
   block_ctrl * priv_prev_block(block_ctrl *ptr);

   //!Returns true if the previous block is allocated
   bool priv_is_prev_allocated(block_ctrl *ptr);

   //!Get a pointer of the "end" block from the first block of the segment
   block_ctrl * priv_end_block(block_ctrl *first_segment_block);

   //!Get the size in the tail of the previous block
   block_ctrl * priv_next_block(block_ctrl *ptr);

   //!Check if this block is free (not allocated)
   bool priv_is_allocated_block(block_ctrl *ptr);

   //!Marks the block as allocated
   void priv_mark_as_allocated_block(block_ctrl *ptr);

   //!Marks the block as allocated
   void priv_mark_as_free_block(block_ctrl *ptr);

   //!Checks if block has enough memory and splits/unlinks the block
   //!returning the address to the users
   void* priv_check_and_allocate(std::size_t units
                                ,block_ctrl* block
                                ,std::size_t &received_size);
   //!Real deallocation algorithm
   void priv_deallocate(void *addr);

   //!Makes a new memory portion available for allocation
   void priv_add_segment(void *addr, std::size_t size);

   void priv_mark_new_allocated_block(block_ctrl *block);

   public:
   
   static const std::size_t Alignment = !MemAlignment
      ? detail::alignment_of<detail::max_align>::value
      : MemAlignment
      ;

   private:
   //Due to embedded bits in size, Alignment must be at least 2
   BOOST_STATIC_ASSERT((Alignment >= 4));
   //Due to rbtree size optimizations, Alignment must have at least pointer alignment
   BOOST_STATIC_ASSERT((Alignment >= detail::alignment_of<void_pointer>::value));
   static const std::size_t AlignmentMask = (Alignment - 1);
   static const std::size_t BlockCtrlBytes = detail::ct_rounded_size<sizeof(block_ctrl), Alignment>::value;
   static const std::size_t BlockCtrlUnits = BlockCtrlBytes/Alignment;
   static const std::size_t AllocatedCtrlBytes  = detail::ct_rounded_size<sizeof(SizeHolder), Alignment>::value;
   static const std::size_t AllocatedCtrlUnits  = AllocatedCtrlBytes/Alignment;
   static const std::size_t EndCtrlBlockBytes   = detail::ct_rounded_size<sizeof(SizeHolder), Alignment>::value;
   static const std::size_t EndCtrlBlockUnits   = EndCtrlBlockBytes/Alignment;
   static const std::size_t MinBlockUnits       = BlockCtrlUnits;
   static const std::size_t UsableByPreviousChunk   = sizeof(std::size_t);

   //Make sure the maximum alignment is power of two
   BOOST_STATIC_ASSERT((0 == (Alignment & (Alignment - std::size_t(1u)))));
   /// @endcond
   public:
   static const std::size_t PayloadPerAllocation = AllocatedCtrlBytes - UsableByPreviousChunk;
};

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline std::size_t rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>
   ::priv_first_block_offset(const void *this_ptr, std::size_t extra_hdr_bytes)
{
   std::size_t uint_this      = (std::size_t)this_ptr;
   std::size_t main_hdr_end   = uint_this + sizeof(rbtree_best_fit) + extra_hdr_bytes;
   std::size_t aligned_main_hdr_end = detail::get_rounded_size(main_hdr_end, Alignment);
   std::size_t block1_off = aligned_main_hdr_end -  uint_this;
   algo_impl_t::assert_alignment(aligned_main_hdr_end);
   algo_impl_t::assert_alignment(uint_this + block1_off);
   return block1_off;
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   rbtree_best_fit(std::size_t size, std::size_t extra_hdr_bytes)
{
   //Initialize the header
   m_header.m_allocated       = 0;
   m_header.m_size            = size;
   m_header.m_extra_hdr_bytes = extra_hdr_bytes;

   //Now write calculate the offset of the first big block that will
   //cover the whole segment
   assert(get_min_size(extra_hdr_bytes) <= size);
   std::size_t block1_off  = priv_first_block_offset(this, extra_hdr_bytes);
   priv_add_segment(detail::char_ptr_cast(this) + block1_off, size - block1_off);
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::~rbtree_best_fit()
{
   //There is a memory leak!
//   assert(m_header.m_allocated == 0);
//   assert(m_header.m_root.m_next->m_next == block_ctrl_ptr(&m_header.m_root));
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
void rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::grow(std::size_t extra_size)
{
   //Get the address of the first block
   std::size_t block1_off =
      priv_first_block_offset(this, m_header.m_extra_hdr_bytes);

   block_ctrl *first_block = reinterpret_cast<block_ctrl *>
                                 (detail::char_ptr_cast(this) + block1_off);
   block_ctrl *old_end_block   = priv_end_block(first_block);
   assert(priv_is_allocated_block(old_end_block));
   std::size_t old_border_offset = (detail::char_ptr_cast(old_end_block) - 
                                    detail::char_ptr_cast(this)) + EndCtrlBlockBytes;

   //Update managed buffer's size
   m_header.m_size += extra_size;

   //We need at least MinBlockUnits blocks to create a new block
//   assert((m_header.m_size - old_end) >= MinBlockUnits);
   if((m_header.m_size - old_border_offset) < MinBlockUnits){
      return;
   }

   //Now create a new block between the old end and the new end
   std::size_t align_offset = (m_header.m_size - old_border_offset)/Alignment;
   block_ctrl *new_end_block = reinterpret_cast<block_ctrl*>
      (detail::char_ptr_cast(old_end_block) + align_offset*Alignment);
   new_end_block->m_size      = (detail::char_ptr_cast(first_block) - 
                                 detail::char_ptr_cast(new_end_block))/Alignment;
   first_block->m_prev_size = new_end_block->m_size;
   assert(first_block == priv_next_block(new_end_block));
   priv_mark_new_allocated_block(new_end_block);
   
   assert(new_end_block == priv_end_block(first_block));

   //The old end block is the new block
   block_ctrl *new_block = old_end_block;
   new_block->m_size = (detail::char_ptr_cast(new_end_block) - 
                        detail::char_ptr_cast(new_block))/Alignment;
   assert(new_block->m_size >= BlockCtrlUnits);
   priv_mark_new_allocated_block(new_block);
   assert(priv_next_block(new_block) == new_end_block);

   m_header.m_allocated += new_block->m_size*Alignment;

   //Now deallocate the newly created block
   this->priv_deallocate(priv_get_user_buffer(new_block));
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
void rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::shrink_to_fit()
{  
   //Get the address of the first block
   std::size_t block1_off =
      priv_first_block_offset(this, m_header.m_extra_hdr_bytes);

   block_ctrl *first_block = reinterpret_cast<block_ctrl*>
                                 (detail::char_ptr_cast(this) + block1_off);
   algo_impl_t::assert_alignment(first_block);

   block_ctrl *old_end_block = priv_end_block(first_block);
   algo_impl_t::assert_alignment(old_end_block);
   assert(priv_is_allocated_block(old_end_block));

   algo_impl_t::assert_alignment(old_end_block);

   std::size_t old_end_block_size = old_end_block->m_size;

   void *unique_buffer = 0;
   block_ctrl *last_block;
   if(priv_next_block(first_block) == old_end_block){
      std::size_t ignore;
      unique_buffer = priv_allocate(allocate_new, 0, 0, ignore).first;
      if(!unique_buffer)
         return;
      algo_impl_t::assert_alignment(unique_buffer);
      block_ctrl *unique_block = priv_get_block(unique_buffer);
      assert(priv_is_allocated_block(unique_block));
      algo_impl_t::assert_alignment(unique_block);
      last_block = priv_next_block(unique_block);
      assert(!priv_is_allocated_block(last_block));
      algo_impl_t::assert_alignment(last_block);
   }
   else{
      if(priv_is_prev_allocated(old_end_block))
         return;
      last_block = priv_prev_block(old_end_block);
   }

   std::size_t last_block_size = last_block->m_size;

   //Erase block from the free tree, since we will erase it
   m_header.m_imultiset.erase(Imultiset::s_iterator_to(*last_block));

   std::size_t shrunk_border_offset = (detail::char_ptr_cast(last_block) - 
                                       detail::char_ptr_cast(this)) + EndCtrlBlockBytes;
   
   block_ctrl *new_end_block = last_block;
   algo_impl_t::assert_alignment(new_end_block);
   new_end_block->m_size = old_end_block_size + last_block_size;
   priv_mark_as_allocated_block(new_end_block);

   //Although the first block might be allocated, we'll
   //store the offset to the end block since in the previous
   //offset can't be overwritten by a previous block
   first_block->m_prev_size = new_end_block->m_size;
   assert(priv_end_block(first_block) == new_end_block);

   //Update managed buffer's size
   m_header.m_size = shrunk_border_offset;
   if(unique_buffer)
      priv_deallocate(unique_buffer);
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
void rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   priv_add_segment(void *addr, std::size_t size)
{  
   //Check alignment
   algo_impl_t::check_alignment(addr);
   //Check size
   assert(size >= (BlockCtrlBytes + EndCtrlBlockBytes));

   //Initialize the first big block and the "end" node
   block_ctrl *first_big_block = new(addr)block_ctrl;
   first_big_block->m_size = size/Alignment - EndCtrlBlockUnits;
   assert(first_big_block->m_size >= BlockCtrlUnits);

   //The "end" node is just a node of size 0 with the "end" bit set
   block_ctrl *end_block = static_cast<block_ctrl*> 
      (new (reinterpret_cast<SizeHolder*>
         (detail::char_ptr_cast(addr) + first_big_block->m_size*Alignment))SizeHolder);

   //This will overwrite the prev part of the "end" node
   priv_mark_as_free_block (first_big_block);
   first_big_block->m_prev_size = end_block->m_size =
      (detail::char_ptr_cast(first_big_block) - detail::char_ptr_cast(end_block))/Alignment;
   priv_mark_as_allocated_block(end_block);

   assert(priv_next_block(first_big_block) == end_block);
   assert(priv_next_block(end_block) == first_big_block);
   assert(priv_end_block(first_big_block) == end_block);
   assert(priv_prev_block(end_block) == first_big_block);

   //Some check to validate the algorithm, since it makes some assumptions
   //to optimize the space wasted in bookkeeping:

   //Check that the sizes of the header are placed before the rbtree
   assert((void*)(SizeHolder*)first_big_block < (void*)(TreeHook*)first_big_block);

   //Check that the alignment is power of two (we use some optimizations based on this)
   //assert((Alignment % 2) == 0);
   //Insert it in the intrusive containers
   m_header.m_imultiset.insert(*first_big_block);
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline void rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   priv_mark_new_allocated_block(block_ctrl *new_block)
{  priv_mark_as_allocated_block(new_block);  }

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline std::size_t rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::get_size()  const
{  return m_header.m_size;  }

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline std::size_t rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::get_free_memory()  const
{
   return m_header.m_size - m_header.m_allocated - 
      priv_first_block_offset(this, m_header.m_extra_hdr_bytes);
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline std::size_t rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   get_min_size (std::size_t extra_hdr_bytes)
{
   return (algo_impl_t::ceil_units(sizeof(rbtree_best_fit)) +
           algo_impl_t::ceil_units(extra_hdr_bytes) + 
           MinBlockUnits + EndCtrlBlockUnits)*Alignment;
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline bool rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
    all_memory_deallocated()
{
   //-----------------------
   boost::interprocess::scoped_lock<interprocess_mutex> guard(m_header);
   //-----------------------
   std::size_t block1_off  = 
      priv_first_block_offset(this, m_header.m_extra_hdr_bytes);

   return m_header.m_allocated == 0 && 
      m_header.m_imultiset.begin() != m_header.m_imultiset.end() &&
       (++m_header.m_imultiset.begin()) == m_header.m_imultiset.end()
       && m_header.m_imultiset.begin()->m_size == 
         (m_header.m_size - block1_off - EndCtrlBlockBytes)/Alignment;
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
bool rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
    check_sanity()
{
   //-----------------------
   boost::interprocess::scoped_lock<interprocess_mutex> guard(m_header);
   //-----------------------
   imultiset_iterator ib(m_header.m_imultiset.begin()), ie(m_header.m_imultiset.end());

   std::size_t free_memory = 0;

   //Iterate through all blocks obtaining their size
   for(; ib != ie; ++ib){
      free_memory += ib->m_size*Alignment;
      algo_impl_t::assert_alignment(&*ib);
      if(!algo_impl_t::check_alignment(&*ib))
         return false;
   }

   //Check allocated bytes are less than size
   if(m_header.m_allocated > m_header.m_size){
      return false;
   }

   std::size_t block1_off  = 
      priv_first_block_offset(this, m_header.m_extra_hdr_bytes);

   //Check free bytes are less than size
   if(free_memory > (m_header.m_size - block1_off)){
      return false;
   }
   return true;
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline void* rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   allocate(std::size_t nbytes)
{  
   //-----------------------
   boost::interprocess::scoped_lock<interprocess_mutex> guard(m_header);
   //-----------------------
   std::size_t ignore;
   void * ret = priv_allocate(allocate_new, nbytes, nbytes, ignore).first;
   return ret;
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline void* rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   allocate_aligned(std::size_t nbytes, std::size_t alignment)
{ 
   //-----------------------
   boost::interprocess::scoped_lock<interprocess_mutex> guard(m_header);
   //-----------------------
   return algo_impl_t::allocate_aligned(this, nbytes, alignment); 
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
template<class T>
inline std::pair<T*, bool> rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   allocation_command  (allocation_type command,   std::size_t limit_size,
                        std::size_t preferred_size,std::size_t &received_size, 
                        T *reuse_ptr)
{
   std::pair<void*, bool> ret = priv_allocation_command
      (command, limit_size, preferred_size, received_size, reuse_ptr, sizeof(T));

   BOOST_ASSERT(0 == ((std::size_t)ret.first % detail::alignment_of<T>::value));
   return std::pair<T *, bool>(static_cast<T*>(ret.first), ret.second);
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline std::pair<void*, bool> rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   raw_allocation_command  (allocation_type command,   std::size_t limit_objects,
                        std::size_t preferred_objects,std::size_t &received_objects, 
                        void *reuse_ptr, std::size_t sizeof_object)
{
   if(!sizeof_object)
      return std::pair<void *, bool>(0, 0);
   if(command & try_shrink_in_place){
      bool success = algo_impl_t::try_shrink
         ( this, reuse_ptr, limit_objects*sizeof_object
         , preferred_objects*sizeof_object, received_objects);
      received_objects /= sizeof_object;
      return std::pair<void *, bool> ((success ? reuse_ptr : 0), true);
   }
   return priv_allocation_command
      (command, limit_objects, preferred_objects, received_objects, reuse_ptr, sizeof_object);
}


template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline std::pair<void*, bool> rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   priv_allocation_command (allocation_type command,   std::size_t limit_size,
                       std::size_t preferred_size,std::size_t &received_size, 
                       void *reuse_ptr, std::size_t sizeof_object)
{
   std::pair<void*, bool> ret;
   std::size_t max_count = m_header.m_size/sizeof_object;
   if(limit_size > max_count || preferred_size > max_count){
      ret.first = 0; return ret;
   }
   std::size_t l_size = limit_size*sizeof_object;
   std::size_t p_size = preferred_size*sizeof_object;
   std::size_t r_size;
   {
      //-----------------------
      boost::interprocess::scoped_lock<interprocess_mutex> guard(m_header);
      //-----------------------
      ret = priv_allocate(command, l_size, p_size, r_size, reuse_ptr, sizeof_object);
   }
   received_size = r_size/sizeof_object;
   return ret;
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline std::size_t rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   size(const void *ptr) const
{
   //We need no synchronization since this block's size is not going
   //to be modified by anyone else
   //Obtain the real size of the block
   return (priv_get_block(ptr)->m_size - AllocatedCtrlUnits)*Alignment + UsableByPreviousChunk;
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline void rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::zero_free_memory()
{
   //-----------------------
   boost::interprocess::scoped_lock<interprocess_mutex> guard(m_header);
   //-----------------------
   imultiset_iterator ib(m_header.m_imultiset.begin()), ie(m_header.m_imultiset.end());

   //Iterate through all blocks obtaining their size
   for(; ib != ie; ++ib){
      //Just clear user the memory part reserved for the user      
      std::memset( detail::char_ptr_cast(&*ib) + BlockCtrlBytes
                 , 0
                 , ib->m_size*Alignment - BlockCtrlBytes);
   }
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
void* rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   priv_expand_both_sides(allocation_type command
                         ,std::size_t min_size
                         ,std::size_t preferred_size
                         ,std::size_t &received_size
                         ,void *reuse_ptr
                         ,bool only_preferred_backwards
                         ,std::size_t backwards_multiple)
{
   algo_impl_t::assert_alignment(reuse_ptr);
   if(command & expand_fwd){
      if(priv_expand(reuse_ptr, min_size, preferred_size, received_size))
         return reuse_ptr;
   }
   else{
      received_size = this->size(reuse_ptr);
      if(received_size >= preferred_size || received_size >= min_size)
         return reuse_ptr;
   }

   if(backwards_multiple){
      BOOST_ASSERT(0 == (min_size       % backwards_multiple));
      BOOST_ASSERT(0 == (preferred_size % backwards_multiple));
   }

   if(command & expand_bwd){
      //Obtain the real size of the block
      block_ctrl *reuse = priv_get_block(reuse_ptr);

      //Sanity check 
      //assert(reuse->m_size == priv_tail_size(reuse));
      algo_impl_t::assert_alignment(reuse);

      block_ctrl *prev_block;

      //If the previous block is not free, there is nothing to do
      if(priv_is_prev_allocated(reuse)){
         return 0;
      }

      prev_block = priv_prev_block(reuse);
      assert(!priv_is_allocated_block(prev_block));

      //Some sanity checks
      assert(prev_block->m_size == reuse->m_prev_size);
      algo_impl_t::assert_alignment(prev_block);

      //Let's calculate the number of extra bytes of data before the current
      //block's begin. The value is a multiple of backwards_multiple
      std::size_t needs_backwards = preferred_size - 
         detail::get_truncated_size(received_size, backwards_multiple);

      const std::size_t lcm = detail::lcm(max_value(backwards_multiple, (std::size_t)Alignment)
                                         ,min_value(backwards_multiple, (std::size_t)Alignment));

      //If we want to use min_size data to get a buffer between preferred_size
      //and min_size if preferred_size can't be achieved, calculate the 
      //biggest of all possibilities
      if(!only_preferred_backwards){
         needs_backwards = min_size - detail::get_truncated_size(received_size, backwards_multiple);
      }

      assert((needs_backwards % backwards_multiple) == 0);

      const std::size_t needs_backwards_aligned = 
         detail::get_rounded_size(needs_backwards, lcm);

      //Check if previous block has enough size
      if(std::size_t(prev_block->m_size*Alignment) >= needs_backwards_aligned){
         //Now take all next space. This will succeed
         if(command & expand_fwd){
            std::size_t received_size2;
            if(!priv_expand(reuse_ptr, received_size, received_size, received_size2)){
               assert(0);
            }
            assert(received_size = received_size2);
         }
         //We need a minimum size to split the previous one
         if(prev_block->m_size >= (needs_backwards_aligned/Alignment + BlockCtrlUnits)){
            block_ctrl *new_block = reinterpret_cast<block_ctrl *>
               (detail::char_ptr_cast(reuse) - needs_backwards_aligned);

            //Free old previous buffer
            new_block->m_size = 
               AllocatedCtrlUnits + (needs_backwards_aligned + (received_size - UsableByPreviousChunk))/Alignment;
            assert(new_block->m_size >= BlockCtrlUnits);
            priv_mark_new_allocated_block(new_block);

            prev_block->m_size = (detail::char_ptr_cast(new_block) - 
                                  detail::char_ptr_cast(prev_block))/Alignment;
            assert(prev_block->m_size >= BlockCtrlUnits);
            priv_mark_as_free_block(prev_block);

            //Update the old previous block in the free chunks tree
            //If the new size fulfills tree invariants do nothing,
            //otherwise erase() + insert()
            {
               imultiset_iterator prev_block_it(Imultiset::s_iterator_to(*prev_block));
               imultiset_iterator was_smaller_it(prev_block_it);
               if(prev_block_it != m_header.m_imultiset.begin() && 
                  (--(was_smaller_it = prev_block_it))->m_size > prev_block->m_size){
                  m_header.m_imultiset.erase(prev_block_it);
                  m_header.m_imultiset.insert(m_header.m_imultiset.begin(), *prev_block);
               }
            }

            received_size = needs_backwards_aligned + received_size;
            m_header.m_allocated += needs_backwards_aligned;
         
            //Check alignment
            algo_impl_t::assert_alignment(new_block);

            //If the backwards expansion has remaining bytes in the
            //first bytes, fill them with a pattern
            void *p = priv_get_user_buffer(new_block);
            void *user_ptr = detail::char_ptr_cast(p);
            assert(((char*)reuse_ptr - (char*)user_ptr) % backwards_multiple == 0);
            algo_impl_t::assert_alignment(user_ptr);
            return user_ptr;
         }
         //Check if there is no place to create a new block and
         //the whole new block is multiple of the backwards expansion multiple
         else if(prev_block->m_size >= needs_backwards_aligned/Alignment &&
                 0 == ((prev_block->m_size*Alignment) % lcm)) {
            //Erase old previous block, since we will change it
            m_header.m_imultiset.erase(Imultiset::s_iterator_to(*prev_block));

            //Just merge the whole previous block
            needs_backwards = detail::get_truncated_size
               (prev_block->m_size*Alignment, backwards_multiple);
            //received_size = received_size/backwards_multiple*backwards_multiple + needs_backwards;
            received_size = received_size + needs_backwards;

            m_header.m_allocated += prev_block->m_size*Alignment;
            //Now update sizes
            prev_block->m_size = prev_block->m_size + reuse->m_size;
            assert(prev_block->m_size >= BlockCtrlUnits);
            priv_mark_new_allocated_block(prev_block);

            //If the backwards expansion has remaining bytes in the
            //first bytes, fill them with a pattern
            void *p = priv_get_user_buffer(prev_block);
            void *user_ptr = detail::char_ptr_cast(p);
            assert(((char*)reuse_ptr - (char*)user_ptr) % backwards_multiple == 0);
            algo_impl_t::assert_alignment(user_ptr);
            return user_ptr;
         }
         else{
            //Alignment issues
         }
      }
   }
   return 0;
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::multiallocation_iterator
   rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   allocate_many(std::size_t elem_bytes, std::size_t num_elements)
{
   //-----------------------
   boost::interprocess::scoped_lock<interprocess_mutex> guard(m_header);
   //-----------------------
   return algo_impl_t::allocate_many(this, elem_bytes, num_elements);
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline void rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   deallocate_many(typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::multiallocation_iterator it)
{
   //-----------------------
   boost::interprocess::scoped_lock<interprocess_mutex> guard(m_header);
   //-----------------------
   while(it){
      void *addr = &*it;
      ++it;
      this->priv_deallocate(addr);
   }
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::multiallocation_iterator
   rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   allocate_many(const std::size_t *elem_sizes, std::size_t n_elements, std::size_t sizeof_element)
{
   //-----------------------
   boost::interprocess::scoped_lock<interprocess_mutex> guard(m_header);
   //-----------------------
   return algo_impl_t::allocate_many(this, elem_sizes, n_elements, sizeof_element);
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
std::pair<void *, bool> rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   priv_allocate(allocation_type command
                ,std::size_t limit_size
                ,std::size_t preferred_size
                ,std::size_t &received_size
                ,void *reuse_ptr
               ,std::size_t backwards_multiple)
{
   //Remove me. Forbid backwards allocation
   //command &= (~expand_bwd);

   if(command & shrink_in_place){
      bool success = 
         algo_impl_t::shrink(this, reuse_ptr, limit_size, preferred_size, received_size);
      return std::pair<void *, bool> ((success ? reuse_ptr : 0), true);
   }

   typedef std::pair<void *, bool> return_type;
   received_size = 0;

   if(limit_size > preferred_size)
      return return_type(0, false);

   //Number of units to request (including block_ctrl header)
   std::size_t preferred_units = priv_get_total_units(preferred_size);

   //Number of units to request (including block_ctrl header)
   std::size_t limit_units = priv_get_total_units(limit_size);

   //Expand in place
   if(reuse_ptr && (command & (expand_fwd | expand_bwd))){
      void *ret = priv_expand_both_sides
         (command, limit_size, preferred_size, received_size, reuse_ptr, true, backwards_multiple);
      if(ret)
         return return_type(ret, true);
   }

   if(command & allocate_new){
      size_block_ctrl_compare comp;
      imultiset_iterator it(m_header.m_imultiset.lower_bound(preferred_units, comp));

      if(it != m_header.m_imultiset.end()){
         return return_type(this->priv_check_and_allocate
            (preferred_units, detail::get_pointer(&*it), received_size), false);
      }

      if(it != m_header.m_imultiset.begin()&&
              (--it)->m_size >= limit_units){
         return return_type(this->priv_check_and_allocate
            (it->m_size, detail::get_pointer(&*it), received_size), false);
      }
   }


   //Now try to expand both sides with min size
   if(reuse_ptr && (command & (expand_fwd | expand_bwd))){
      return return_type(priv_expand_both_sides
         (command, limit_size, preferred_size, received_size, reuse_ptr, false, backwards_multiple), true);
   }

   return return_type(0, false);
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline
typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::block_ctrl *
   rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::priv_get_block(const void *ptr)
{
   return reinterpret_cast<block_ctrl*>(detail::char_ptr_cast(ptr) - AllocatedCtrlBytes);
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline
void *rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
      priv_get_user_buffer(const typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::block_ctrl *block)
{  return detail::char_ptr_cast(block) + AllocatedCtrlBytes;   }

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
inline
std::size_t rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   priv_get_total_units(std::size_t userbytes)
{
   if(userbytes < UsableByPreviousChunk)
      userbytes = UsableByPreviousChunk;
   std::size_t units = detail::get_rounded_size(userbytes - UsableByPreviousChunk, Alignment)/Alignment + AllocatedCtrlUnits;
   if(units < BlockCtrlUnits) units = BlockCtrlUnits;
   return units;
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
bool rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::
   priv_expand (void *ptr
               ,const std::size_t min_size
               ,const std::size_t preferred_size
               ,std::size_t &received_size)
{
   //Obtain the real size of the block
   block_ctrl *block = priv_get_block(ptr);
   std::size_t old_block_units = block->m_size;

   //The block must be marked as allocated and the sizes must be equal
   assert(priv_is_allocated_block(block));
   //assert(old_block_units == priv_tail_size(block));
   
   //Put this to a safe value
   received_size = (old_block_units - AllocatedCtrlUnits)*Alignment + UsableByPreviousChunk;
   if(received_size >= preferred_size || received_size >= min_size)
      return true;

   //Now translate it to Alignment units
   const std::size_t min_user_units = algo_impl_t::ceil_units(min_size - UsableByPreviousChunk);
   const std::size_t preferred_user_units = algo_impl_t::ceil_units(preferred_size - UsableByPreviousChunk);

   //Some parameter checks
   assert(min_user_units <= preferred_user_units);

   block_ctrl *next_block;

   if(priv_is_allocated_block(next_block = priv_next_block(block))){
      return received_size >= min_size ? true : false;
   }
   algo_impl_t::assert_alignment(next_block);

   //Is "block" + "next_block" big enough?
   const std::size_t merged_units = old_block_units + next_block->m_size;

   //Now get the expansion size
   const std::size_t merged_user_units = merged_units - AllocatedCtrlUnits;

   if(merged_user_units < min_user_units){
      received_size = merged_units*Alignment - UsableByPreviousChunk;
      return false;
   }

   //Now get the maximum size the user can allocate
   std::size_t intended_user_units = (merged_user_units < preferred_user_units) ?
      merged_user_units : preferred_user_units;

   //These are total units of the merged block (supposing the next block can be split)
   const std::size_t intended_units = AllocatedCtrlUnits + intended_user_units;

   //Check if we can split the next one in two parts
   if((merged_units - intended_units) >=  BlockCtrlUnits){
      //This block is bigger than needed, split it in 
      //two blocks, the first one will be merged and
      //the second's size will be the remaining space
      assert(next_block->m_size == priv_next_block(next_block)->m_prev_size);
      const std::size_t rem_units = merged_units - intended_units;

      //Check if we we need to update the old next block in the free chunks tree
      //If the new size fulfills tree invariants, we just need to replace the node
      //(the block start has been displaced), otherwise erase() + insert().
      //
      //This fixup must be done in two parts, because the new next chunk might
      //overwrite the tree hook of the old next chunk. So we first erase the
      //old if needed and we'll insert the new one after creating the new next
      imultiset_iterator old_next_block_it(Imultiset::s_iterator_to(*next_block));
      const bool size_invariants_broken = 
            (next_block->m_size - rem_units ) < BlockCtrlUnits ||
            (old_next_block_it != m_header.m_imultiset.begin() && 
            (--imultiset_iterator(old_next_block_it))->m_size > rem_units);
      if(size_invariants_broken){
         m_header.m_imultiset.erase(old_next_block_it);
      }
      //This is the remaining block
      block_ctrl *rem_block = new(reinterpret_cast<block_ctrl*>
                     (detail::char_ptr_cast(block) + intended_units*Alignment))block_ctrl;
      rem_block->m_size  = rem_units;
      algo_impl_t::assert_alignment(rem_block);
      assert(rem_block->m_size >= BlockCtrlUnits);
      priv_mark_as_free_block(rem_block);

      //Now the second part of the fixup
      if(size_invariants_broken)
         m_header.m_imultiset.insert(m_header.m_imultiset.begin(), *rem_block);
      else
         m_header.m_imultiset.replace_node(old_next_block_it, *rem_block);

      //Write the new length
      block->m_size = intended_user_units + AllocatedCtrlUnits;
      assert(block->m_size >= BlockCtrlUnits);
      m_header.m_allocated += (intended_units - old_block_units)*Alignment;
   }
   //There is no free space to create a new node: just merge both blocks
   else{
      //Now we have to update the data in the tree
      m_header.m_imultiset.erase(Imultiset::s_iterator_to(*next_block));

      //Write the new length
      block->m_size = merged_units;
      assert(block->m_size >= BlockCtrlUnits);
      m_header.m_allocated += (merged_units - old_block_units)*Alignment;
   }
   priv_mark_as_allocated_block(block);
   received_size = (block->m_size - AllocatedCtrlUnits)*Alignment + UsableByPreviousChunk;
   return true;
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment> inline
typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::block_ctrl *
   rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::priv_prev_block
      (typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::block_ctrl *ptr)
{
   assert(!ptr->m_prev_allocated);
   return reinterpret_cast<block_ctrl *>
      (detail::char_ptr_cast(ptr) - ptr->m_prev_size*Alignment);
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment> inline
bool rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::priv_is_prev_allocated
      (typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::block_ctrl *ptr)
{
   if(ptr->m_prev_allocated){
      return true;
   }
   else{
      block_ctrl *prev = priv_prev_block(ptr);
      (void)prev;
      assert(!priv_is_allocated_block(prev));
      return false;
   }
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment> inline
typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::block_ctrl *
   rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::priv_end_block
      (typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::block_ctrl *first_segment_block)
{
   assert(first_segment_block->m_prev_allocated);
   block_ctrl *end_block = reinterpret_cast<block_ctrl *>
      (detail::char_ptr_cast(first_segment_block) - first_segment_block->m_prev_size*Alignment);
   (void)end_block;
   assert(priv_is_allocated_block(end_block));
   assert(end_block > first_segment_block);
   return end_block;
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment> inline
typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::block_ctrl *
   rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::priv_next_block
      (typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::block_ctrl *ptr)
{
   return reinterpret_cast<block_ctrl *>
      (detail::char_ptr_cast(ptr) + ptr->m_size*Alignment);
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment> inline
bool rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::priv_is_allocated_block
      (typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::block_ctrl *block)
{
   bool allocated = block->m_allocated != 0;
   block_ctrl *next_block = (block_ctrl *)
      (detail::char_ptr_cast(block) + block->m_size*Alignment);
   bool next_block_prev_allocated = next_block->m_prev_allocated != 0;
   (void)next_block_prev_allocated;
   assert(allocated == next_block_prev_allocated);
   return allocated;
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment> inline
void rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::priv_mark_as_allocated_block
      (typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::block_ctrl *block)
{
   //assert(!priv_is_allocated_block(block));
   block->m_allocated = 1;
   ((block_ctrl *)(((char*)block) + block->m_size*Alignment))->m_prev_allocated = 1;
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment> inline
void rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::priv_mark_as_free_block
      (typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::block_ctrl *block)
{
   block->m_allocated = 0;
   ((block_ctrl *)(((char*)block) + block->m_size*Alignment))->m_prev_allocated = 0;
   //assert(!priv_is_allocated_block(ptr));
   priv_next_block(block)->m_prev_size = block->m_size;
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment> inline
void* rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::priv_check_and_allocate
   (std::size_t nunits
   ,typename rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::block_ctrl* block
   ,std::size_t &received_size)
{
   std::size_t upper_nunits = nunits + BlockCtrlUnits;
   imultiset_iterator it_old = Imultiset::s_iterator_to(*block);
   algo_impl_t::assert_alignment(block);

   if (block->m_size >= upper_nunits){
      //This block is bigger than needed, split it in 
      //two blocks, the first's size will be "units" and
      //the second's size "block->m_size-units"
      std::size_t block_old_size = block->m_size;
      block->m_size = nunits;
      assert(block->m_size >= BlockCtrlUnits);

      //This is the remaining block
      block_ctrl *rem_block = new(reinterpret_cast<block_ctrl*>
                     (detail::char_ptr_cast(block) + Alignment*nunits))block_ctrl;
      algo_impl_t::assert_alignment(rem_block);
      rem_block->m_size  = block_old_size - nunits;
      assert(rem_block->m_size >= BlockCtrlUnits);
      priv_mark_as_free_block(rem_block);

      imultiset_iterator it_hint;
      if(it_old == m_header.m_imultiset.begin()
         || (--imultiset_iterator(it_old))->m_size < rem_block->m_size){
         //option a: slow but secure
         //m_header.m_imultiset.insert(m_header.m_imultiset.erase(it_old), *rem_block);
         //option b: Construct an empty node and swap
         //Imultiset::init_node(*rem_block);
         //block->swap_nodes(*rem_block);
         //option c: replace the node directly
         m_header.m_imultiset.replace_node(Imultiset::s_iterator_to(*it_old), *rem_block);
      }
      else{
         //Now we have to update the data in the tree
         m_header.m_imultiset.erase(it_old);
         m_header.m_imultiset.insert(m_header.m_imultiset.begin(), *rem_block);
      }
         
   }
   else if (block->m_size >= nunits){
      m_header.m_imultiset.erase(it_old);
   }
   else{
      assert(0);
      return 0;
   }
   //We need block_ctrl for deallocation stuff, so
   //return memory user can overwrite
   m_header.m_allocated += block->m_size*Alignment;
   received_size =  (block->m_size - AllocatedCtrlUnits)*Alignment + UsableByPreviousChunk;

   //Mark the block as allocated
   priv_mark_as_allocated_block(block);

   //Clear the memory occupied by the tree hook, since this won't be
   //cleared with zero_free_memory
   TreeHook *t = static_cast<TreeHook*>(block);
   std::memset(t, 0, sizeof(*t));
   this->priv_next_block(block)->m_prev_size = 0;
   return priv_get_user_buffer(block);
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
void rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::deallocate(void* addr)
{
   if(!addr)   return;
   //-----------------------
   boost::interprocess::scoped_lock<interprocess_mutex> guard(m_header);
   //-----------------------
   return this->priv_deallocate(addr);
}

template<class MutexFamily, class VoidPointer, std::size_t MemAlignment>
void rbtree_best_fit<MutexFamily, VoidPointer, MemAlignment>::priv_deallocate(void* addr)
{
   if(!addr)   return;

   block_ctrl *block = priv_get_block(addr);
  
   //The blocks must be marked as allocated and the sizes must be equal
   assert(priv_is_allocated_block(block));
//   assert(block->m_size == priv_tail_size(block));

   //Check if alignment and block size are right
   algo_impl_t::assert_alignment(addr);

   std::size_t block_old_size = Alignment*block->m_size;
   assert(m_header.m_allocated >= block_old_size);

   //Update used memory count
   m_header.m_allocated -= block_old_size;

   //The block to insert in the tree
   block_ctrl *block_to_insert = block;

   //Get the next block
   block_ctrl *next_block  = priv_next_block(block);
   bool merge_with_prev    = !priv_is_prev_allocated(block);
   bool merge_with_next    = !priv_is_allocated_block(next_block);

   //Merge logic. First just update block sizes, then fix free chunks tree
   if(merge_with_prev || merge_with_next){
      //Merge if the previous is free
      if(merge_with_prev){
         //Get the previous block
         block_ctrl *prev_block = priv_prev_block(block);
         prev_block->m_size += block->m_size;
         assert(prev_block->m_size >= BlockCtrlUnits);
         block_to_insert = prev_block;
      }
      //Merge if the next is free
      if(merge_with_next){
         block_to_insert->m_size += next_block->m_size;
         assert(block_to_insert->m_size >= BlockCtrlUnits);
         if(merge_with_prev)
            m_header.m_imultiset.erase(Imultiset::s_iterator_to(*next_block));
      }

      bool only_merge_next = !merge_with_prev && merge_with_next;
      imultiset_iterator free_block_to_check_it
         (Imultiset::s_iterator_to(only_merge_next ? *next_block : *block_to_insert));
      imultiset_iterator was_bigger_it(free_block_to_check_it);

      //Now try to shortcut erasure + insertion (O(log(N))) with
      //a O(1) operation if merging does not alter tree positions
      if(++was_bigger_it != m_header.m_imultiset.end()   &&
         block_to_insert->m_size > was_bigger_it->m_size ){
         m_header.m_imultiset.erase(free_block_to_check_it);
         m_header.m_imultiset.insert(m_header.m_imultiset.begin(), *block_to_insert);
      }
      else if(only_merge_next){
         m_header.m_imultiset.replace_node(free_block_to_check_it, *block_to_insert);
      }
   }
   else{
      m_header.m_imultiset.insert(m_header.m_imultiset.begin(), *block_to_insert);
   }
   priv_mark_as_free_block(block_to_insert);
}

}  //namespace interprocess {
}  //namespace boost {

#include <boost/interprocess/detail/config_end.hpp>

#endif   //#ifndef BOOST_INTERPROCESS_MEM_ALGO_RBTREE_BEST_FIT_HPP

